#include <common_types.h>
#include <lwps/pktBuf.h>
#include <salflash.h>

/* called every 5ms */
void handler_timer()
{
}

/* called when link down port is detected */
#if 0//NOT used
void handler_linkdown(uint16 pmsk)
{
    /* kill warning message */
	pmsk = 0;
}

/* called when link up port is detected */
void handler_linkup(uint16 pmsk)
{
    /* kill warning message */
	pmsk = 0;
}
#endif

void handler_net_l2(pktBuf_t *pBuf)
{
    /* kill warning message */
    *(uint8*)pBuf = 0;
}
/* called at system initilization time */
void handler_init()
{
#if 0
    sal_mac_t mac;

    /* example: write MAC address to non-volatile area in flash */
    mac.octet[0] = 0x0;
    mac.octet[1] = 0x01;
    mac.octet[2] = 0x02;
    mac.octet[3] = 0x03;
    mac.octet[4] = 0x04;
    mac.octet[5] = 0x05;
    sal_flash_sysMac_set(&mac);
#endif    
}

