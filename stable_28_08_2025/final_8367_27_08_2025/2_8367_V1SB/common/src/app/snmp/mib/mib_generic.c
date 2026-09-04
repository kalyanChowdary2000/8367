#include "local/config.h"


#if MIB_ENABLE

#include "snmp/snmp.h"
#include "snmp/mib/mib.h"

#if !defined(SAL_REPLACE)
#include <local/rtk_sal_mib.h>
#else
#include <snmp/mib_glue.h>
#endif



void uncpy(u32_t * dst, u32_t * src, u8_t n)
{
    while (n > 0)
    {
        n--;
        *dst++ = *src++;
    }
}

/**
 * Copy octet string.
 *
 * @param dst points to destination
 * @param src points to source
 * @param n number of octets to copy.
 */
void ocstrncpy(u8_t *dst, u8_t *src, u8_t n)
{
  while (n > 0)
  {
    n--;
    *dst++ = *src++;
  }
}

/**
 * Copy object identifier (s32_t) array.
 *
 * @param dst points to destination
 * @param src points to source
 * @param n number of sub identifiers to copy.
 */
void objectidncpy(s32_t *dst, s32_t *src, u8_t n)
{
  while(n > 0)
  {
    n--;
    *dst++ = *src++;
  }
}

void mib_init(void)
{
	uint16 xdata i;

	/* Per Port Init */
	for(i=0; i<INTERFACE_NUMBER; i++)
	{
#if LWPS_SNMP_IFTABLE
		snmp_inc_iflist();    /*do not support mib2-interface group*/
#endif
        
#if LWPS_SNMP_ETHERNET_LIKE
		snmp_inc_dot3StatsTable();
		//snmp_insert_dot3CollTable_tree(i+1, 1);
		snmp_inc_dot3ControlTable();
		snmp_inc_dot3PauseTable();
		//snmp_inc_dot3HCStatsTable(); /*not support 64bit*/
#endif /* end of LWPS_SNMP_ETHERNET_LIKE */

#if LWPS_SNMP_INTERFACE_GROUP
        snmp_inc_ifXTable();
        ifPromiscuousMode[i] = 2;
		ifAliasLen[i] = 0;
#endif /* end of LWPS_SNMP_INTERFACE_GROUP */

#if LWPS_SNMP_BRIDGE_MIB
    snmp_inc_bridgeXTable();
#endif

	}
	/* Global Init */
#if LWPS_SNMP_RMON
	mib2_rmon_init();
#endif /* end of LWPS_SNMP_RMON */
}

#endif /* end of MIB_ENABLE */


