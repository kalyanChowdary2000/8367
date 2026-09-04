
#pragma STRING (far)
/*
 * Copyright Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Program  : Light-weight Protocol Stack
 * Abstract : 
 * Author   : 
 * $Id: $
 */

#include <local/lwps_config.h>

#if LWPS_UDP

/*  
 * Include Files 
 */
#include <lwps/lwps.h>

/*
 * Symbol Definition 
 */
#define UDP_PORT_ANY    (0)
#define UDP_PORT_BEGIN  (21024)
#define UDP_PORT_MAX    (65535)

/*
 * Data Declaration 
 */
static udp_pcb_t     xdata udp_pcb_entry[UDP_MAX_CONNECTION];
static udp_handler_t xdata _UdpHandler;
static ip_addr_t     xdata ip_addr_empty = { { 0, 0, 0, 0 } };
static uint32        xdata udp_datagrams_delivered = 0;    /* Counter for delivered datagrams */
static uint32        xdata udp_datagrams_no_app = 0;       /* Counter for datagrams with no application */
static uint32        xdata udp_datagrams_failed_other = 0; /* Counter for datagrams failed for other reasons */
static uint32        xdata udp_datagrams_sent = 0;         /* Counter for datagrams sent */

/*
 * Macro Definition
 */
#define IF_TRUE_RET(exp, ret)           do {    \
    if (exp) {                                                                          \
        OSAL_PRINTF("%s():%d (" #exp ") => return " #ret "\n", __FILE__, (uint16)__LINE__); \
        return ret;                                                                     \
    }                                                                                   \
} while (0)
#define IS_PCBID_INVALID(idx)           (idx < 0 || idx >= UDP_MAX_CONNECTION)
#define IS_PCB_VALID(idx)               (udp_pcb_entry[idx].valid == TRUE)

/*
 * Function Declaration 
 */
#if DBG_LWPS_UDP_ENABLE
static void udp_hdr_dump(udp_hdr_t *pUdpHdr) large
{
    //OSAL_PRINTF("%s():%d pUdpHdr = %p\n", __FILE__, (uint16)__LINE__, pUdpHdr);
    OSAL_PRINTF("-- [UDP HDR dump] -----------------------------\n");
    OSAL_PRINTF("   Src Port: 0x%04X (%d)\n", pUdpHdr->src_port, pUdpHdr->src_port);
    OSAL_PRINTF("   Dst Port: 0x%04X (%d)\n", pUdpHdr->dst_port, pUdpHdr->dst_port);
    OSAL_PRINTF("     Length: 0x%04X (%d)\n", pUdpHdr->length, pUdpHdr->length);
    OSAL_PRINTF("   Checksum: 0x%04X\n", pUdpHdr->chksum);
    OSAL_PRINTF("-----------------------------------------------\n");
}
#endif /* end of DBG_LWPS_UDP_ENABLE */

static void udp_delPCB(uint16 idx) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

    if (idx >= UDP_MAX_CONNECTION)
    {
        DBG_LWPS_UDP_PRINTF(("%s():%d - idx (%lu) error\n", __FILE__, (uint16)__LINE__, idx));
        return;
    }

    udp_pcb_entry[idx].local_ip = ip_addr_empty;
    udp_pcb_entry[idx].remote_ip = ip_addr_empty;
    udp_pcb_entry[idx].local_port = 0;
    udp_pcb_entry[idx].remote_port = 0;
    udp_pcb_entry[idx].parent_id = idx; /* self */
}

void udp_timer(void) large
{
    return;
}

int16 udp_getPCB(uint16 idx, udp_pcb_t **ppUdpPCB) large
{
    udp_pcb_t *pUdpPCB = (udp_pcb_t *)(*((uint16*)ppUdpPCB));
    IF_TRUE_RET(IS_PCBID_INVALID(idx), LWPS_ERR);
    IF_TRUE_RET(ppUdpPCB == NULL, LWPS_ERR_NULLPOINT);

    /* Assign pointer */
    pUdpPCB = &udp_pcb_entry[idx];

    return LWPS_OK;
}

int16 udp_setHandler(udp_handler_t *pUdpHandler) large
{
    IF_TRUE_RET(pUdpHandler == NULL, LWPS_ERR_NULLPOINT);

    _UdpHandler.event_connect = pUdpHandler->event_connect;
    _UdpHandler.event_disconnect = pUdpHandler->event_disconnect;
    _UdpHandler.event_recvfrom = pUdpHandler->event_recvfrom;

    return LWPS_OK;
}

