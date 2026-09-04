#pragma STRING(far)
/*
* Copyright Realtek Semiconductor Corporation, 2008
* All rights reserved.
*
* Program  : ICMP (Internet Control Message Protocol) of Light-weight Protocol Stack
* Abstract : 
* Author   : 
* $Id: $
*/

#include <local/lwps_config.h>

#if LWPS_ICMP

/*
 * Include Files
 */

#include <lwps/lwps.h>
#include <stdio.h>
#include <string.h>

#include <local/sal_config.h>

/*
 * Symbol Definition
 */



/*
 * Function Declaration
 */
#if DBG_LWPS_ICMP_ENABLE
static void icmp_hdr_dump(icmp_hdr_t *pIcmpHdr) large
{
    DBG_LWPS_ICMP_PRINTF("-- [ICMP HDR dump] ----------------------------\n");
    DBG_LWPS_ICMP_PRINTF("       Type: 0x%02X (%d)\n", (uint16)pIcmpHdr->type, (uint16)pIcmpHdr->type);
    DBG_LWPS_ICMP_PRINTF("       Code: 0x%02X (%d)\n", (uint16)pIcmpHdr->cod, (uint16)pIcmpHdr->cod);
    DBG_LWPS_ICMP_PRINTF("   Checksum: 0x%04X\n", (uint16)pIcmpHdr->chksum);
    DBG_LWPS_ICMP_PRINTF(" Identifier: 0x%04X\n", (uint16)pIcmpHdr->identifier);
    DBG_LWPS_ICMP_PRINTF(" Seq-Number: 0x%04X (%d)\n", (uint16)pIcmpHdr->seq_number, (uint16)pIcmpHdr->seq_number);
    DBG_LWPS_ICMP_PRINTF("-----------------------------------------------\n");
}
#endif /* end of DBG_LWPS_ICMP_ENABLE */

int32 icmp_init(void) large
{
    DBG_LWPS_ICMP_PRINTF("%s():%d\n", __FILE__, (uint16)__LINE__);

    return LWPS_OK;
}

static int32 icmp_up_handle_echo_request(pktBuf_t xdata *pBuf) large
{
    ip_hdr_t   xdata *pIpHdr         = (ip_hdr_t *)pBuf->l3;  /* IP only */
    icmp_hdr_t xdata *pIcmpHdr       = (icmp_hdr_t *)pBuf->l4;
    ip_addr_t  xdata DstIpAddr;
    pktBuf_t   xdata *pBuf_Reply     = pBuf;
    icmp_hdr_t xdata *pIcmpHdr_Reply = (icmp_hdr_t *)pBuf->l4;

    DBG_LWPS_ICMP_PRINTF("%s():%d\n", __FILE__, (uint16)__LINE__);

    /* ICMP HDR setting */
    pIcmpHdr_Reply->type = ICMP_TYPE_ECHOREPLY;
    pIcmpHdr_Reply->cod = 0;
    pIcmpHdr_Reply->chksum = 0;
    pIcmpHdr_Reply->identifier = pIcmpHdr->identifier;
    pIcmpHdr_Reply->seq_number = pIcmpHdr->seq_number;

    //icmp_hdr_dump(pIcmpHdr_Reply);

    /* IP information */
    IPADDR_COPY( (DstIpAddr.addr), pIpHdr->src_ip);
    /* Reply the padding */
           
    //pBuf_Reply->length += (pBuf->length - sizeof(icmp_hdr_t));


    pBuf_Reply->l4fmt = L4FMT_ICMP;
	pBuf_Reply->size -= sizeof(eth_hdr_t);//pre-subtract

    if (LWPS_OK != ip_down(pBuf_Reply, &DstIpAddr)) {
        OSAL_PRINTF("%s():%d - ip_output failed!\n", __FILE__, (uint16)__LINE__);
    }
    
    //icmp_hdr_dump(pIcmpHdr_Reply);

    return LWPS_OK;
}

/*
 * Ping (Echo) client state
 * ------------------------
 * Only compiled in for builds that define LWPS_ICMP_PING_CLIENT=1 (the
 * main firmware). The loader also links this file but is extremely
 * tight on code space and doesn't need a ping client, so none of this
 * is compiled in there (see icmp.h).
 */
#ifndef LWPS_ICMP_PING_CLIENT
#define LWPS_ICMP_PING_CLIENT (0)
#endif

#if LWPS_ICMP_PING_CLIENT
static uint8  xdata icmp_echo_reply_got   = 0;
static uint16 xdata icmp_echo_reply_ident = 0;
static uint16 xdata icmp_echo_reply_seq   = 0;
#endif

static int32 icmp_up_handle_echo_reply(pktBuf_t *pBuf) large
{
#if LWPS_ICMP_PING_CLIENT
    icmp_hdr_t xdata *pIcmpHdr = (icmp_hdr_t *)pBuf->l4;

    DBG_LWPS_ICMP_PRINTF("%s():%d - ECHO reply, id=0x%04X seq=%d\n", \
        __FILE__, (uint16)__LINE__, (uint16)pIcmpHdr->identifier, (uint16)pIcmpHdr->seq_number);

    icmp_echo_reply_got   = 1;
    icmp_echo_reply_ident = pIcmpHdr->identifier;
    icmp_echo_reply_seq   = pIcmpHdr->seq_number;
#else
    /* No ping client in this build; nothing to do with an Echo Reply. */
    LWIP_UNUSED_ARG(pBuf);
#endif

    return LWPS_OK;
}


