#include <common.h>
#include <sal/salflash.h>
#include <sal/salsys.h>
#include <sal/saltype.h>
//===========================
#include <flash_config.h>
#include <salflash.h>
#include <salutil.h>
#include <snmp/snmp_msg.h>
#include <lwps/lwps_api.h>

// typedef s8_t err_t;

extern u16_t vlanIds;
extern sal_ipaddr_t netmask = 0;
extern int add_gateway_over_vlan_acl_rule(uint16 rule_num, uint16 vlan_id, uint32 gateway_ip, uint32 subnetmask);
// extern int add_ip_over_vlan_acl_rule(uint16 rule_num, uint16 vlan_id, uint32 ip_address,uint32 subnetmask);
// extern int32 sal_flash_vlan_id_set(uint32 addr , uint16 length, uint8 *pData);
extern int32 sal_flash_vlan_id_get(uint32 addr, uint16 length, uint8 *pData);
extern err_t snmp_send_trap(s8_t generic_trap, struct snmp_obj_id *eoid, s32_t specific_trap);
extern volatile uint32 data current_time;
extern uint8 xdata this_ip[];
extern char m_u8_uart_data[64];

#define POST_TCP_ENABLE 0
#define POST_IP0 192
#define POST_IP1 168
#define POST_IP2 1
#define POST_IP3 111
#define POST_PORT 4362

#if POST_TCP_ENABLE
static int32 post_sid = -1;
static uint8 post_busy = 0;
static uint32 post_t0 = 0;
static char xdata post_buf[128];

static void post_on_connect(int32 parentSocketId, int32 socketId, ip_addr_t ip, uint16 port) reentrant;
static void post_on_disconnect(int32 socketId) reentrant;
static void post_on_recv(int32 socketId, uint8 *pData, uint32 length) reentrant;
static void post_send_kick(void) large;
static uint8 u16_to_dec(char *b, uint8 p, uint16 v) large;

static lwps_handler_t xdata post_h = { post_on_connect, post_on_disconnect, post_on_recv, NULL };

static uint8 u16_to_dec(char *b, uint8 p, uint16 v) large
{
	char t[5];
	uint8 i = 0;
	if (v == 0) { b[p++] = '0'; return p; }
	while (v > 0 && i < 5) { t[i++] = (char)('0' + (v % 10)); v /= 10; }
	while (i) { b[p++] = t[--i]; }
	return p;
}

static void post_on_connect(int32 parentSocketId, int32 socketId, ip_addr_t ip, uint16 port) reentrant
{
	(void)parentSocketId; (void)port;
	(void)ip.addr[0]; (void)ip.addr[1]; (void)ip.addr[2]; (void)ip.addr[3];
	if (socketId != post_sid) return;
	lwps_send(socketId, (uint8 *)post_buf, (int32)strlen(post_buf));
	lwps_close(socketId);
}

static void post_on_disconnect(int32 socketId) reentrant
{
	(void)socketId;
	post_sid = -1;
	post_busy = 0;
}

static void post_on_recv(int32 socketId, uint8 *pData, uint32 length) reentrant
{
	(void)pData; (void)length;
	lwps_close(socketId);
}
#endif

/* Trigger hook: runs every 5 seconds from main loop. */
static void post_client_periodic_trigger(void)
{
#if POST_TCP_ENABLE
	post_send_kick();
#endif
}

#if POST_TCP_ENABLE
static void post_send_kick(void) large
{
	ip_addr_t lip, rip;
	uint16 lport;
	uint8 p = 0;
	uint16 blen;
	static const char code h1[] = "POST / HTTP/1.0\r\nContent-Length: ";
	static const char code h2[] = "\r\n\r\n";

	if (post_busy) return;
	blen = (uint16)strlen(m_u8_uart_data);
	if (blen < 3 || blen > 63) return;

	memcpy(post_buf + p, h1, sizeof(h1) - 1); p += (uint8)(sizeof(h1) - 1);
	p = u16_to_dec(post_buf, p, blen);
	memcpy(post_buf + p, h2, sizeof(h2) - 1); p += (uint8)(sizeof(h2) - 1);
	memcpy(post_buf + p, m_u8_uart_data, blen); p += (uint8)blen;
	post_buf[p] = '\0';

	post_sid = lwps_socket(LWPS_L3TYPE_IP, LWPS_L4TYPE_TCP, post_h);
	if (post_sid < 0) { post_sid = -1; return; }
	post_busy = 1; post_t0 = current_time;

	lip.addr[0] = this_ip[0]; lip.addr[1] = this_ip[1]; lip.addr[2] = this_ip[2]; lip.addr[3] = this_ip[3];
	lport = (uint16)(0xC000U + ((uint16)post_sid & 0x00FFU));
	if (lwps_bind(post_sid, lip, lport) != LWPS_OK) { lwps_close(post_sid); post_sid = -1; post_busy = 0; return; }

	rip.addr[0] = POST_IP0; rip.addr[1] = POST_IP1; rip.addr[2] = POST_IP2; rip.addr[3] = POST_IP3;
	(void)lwps_arp_request(rip);
	if (lwps_connect(post_sid, rip, POST_PORT) != LWPS_OK) { lwps_close(post_sid); post_sid = -1; post_busy = 0; return; }
}
#endif

void main()
{
#if defined(PARALLEL_MODE)
	uint16 tmpreg, ledValue;
#endif

	//=====================ACL Variables==========================
	uint8 getvlan_id[5];
	sal_ipaddr_t gateway = 0;
	// u8_t    ip = 0;
	// sal_ipaddr_t    netmask = 0;
	//=====================ACL Variables==========================
	
	sal_mac_t mac;
	
	setReg(0x1d32, 0x0004); // Enable 67C UART
	setReg(0x1d3f, 0x0100); // Force deglitch enable

	/* system initialization */
	all_init();

	/* Check MAC prefix and set if needed */
	sal_sys_mac_get(&mac);
	if (!(mac.octet[0] == 0xA4 && mac.octet[1] == 0xDA && mac.octet[2] == 0x22 && mac.octet[3] == 0x52)) {
		/* Set to all zeros for identification */
		mac.octet[0] = 0xA4;
		mac.octet[1] = 0xDA;
		mac.octet[2] = 0x22;
		mac.octet[3] = 0x52;
		mac.octet[4] = 0x00;
		mac.octet[5] = 0x00;
		sal_sys_mac_set(&mac);
		sal_flash_sysMac_set(&mac);
	}

	//====================================START ACL =======================
	if (sal_flash_vlan_id_get(FLSH_CONFIG_DUMMY_START, 4, getvlan_id) == 0) {
		getvlan_id[4] = '\0'; // Null-terminate
	}
	
	sal_sys_gateway_get(&gateway);
	vlanIds = (u16_t)atoi((char *)getvlan_id);

	if (strncmp(getvlan_id, "NEW ", 4) != 0) {
		add_gateway_over_vlan_acl_rule(7, vlanIds, gateway, netmask);
	}
	//====================================END ACL =======================

	{
		uint32 last_post_trigger = current_time;
	while (1) {
		/* don't remove this two function calls */
		event_dispatch();

		if ((current_time - last_post_trigger) >= 5) {
			last_post_trigger = current_time;
			post_client_periodic_trigger();
		}
#if POST_TCP_ENABLE
		if (post_busy && ((current_time - post_t0) >= 8)) {
			lwps_close(post_sid);
			post_sid = -1;
			post_busy = 0;
		}
#endif
	};
	}
}