int16 udp_init(void) large
{
    int32 i;

    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

    /* Clear handler */
    _UdpHandler.event_connect = NULL;
    _UdpHandler.event_disconnect = NULL;
    _UdpHandler.event_recvfrom = NULL;

    /* Reset PCB */
    for (i=0; i<UDP_MAX_CONNECTION; i++) {
        udp_delPCB(i);
    }

    /* Initialize datagram counters */
    udp_datagrams_delivered = 0;
    udp_datagrams_no_app = 0;
    udp_datagrams_failed_other = 0;
    udp_datagrams_sent = 0;

    return LWPS_OK;
}

#if (defined(LWS_A_SET) || defined(UPGRADE_TFTP))
static int16 udp_localport_check(uint16 pcbId) large
{
    uint16 lport;
    int32 i;

    /* find a valid port if local-port is invalid */
    if (udp_pcb_entry[pcbId].local_port == UDP_PORT_ANY)
    {
        for (lport=UDP_PORT_BEGIN; lport<UDP_PORT_MAX; lport++)
        {
            for (i=0; i<UDP_MAX_CONNECTION; i++)
            {
                if ((i != pcbId) && (udp_pcb_entry[i].local_port == lport))
                {
                    break;
                }
            }

            /* is valid port? */
            if (udp_pcb_entry[i].local_port != lport)
            {
                udp_pcb_entry[pcbId].local_port = lport;
                break;
            }
        }

        /* not found vaild-port */
        if (udp_pcb_entry[pcbId].local_port == UDP_PORT_ANY)
        {
            OSAL_PRINTF("%s():%d Cannot find a vaild port.\n", __FILE__, (uint16)__LINE__);

            return LWPS_ERR;
        }
    }

    return LWPS_OK;
}

static int16 udp_xmit_to(uint16 pcbId, uint8 *pData, uint16 length, ip_addr_t ip, uint16 port) large
{
    pktBuf_t *pBuf_Xmit = &pktbuf;
    udp_hdr_t *pUdpHdr_Xmit;
    uint8 *pUdpDatagram;

    DBG_LWPS_UDP_PRINTF(("%s():%d pcbId = %d, pData = %p, length = %d\n", \
        __FILE__, (uint16)__LINE__, pcbId, pData, length));

    /* [WARN] DO NOT check pcbId, because it's vaild likely */
#if 0
    if ((pData == NULL) && (length == 0)) {
        return LWPS_ERR_NULLPOINT;
    }
#endif

    /* [FIXME] fix length of pbuf note
     * <IP alignment> ETH (6+6+2) + [RTK (8)] + VLAN (4) + IP (20) + UDP (8)
     * DON'T have to consider RTK tag
     */
#define UDP_XMIT_PBUF_SIZE  (14 + 4 + 20 + 8)

    /* alloc output pbuf and extend 4B for MSS */
    /*
    if (LWPS_OK != lwps_pbuf_alloc(UDP_XMIT_PBUF_SIZE + length, 0, &pBuf_Xmit)) {
        OSAL_PRINTF("%s():%d - pBuf alloc failed!\n", __FILE__, (uint16)__LINE__);
        return LWPS_ERR_OUTOFMEMORY;
    }
    */

    //OSAL_PRINTF("%s():%d - pBuf_Xmit->data = %p\n", __FILE__, (uint16)__LINE__, pBuf_Xmit->data);
    //OSAL_PRINTF("%s():%d pBuf_Xmit - h: %p d: %p t: %p e: %p\n", __FILE__, (uint16)__LINE__, pBuf_Xmit->head, pBuf_Xmit->data, pBuf_Xmit->tail, pBuf_Xmit->end);

    /* pointer to L4 hdr */
    pBuf_Xmit->l4 = (uint8*)(pkt_head + UDP_XMIT_PBUF_SIZE - sizeof(udp_hdr_t));    /* Max UDP Hdr size */
    pBuf_Xmit->size = 0;

    /* Insert UDP HDR */
    pUdpHdr_Xmit = (udp_hdr_t *)pBuf_Xmit->l4;
    pUdpHdr_Xmit->src_port = udp_pcb_entry[pcbId].local_port;
    //pUdpHdr_Xmit->dst_port = udp_pcb_entry[pcbId].remote_port;
    pUdpHdr_Xmit->dst_port = port;
    pUdpHdr_Xmit->length = sizeof(udp_hdr_t) + length;
    pUdpHdr_Xmit->chksum = 0;
    pBuf_Xmit->size += sizeof(udp_hdr_t);

    pUdpDatagram = pBuf_Xmit->l4 + sizeof(udp_hdr_t);

    /* UDP segment data */
    if (length > 0) {
        memcpy(pUdpDatagram, pData, length);
        pBuf_Xmit->size += length;
    }

    //udp_hdr_dump(pUdpHdr_Xmit);

#if 0
    /* dump packet (data ~ tail) */
    {
        uint32 i;

        OSAL_PRINTF("-- [udp_xmit_to] --------------------------------------------\n");
        for (i=0; i<pBuf_Xmit->length; i++)
        {
            if ((i & 0xf) == 0x0) OSAL_PRINTF("[%03X] ", i);
            OSAL_PRINTF("%02X ", *(pBuf_Xmit->data + i));
            if ((i & 0xf) == 0xf) OSAL_PRINTF("\n");
        }
        OSAL_PRINTF("-------------------------------------------------------------\n");
    }
#endif

    //OSAL_PRINTF("%s():%d - pBuf_Xmit->data = %p, pBuf_Xmit->length = %d\n", __FILE__, (uint16)__LINE__, pBuf_Xmit->data, pBuf_Xmit->length);

    pBuf_Xmit->l4fmt = L4FMT_UDP;
    
    //if (LWPS_OK != ip_output(pBuf_Xmit, &udp_pcb_entry[pcbId].remote_ip)) {
    if (LWPS_OK != ip_down(pBuf_Xmit, &ip)) {
        OSAL_PRINTF("%s():%d - ip_output failed!\n", __FILE__, (uint16)__LINE__);
        //lwps_pbuf_free(pBuf_Xmit);
        return LWPS_ERR;
    }

    /* Increment sent counter on successful transmission */
    udp_datagrams_sent++;
    DBG_LWPS_UDP_PRINTF(("%s():%d Datagram sent, total: %lu\n", __FILE__, (uint16)__LINE__, udp_datagrams_sent));

    return LWPS_OK;
}
#endif

