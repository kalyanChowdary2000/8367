/*
 * Copyright Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Program  : ICMP (Internet Control Message Protocol) of Light-weight Protocol Stack
 * Abstract : 
 * Author   : 
 * $Id: $
 */


#ifndef	__LWPS_ICMP_H__
#define	__LWPS_ICMP_H__

#include "local/config.h"

#if LWPS_ICMP

/*
 * Include Files
 */
#include <lwps/lwps.h>

/*
 * Symbol Definition
 */
typedef	struct icmp_hdr_s {
    uint8  type;       /* Type */
    uint8  cod;
//    uint8  code;       /* reserved keyword in 8051 C language */
    uint16 chksum;     /* Checksum */

    uint16 identifier; /* Id Number */
    uint16 seq_number; /* Sequence */
} icmp_hdr_t;

typedef enum icmp_type_e {
    ICMP_TYPE_ECHOREPLY = 0,            /* Echo Reply                   */
    ICMP_TYPE_DESTUNREACH = 3,          /* Destination Unreachable      */
    ICMP_TYPE_SOURCEQUENCH = 4,         /* Source Quench                */
    ICMP_TYPE_REDIRECT = 5,             /* Redirect (change route)      */
    ICMP_TYPE_ECHO = 8,                 /* Echo Request                 */
    ICMP_TYPE_TIMEEXCEEDED = 11,        /* Time Exceeded                */
    ICMP_TYPE_PARAMETERPROB = 12,       /* Parameter Problem            */
    ICMP_TYPE_TIMESTAMP = 13,           /* Timestamp Request            */
    ICMP_TYPE_TIMESTAMPREPLY = 14,      /* Timestamp Reply              */
    ICMP_TYPE_INFOREQUEST = 15,         /* Information Request          */
    ICMP_TYPE_INFOREPLY = 16,           /* Information Reply            */
    ICMP_TYPE_ADDRESS = 17,             /* Address Mask Request         */
    ICMP_TYPE_ADDRESSREPLY = 18         /* Address Mask Reply           */
} icmp_type_t;

/*
 * Macro Definition
 */

#define DBG_LWPS_ICMP_ENABLE    (0)
#if DBG_LWPS_ICMP_ENABLE
#define DBG_LWPS_ICMP_PRINTF   OSAL_PRINTF
#else  /* else of DBG_LWPS_ICMP_ENABLE */
#define DBG_LWPS_ICMP_PRINTF    
#endif /* end of else of DBG_LWPS_ICMP_ENABLE */

/*
 * Function Declaration
 */
extern int32 icmp_init(void) large;
extern int32 icmp_up(pktBuf_t *pBuf) large;

/*
 * ICMP Echo (ping) client helpers used by the web "IP Diagnostic" tool.
 * The web layer owns the multi-request ping session (how many requests,
 * how long to wait, etc). These helpers only build/send a single Echo
 * Request and let the caller ask whether a matching Echo Reply has been
 * seen. This mirrors how etharp_request()/etharp_entry_find() are used
 * for the ARP based reachability check elsewhere in the web code.
 *
 * Gated separately by LWPS_ICMP_PING_CLIENT (not just LWPS_ICMP) because
 * this same icmp.c is also linked into the tiny bootloader project,
 * which is extremely tight on code space and has no use for a ping
 * client. The loader's lwps_config.h leaves this at 0.
 */
#ifndef LWPS_ICMP_PING_CLIENT
#define LWPS_ICMP_PING_CLIENT (1)
#endif

#if LWPS_ICMP_PING_CLIENT
extern int32 icmp_echo_send(ip_addr_t dst, uint16 ident, uint16 seq) large;
extern uint8 icmp_echo_reply_check(uint16 ident, uint16 seq) large;
extern void  icmp_echo_reply_clear(void) large;
#endif /* LWPS_ICMP_PING_CLIENT */

#else  /* else of LWPS_ICMP */
#define icmp_init()

#endif /* end of else of LWPS_ICMP */

#endif	/* __LWPS_ICMP_H__ */