int32 icmp_up(pktBuf_t *pBuf) large
{
    icmp_hdr_t xdata *pIcmpHdr = (icmp_hdr_t *)pBuf->l4;

    DBG_LWPS_ICMP_PRINTF("%s():%d\n", __FILE__, (uint16)__LINE__);

    //icmp_hdr_dump(pIcmpHdr);

    switch (pIcmpHdr->type) {
    case ICMP_TYPE_ECHO: 
        return icmp_up_handle_echo_request(pBuf);
        break;

    case ICMP_TYPE_ECHOREPLY:
        return icmp_up_handle_echo_reply(pBuf);
        break;

    default:
        DBG_LWPS_ICMP_PRINTF("icmp_up() - Not support icmp type %d \n", pIcmpHdr->type);
        //lwps_pbuf_free(pBuf);
        break;
    }

	
    return LWPS_OK;
}

#if LWPS_ICMP_PING_CLIENT
/*
 * icmp_echo_send() - build and transmit a single ICMP Echo Request.
 * Pattern mirrors udp_xmit_to() exactly. ARP MUST already exist or
 * eth_down() silently drops the packet (see eth.c [FIXME]).
 */
#define ICMP_ECHO_PAYLOAD_LEN   (32)
/* ETH(14) + VLAN(4) + IP(20)  -- same as UDP_XMIT_PBUF_SIZE without L4 hdr */
#define ICMP_L4_OFFSET          (14 + 4 + 20)

/* Simple Internet checksum (software). Some NICs only offload IP, not ICMP. */
static uint16 icmp_inet_chksum(uint8 xdata *tdata, uint16 len) large
{
    uint32 xdata sum = 0;
    uint16 xdata i;

    for (i = 0; i + 1 < len; i += 2)
    {
        sum += ((uint16)tdata[i] << 8) | (uint16)tdata[i + 1];
    }
    if (i < len)
    {
        sum += (uint16)tdata[i] << 8;
    }
    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (uint16)(~sum);
}

int32 icmp_echo_send(ip_addr_t dst, uint16 ident, uint16 seq) large
{
    pktBuf_t   xdata *pBuf_Xmit = &pktbuf;
    icmp_hdr_t xdata *pIcmpHdr_Xmit;
    eth_addr_t xdata mac;
    uint8      xdata *pPayload;
    uint8      xdata i;
    uint16     xdata icmp_len;

    /* eth_down drops IP frames with no ARP entry -- fail early */
    if (LWPS_OK != etharp_entry_find(&dst, &mac))
    {
        return LWPS_ERR;
    }

    /* L4 pointer: leave room for ETH+VLAN+IP (identical to UDP path) */
    pBuf_Xmit->l4 = (uint8 *)(pkt_head + ICMP_L4_OFFSET);
    pBuf_Xmit->size = 0;
    pBuf_Xmit->l2fmt = L2FMT_NONE;
    pBuf_Xmit->l3fmt = L3FMT_NONE;
    pBuf_Xmit->l4fmt = L4FMT_ICMP;

    pIcmpHdr_Xmit = (icmp_hdr_t *)pBuf_Xmit->l4;
    pIcmpHdr_Xmit->type       = ICMP_TYPE_ECHO;
    pIcmpHdr_Xmit->cod        = 0;
    pIcmpHdr_Xmit->chksum     = 0;
    pIcmpHdr_Xmit->identifier = ident;
    pIcmpHdr_Xmit->seq_number = seq;
    pBuf_Xmit->size = sizeof(icmp_hdr_t);

    pPayload = pBuf_Xmit->l4 + sizeof(icmp_hdr_t);
    for (i = 0; i < ICMP_ECHO_PAYLOAD_LEN; i++)
    {
        pPayload[i] = (uint8)('a' + (i % 23));
    }
    pBuf_Xmit->size += ICMP_ECHO_PAYLOAD_LEN;

    /* Software ICMP checksum over header + payload */
    icmp_len = pBuf_Xmit->size;
    pIcmpHdr_Xmit->chksum = 0;
    pIcmpHdr_Xmit->chksum = icmp_inet_chksum(pBuf_Xmit->l4, icmp_len);

    if (LWPS_OK != ip_down(pBuf_Xmit, &dst))
    {
        return LWPS_ERR;
    }

    return LWPS_OK;
}

uint8 icmp_echo_reply_check(uint16 ident, uint16 seq) large
{
    if (icmp_echo_reply_got &&
        (icmp_echo_reply_ident == ident) &&
        (icmp_echo_reply_seq == seq))
    {
        return 1;
    }
    return 0;
}

void icmp_echo_reply_clear(void) large
{
    icmp_echo_reply_got = 0;
}
#endif /* LWPS_ICMP_PING_CLIENT */



#endif /* end of LWPS_ICMP */