#ifdef LWPS_IP
static int16 udp_up_handle_ip(pktBuf_t *pBuf) large
{
    udp_hdr_t *pUdpHdr = (udp_hdr_t *)pBuf->l4;
    ip_hdr_t *pIpHdr = (ip_hdr_t *)pBuf->l3;
    ip_addr_t src_ip;
    int32 i;

    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

    /* Basic validation: Check if packet size is sufficient for UDP header */
    if (pBuf->size < sizeof(udp_hdr_t))
    {
        udp_datagrams_failed_other++;
        DBG_LWPS_UDP_PRINTF(("%s():%d Invalid UDP packet size, total failed other: %lu\n", 
            __FILE__, (uint16)__LINE__, udp_datagrams_failed_other));
        //lwps_pbuf_free(pBuf);
        return LWPS_ERR;
    }

    /* find the handler */
    for (i=0; i<UDP_MAX_CONNECTION; i++)
    {
        /* make sure it's to us */
        if (udp_pcb_entry[i].local_port == pUdpHdr->dst_port)
        {
            /* [FIXME] Not support connect, disconnect YET */
            if (udp_pcb_entry[i].remote_port == UDP_PORT_ANY) {
                /* Receive Data? */
                if ((pBuf->size - sizeof(udp_hdr_t)) > 0)
                {
                    if (_UdpHandler.event_recvfrom != NULL)
                    {
                        memcpy(src_ip.addr, pIpHdr->src_ip, IP_ADDR_LEN);
                        _UdpHandler.event_recvfrom(i, 
                            pBuf->l4 + sizeof(udp_hdr_t), 
                            pUdpHdr->length - sizeof(udp_hdr_t),
                            src_ip, pUdpHdr->src_port);
                        udp_datagrams_delivered++; /* Increment delivered counter */
                        DBG_LWPS_UDP_PRINTF(("%s():%d Datagram delivered, total: %lu\n", 
                            __FILE__, (uint16)__LINE__, udp_datagrams_delivered));
                    }
                }
            }
            break;
        }
    }

    /* If no matching PCB was found, increment no-app counter */
    if (i == UDP_MAX_CONNECTION)
    {
        udp_datagrams_no_app++;
        DBG_LWPS_UDP_PRINTF(("%s():%d No application for port %d, total undelivered: %lu\n", 
            __FILE__, (uint16)__LINE__, pUdpHdr->dst_port, udp_datagrams_no_app));
    }

    //lwps_pbuf_free(pBuf);

    return LWPS_OK;
}
#endif /* LWPS_IP */

int16 udp_up(pktBuf_t *pBuf) large
{
    udp_hdr_t *pUdpHdr = (udp_hdr_t *)pBuf->l4;

    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));
    //printf("%s():%d\n", __FILE__, (uint16)__LINE__);

    //udp_hdr_dump(pUdpHdr);

    /* Dispatch */
    switch (pBuf->l3fmt)
    {
#ifdef LWPS_IP
    case L3FMT_IP:
        if (LWPS_OK != udp_up_handle_ip(pBuf))
        {
            DBG_LWPS_UDP_PRINTF(("%s():%d - udp_up_handle_ip failed!\n", __FILE__, (uint16)__LINE__));
            udp_datagrams_failed_other++; /* Increment counter for other failures */
            DBG_LWPS_UDP_PRINTF(("%s():%d Failed in udp_up_handle_ip, total failed other: %lu\n", 
                __FILE__, (uint16)__LINE__, udp_datagrams_failed_other));
            return LWPS_ERR;
        }
        break;
#endif /* LWPS_IP */

    default:
        DBG_LWPS_UDP_PRINTF(("%s():%d - l3fmt (%d) is NOT supported!\n", __FILE__, (uint16)__LINE__, pBuf->l3fmt));
        udp_datagrams_failed_other++; /* Increment counter for unsupported L3 format */
        DBG_LWPS_UDP_PRINTF(("%s():%d Unsupported L3 format, total failed other: %lu\n", 
            __FILE__, (uint16)__LINE__, udp_datagrams_failed_other));
        //lwps_pbuf_free(pBuf);
        break;
    }

    return LWPS_OK;
} /* end of udp_up */

int16 udp_bind(uint16 pcbId, ip_addr_t ip, uint16 port) large
{
    int32 i;

    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);

    /* check port number is not used */
    for (i=0; i<UDP_MAX_CONNECTION; i++)
    {
        if ((i != pcbId) && udp_pcb_entry[i].local_port == port)
        {
            /* this port is using */
            return LWPS_ERR;
        }
    }

    udp_pcb_entry[pcbId].local_ip = ip;
    udp_pcb_entry[pcbId].local_port = port;

    return LWPS_OK;
}

#if 0
int16 udp_from_to(uint16 pcbId, ip_addr_t ip, uint16 port) large
{
    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);

    udp_pcb_entry[pcbId].remote_ip = ip;
    udp_pcb_entry[pcbId].remote_port = port;

    return LWPS_OK;
}
#endif

#if (defined(LWS_A_SET) || defined(UPGRADE_TFTP))
int16 udp_sendto(uint16 pcbId, uint8 *pData, uint16 length, ip_addr_t ip, uint16 port) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);
    IF_TRUE_RET(pData == NULL, LWPS_ERR_NULLPOINT);

    if (LWPS_OK != udp_localport_check(pcbId))
    {
        DBG_LWPS_UDP_PRINTF(("%s():%d udp_localport_check failed!\n", __FILE__, (uint16)__LINE__));

        return LWPS_ERR;
    }

    /* transmit packet */
    if (LWPS_OK != udp_xmit_to(pcbId, pData, length, ip, port))
    {
        DBG_LWPS_UDP_PRINTF(("%s():%d udp_xmit failed\n", __FILE__, (uint16)__LINE__));

        return LWPS_ERR;
    }

    return LWPS_OK;
}
#endif

int16 udp_close(uint16 pcbId) large
{
    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);

    /* delete this entry */
    udp_delPCB(pcbId);

    return LWPS_OK;
}
//===============================  Monty Add for UDP snmp oid Start =======================
/* 
 * Function to get the total number of UDP datagrams delivered to users 
 */
uint32 udp_get_datagrams_delivered(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Total UDP datagrams delivered: %lu\n", 
        __FILE__, (uint16)__LINE__, udp_datagrams_delivered));
    return udp_datagrams_delivered;
}

/* 
 * Function to reset the total number of UDP datagrams delivered 
 */
int16 udp_reset_datagrams_delivered(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Resetting UDP datagrams delivered counter\n", 
        __FILE__, (uint16)__LINE__));
    udp_datagrams_delivered = 0;
    return LWPS_OK;
}

/* 
 * Function to get the total number of UDP datagrams received with no application at destination port 
 */
uint32 udp_get_datagrams_no_app(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Total UDP datagrams with no application: %lu\n", 
        __FILE__, (uint16)__LINE__, udp_datagrams_no_app));
    return udp_datagrams_no_app;
}

/* 
 * Function to reset the total number of UDP datagrams received with no application 
 */
int16 udp_reset_datagrams_no_app(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Resetting UDP datagrams no application counter\n", 
        __FILE__, (uint16)__LINE__));
    udp_datagrams_no_app = 0;
    return LWPS_OK;
}

/* 
 * Function to get the total number of UDP datagrams that failed delivery for reasons other than no application 
 */
uint32 udp_get_datagrams_failed_other(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Total UDP datagrams failed for other reasons: %lu\n", 
        __FILE__, (uint16)__LINE__, udp_datagrams_failed_other));
    return udp_datagrams_failed_other;
}

/* 
 * Function to reset the total number of UDP datagrams failed for other reasons 
 */
int16 udp_reset_datagrams_failed_other(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Resetting UDP datagrams failed other counter\n", 
        __FILE__, (uint16)__LINE__));
    udp_datagrams_failed_other = 0;
    return LWPS_OK;
}

/* 
 * Function to get the total number of UDP datagrams sent 
 */
uint32 udp_get_datagrams_sent(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Total UDP datagrams sent: %lu\n", 
        __FILE__, (uint16)__LINE__, udp_datagrams_sent));
    return udp_datagrams_sent;
}

/* 
 * Function to reset the total number of UDP datagrams sent 
 */
int16 udp_reset_datagrams_sent(void) large
{
    DBG_LWPS_UDP_PRINTF(("%s():%d Resetting UDP datagrams sent counter\n", 
        __FILE__, (uint16)__LINE__));
    udp_datagrams_sent = 0;
    return LWPS_OK;
}

//===============================  Monty Add for UDP snmp oid End =======================
#endif /* end of LWPS_UDP */


























//#pragma STRING (far)
///*
//* Copyright Realtek Semiconductor Corporation, 2008
//* All rights reserved.
//*
//* Program  : Light-weight Protocol Stack
//* Abstract : 
//* Author   : 
//* $Id: $
//*/

//#include <local/lwps_config.h>

//#if LWPS_UDP

///*  
// * Include Files 
// */
//#include <lwps/lwps.h>


///*
// * Symbol Definition 
// */
//#define UDP_PORT_ANY    (0)
//#define UDP_PORT_BEGIN  (21024)
//#define UDP_PORT_MAX    (65535)


///*
// * Data Declaration 
// */
//static udp_pcb_t     xdata udp_pcb_entry[UDP_MAX_CONNECTION];
//static udp_handler_t xdata _UdpHandler;
//static ip_addr_t     xdata ip_addr_empty = { { 0, 0, 0, 0 } };


///*
// * Macro Definition
// */
//#define IF_TRUE_RET(exp, ret)           do {    \
//    if (exp) {                                                                          \
//        OSAL_PRINTF("%s():%d (" #exp ") => return " #ret "\n", __FILE__, (uint16)__LINE__); \
//        return ret;                                                                     \
//    }                                                                                   \
//} while (0)
//#define IS_PCBID_INVALID(idx)           (idx < 0 || idx >= UDP_MAX_CONNECTION)
//#define IS_PCB_VALID(idx)               (udp_pcb_entry[idx].valid == TRUE)


///*
// * Function Declaration 
// */
//#if DBG_LWPS_UDP_ENABLE
//static void udp_hdr_dump(udp_hdr_t *pUdpHdr) large
//{
//    //OSAL_PRINTF("%s():%d pUdpHdr = %p\n", __FILE__, (uint16)__LINE__, pUdpHdr);
//    OSAL_PRINTF("-- [UDP HDR dump] -----------------------------\n");
//    OSAL_PRINTF("   Src Port: 0x%04X (%d)\n", pUdpHdr->src_port, pUdpHdr->src_port);
//    OSAL_PRINTF("   Dst Port: 0x%04X (%d)\n", pUdpHdr->dst_port, pUdpHdr->dst_port);
//    OSAL_PRINTF("     Length: 0x%04X (%d)\n", pUdpHdr->length, pUdpHdr->length);
//    OSAL_PRINTF("   Checksum: 0x%04X\n", pUdpHdr->chksum);
//    OSAL_PRINTF("-----------------------------------------------\n");
//}
//#endif /* end of DBG_LWPS_UDP_ENABLE */

//static void udp_delPCB(uint16 idx) large
//{
//    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

//    if (idx >= UDP_MAX_CONNECTION)
//    {
//        DBG_LWPS_UDP_PRINTF(("%s():%d - idx (%lu) error\n", __FILE__, (uint16)__LINE__, idx));
//        return;
//    }

//    udp_pcb_entry[idx].local_ip = ip_addr_empty;
//    udp_pcb_entry[idx].remote_ip = ip_addr_empty;
//    udp_pcb_entry[idx].local_port = 0;
//    udp_pcb_entry[idx].remote_port = 0;
//    udp_pcb_entry[idx].parent_id = idx; /* self */
//}

//void udp_timer(void) large
//{
//    return;
//}

//int16 udp_getPCB(uint16 idx, udp_pcb_t **ppUdpPCB) large
//{
//    udp_pcb_t *pUdpPCB = (udp_pcb_t *)(*((uint16*)ppUdpPCB));
//    IF_TRUE_RET(IS_PCBID_INVALID(idx), LWPS_ERR);
//    IF_TRUE_RET(ppUdpPCB == NULL, LWPS_ERR_NULLPOINT);

//    /* Assign pointer */
//    pUdpPCB = &udp_pcb_entry[idx];

//    return LWPS_OK;
//}

//int16 udp_setHandler(udp_handler_t *pUdpHandler) large
//{
//    IF_TRUE_RET(pUdpHandler == NULL, LWPS_ERR_NULLPOINT);

//    _UdpHandler.event_connect = pUdpHandler->event_connect;
//    _UdpHandler.event_disconnect = pUdpHandler->event_disconnect;
//    _UdpHandler.event_recvfrom = pUdpHandler->event_recvfrom;

//    return LWPS_OK;
//}

//int16 udp_init(void) large
//{
//    int32 i;

//    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

//    /* Clear handler */
//    _UdpHandler.event_connect = NULL;
//    _UdpHandler.event_disconnect = NULL;
//    _UdpHandler.event_recvfrom = NULL;

//    /* Reset PCB */
//    for (i=0; i<UDP_MAX_CONNECTION; i++) {
//        udp_delPCB(i);
//    }

//    return LWPS_OK;
//}
//#if (defined( LWS_A_SET) || defined( UPGRADE_TFTP))
//static int16 udp_localport_check(uint16 pcbId) large
//{
//    uint16 lport;
//    int32 i;

//    /* find a valid port if local-port is invalid */
//    if (udp_pcb_entry[pcbId].local_port == UDP_PORT_ANY)
//    {
//        for (lport=UDP_PORT_BEGIN; lport<UDP_PORT_MAX; lport++)
//        {
//            for (i=0; i<UDP_MAX_CONNECTION; i++)
//            {
//                if ((i != pcbId) && (udp_pcb_entry[i].local_port == lport))
//                {
//                    break;
//                }
//            }

//            /* is valid port? */
//            if (udp_pcb_entry[i].local_port != lport)
//            {
//                udp_pcb_entry[pcbId].local_port = lport;
//                break;
//            }
//        }

//        /* not found vaild-port */
//        if (udp_pcb_entry[pcbId].local_port == UDP_PORT_ANY)
//        {
//            OSAL_PRINTF("%s():%d Cannot find a vaild port.\n", __FILE__, (uint16)__LINE__);

//            return LWPS_ERR;
//        }
//    }

//    return LWPS_OK;
//}

//static int16 udp_xmit_to(uint16 pcbId, uint8 *pData, uint16 length, ip_addr_t ip, uint16 port) large
//{
//    pktBuf_t *pBuf_Xmit = &pktbuf;
//    udp_hdr_t *pUdpHdr_Xmit;
//    uint8 *pUdpDatagram;

//    DBG_LWPS_UDP_PRINTF(("%s():%d pcbId = %d, pData = %p, length = %d\n", \
//        __FILE__, (uint16)__LINE__, pcbId, pData, length));

//    /* [WARN] DO NOT check pcbId, because it's vaild likely */
//#if 0
//    if ((pData == NULL) && (length == 0)) {
//        return LWPS_ERR_NULLPOINT;
//    }
//#endif

//    /* [FIXME] fix length of pbuf note
//     * <IP alignment> ETH (6+6+2) + [RTK (8)] + VLAN (4) + IP (20) + UDP (8)
//     * DON'T have to consider RTK tag
//     * 
//     */
//#define UDP_XMIT_PBUF_SIZE  (14 + 4 + 20 + 8)

//	
//    /* alloc output pbuf and extend 4B for MSS */
//    /*
//    if (LWPS_OK != lwps_pbuf_alloc(UDP_XMIT_PBUF_SIZE + length, 0, &pBuf_Xmit)) {
//        OSAL_PRINTF("%s():%d - pBuf alloc failed!\n", __FILE__, (uint16)__LINE__);
//        return LWPS_ERR_OUTOFMEMORY;
//    }
//    */

//    //OSAL_PRINTF("%s():%d - pBuf_Xmit->data = %p\n", __FILE__, (uint16)__LINE__, pBuf_Xmit->data);
//    //OSAL_PRINTF("%s():%d pBuf_Xmit - h: %p d: %p t: %p e: %p\n", __FILE__, (uint16)__LINE__, pBuf_Xmit->head, pBuf_Xmit->data, pBuf_Xmit->tail, pBuf_Xmit->end);

//    /* pointer to L4 hdr */
//    pBuf_Xmit->l4 = (uint8*)(pkt_head + UDP_XMIT_PBUF_SIZE - sizeof(udp_hdr_t));    /* Max UDP Hdr size */
//    pBuf_Xmit->size = 0;

//    /* Insert UDP HDR */
//    pUdpHdr_Xmit = (udp_hdr_t *)pBuf_Xmit->l4;
//    pUdpHdr_Xmit->src_port = udp_pcb_entry[pcbId].local_port;
//    //pUdpHdr_Xmit->dst_port = udp_pcb_entry[pcbId].remote_port;
//    pUdpHdr_Xmit->dst_port = port;
//    pUdpHdr_Xmit->length = sizeof(udp_hdr_t) + length;
//    pUdpHdr_Xmit->chksum = 0;
//    pBuf_Xmit->size += sizeof(udp_hdr_t);

//    pUdpDatagram = pBuf_Xmit->l4 + sizeof(udp_hdr_t);

//    /* UDP segment data */
//    if (length > 0) {
//        memcpy(pUdpDatagram, pData, length);
//        pBuf_Xmit->size += length;
//    }

//    //udp_hdr_dump(pUdpHdr_Xmit);

//#if 0
//    /* dump packet (data ~ tail) */
//    {
//        uint32 i;

//        OSAL_PRINTF("-- [udp_xmit_to] --------------------------------------------\n");
//        for (i=0; i<pBuf_Xmit->length; i++)
//        {
//            if ((i & 0xf) == 0x0) OSAL_PRINTF("[%03X] ", i);
//            OSAL_PRINTF("%02X ", *(pBuf_Xmit->data + i));
//            if ((i & 0xf) == 0xf) OSAL_PRINTF("\n");
//        }
//        OSAL_PRINTF("-------------------------------------------------------------\n");
//    }
//#endif

//    //OSAL_PRINTF("%s():%d - pBuf_Xmit->data = %p, pBuf_Xmit->length = %d\n", __FILE__, (uint16)__LINE__, pBuf_Xmit->data, pBuf_Xmit->length);

//    pBuf_Xmit->l4fmt = L4FMT_UDP;
//    
//    //if (LWPS_OK != ip_output(pBuf_Xmit, &udp_pcb_entry[pcbId].remote_ip)) {
//    if (LWPS_OK != ip_down(pBuf_Xmit, &ip)) {
//        OSAL_PRINTF("%s():%d - ip_output failed!\n", __FILE__, (uint16)__LINE__);
//        //lwps_pbuf_free(pBuf_Xmit);
//    }

//    return LWPS_OK;
//}
//#endif
//#ifdef  LWPS_IP
//static int16 udp_up_handle_ip(pktBuf_t *pBuf) large
//{
//    udp_hdr_t *pUdpHdr = (udp_hdr_t *)pBuf->l4;
//    ip_hdr_t *pIpHdr = (ip_hdr_t *)pBuf->l3;
//    ip_addr_t src_ip;
//    int32 i;

//    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

//    /* find the handler */
//    for (i=0; i<UDP_MAX_CONNECTION; i++)
//    {
//        /* make sure it's to us */
//        if (udp_pcb_entry[i].local_port == pUdpHdr->dst_port)
//        {
//            /* [FIXME] Not support connect, disconnect YET */
//            if (udp_pcb_entry[i].remote_port == UDP_PORT_ANY) {
//                /* Receive Data? */
//                if ((pBuf->size - sizeof(udp_hdr_t)) > 0)
//                {
//                    if (_UdpHandler.event_recvfrom != NULL)
//                    {
//                        memcpy(src_ip.addr, pIpHdr->src_ip, IP_ADDR_LEN);
//                        _UdpHandler.event_recvfrom(i, 
//                            pBuf->l4 + sizeof(udp_hdr_t), 
//                            pUdpHdr->length - sizeof(udp_hdr_t),
//                            src_ip, pUdpHdr->src_port);
//                    }
//                }
//            }
//            break;
//        }
//    }

//    //lwps_pbuf_free(pBuf);

//    return LWPS_OK;
//}
//#endif  /* LWPS_IP */

//int16 udp_up(pktBuf_t *pBuf) large
//{
//    udp_hdr_t *pUdpHdr = (udp_hdr_t *)pBuf->l4;

//    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));
//    //printf("%s():%d\n", __FILE__, (uint16)__LINE__);


//    //udp_hdr_dump(pUdpHdr);

//    /* Dispatch */
//    switch (pBuf->l3fmt)
//    {
//#ifdef  LWPS_IP
//    case L3FMT_IP:
//        if (LWPS_OK != udp_up_handle_ip(pBuf))
//        {
//            DBG_LWPS_UDP_PRINTF(("%s():%d - udp_up_handle_ip failed!\n", __FILE__, (uint16)__LINE__));
//            return LWPS_ERR;
//        }
//        break;
//#endif  /* LWPS_IP */

//    default:
//        DBG_LWPS_UDP_PRINTF(("%s():%d - l3fmt (%d) is NOT supported!\n", __FILE__, (uint16)__LINE__, pBuf->l3fmt));
//        //lwps_pbuf_free(pBuf);
//        break;
//    }

//    return LWPS_OK;
//} /* end of udp_up */


//int16 udp_bind(uint16 pcbId, ip_addr_t ip, uint16 port) large
//{
//    int32 i;

//    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);

//    /* check port number is not used */
//    for (i=0; i<UDP_MAX_CONNECTION; i++)
//    {
//        if ((i != pcbId) && udp_pcb_entry[i].local_port == port)
//        {
//            /* this port is using */
//            return LWPS_ERR;
//        }
//    }

//    udp_pcb_entry[pcbId].local_ip = ip;
//    udp_pcb_entry[pcbId].local_port = port;

//    return LWPS_OK;
//}

//#if 0
//int16 udp_from_to(uint16 pcbId, ip_addr_t ip, uint16 port) large
//{
//    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);

//    udp_pcb_entry[pcbId].remote_ip = ip;
//    udp_pcb_entry[pcbId].remote_port = port;

//    return LWPS_OK;
//}
//#endif
//#if (defined( LWS_A_SET) || defined( UPGRADE_TFTP))
//int16 udp_sendto(uint16 pcbId, uint8 *pData, uint16 length, ip_addr_t ip, uint16 port) large
//{
//    DBG_LWPS_UDP_PRINTF(("%s():%d\n", __FILE__, (uint16)__LINE__));

//    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);
//    IF_TRUE_RET(pData == NULL, LWPS_ERR_NULLPOINT);

//    if (LWPS_OK != udp_localport_check(pcbId))
//    {
//        DBG_LWPS_UDP_PRINTF(("%s():%d udp_localport_check failed!\n", __FILE__, (uint16)__LINE__));

//        return LWPS_ERR;
//    }

//    /* transmit packet */
//    if (LWPS_OK != udp_xmit_to(pcbId, pData, length, ip, port))
//    {
//        DBG_LWPS_UDP_PRINTF(("%s():%d udp_xmit failed\n", __FILE__, (uint16)__LINE__));

//        return LWPS_ERR;
//    }

//    return LWPS_OK;
//}
//#endif
//int16 udp_close(uint16 pcbId) large
//{
//    IF_TRUE_RET(IS_PCBID_INVALID(pcbId), LWPS_ERR);

//    /* delete this entry */
//    udp_delPCB(pcbId);

//    return LWPS_OK;
//}

//#endif /* end of LWPS_UDP */

