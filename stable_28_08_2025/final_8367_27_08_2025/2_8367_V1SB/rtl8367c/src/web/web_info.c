#pragma STRING (far)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <local/web_config.h>


#include <salsys.h>
#include <saltrunk.h>
#include <salerror.h>
#include <salutil.h>


#include <web/web_api.h>
#include <web/http_debug.h>
#include <lwps/lwps_api.h>
#include <lwps/etharp.h>
#include <lwps/icmp.h>
#include "web_info.h"
#include "web_util.h"

int32 get_info(struct http_request_s *pReq)
{
    sal_mac_t       mac;
    uint8           macStr[18];
    uint8           firmVer[SAL_SYS_STRING_LEN];
    uint8           firmDate[FIRMWARE_DATE_LEN];
    uint8           hdrVer[SAL_SYS_STRING_LEN];
    uint8           devDesc[SAL_SYS_STRING_LEN];    
    sal_ipaddr_t    ip, netmask, gateway;
    uint8           ipStr[16], netMaskStr[16], gatewayStr[16];

    DBG_LWPS_HTTP_PRINTF(("-------get_info()-------\n"));

    sal_sys_mac_get(&mac);    
    mac_to_str(macStr, mac.octet);    
    sal_sys_ip_get(&ip, &netmask);
    ip_to_str(ipStr, (uint8 *)&ip);
    ip_to_str(netMaskStr, (uint8 *)&netmask);
    sal_sys_gateway_get(&gateway);
    ip_to_str(gatewayStr, (uint8 *)&gateway);

    sal_sys_firmVersion_get(firmVer);
    sal_sys_firmDate_get(firmDate);
    sal_sys_hardwareVersion_get(hdrVer);
    sal_sys_dev_type_get(devDesc);

    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html lang=\"en\">\n");
    WEB_PRINTF(pReq, "<head>\n");
    WEB_PRINTF(pReq, "<meta charset=\"UTF-8\">\n");
    WEB_PRINTF(pReq, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    WEB_PRINTF(pReq, "<title>System Information</title>\n");
    WEB_PRINTF(pReq, "<style>\n");
    WEB_PRINTF(pReq, "* { box-sizing: border-box; margin: 0; padding: 0; }\n");
    WEB_PRINTF(pReq, "body { font-family: Arial, sans-serif; background-color: #f4f7fa; }\n");
    WEB_PRINTF(pReq, ".container { max-width: 900px; margin: 20px auto; padding: 20px; }\n");
    WEB_PRINTF(pReq, ".page-title { text-align: center; margin-bottom: 30px; color: #333; font-size: 28px; }\n");
    WEB_PRINTF(pReq, ".dashboard { display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; }\n");
    WEB_PRINTF(pReq, ".card { background: white; border-radius: 10px; box-shadow: 0 5px 15px rgba(0,0,0,0.08); padding: 20px; flex: 1 1 300px; margin-bottom: 20px; }\n");
    WEB_PRINTF(pReq, ".card-header { margin-bottom: 15px; padding-bottom: 10px; border-bottom: 1px solid #eee; display: flex; justify-content: space-between; align-items: center; }\n");
    WEB_PRINTF(pReq, ".card-title { font-size: 18px; font-weight: 600; color: #333; }\n");
    WEB_PRINTF(pReq, ".card-content { display: flex; flex-direction: column; gap: 15px; }\n");
    WEB_PRINTF(pReq, ".info-row { display: flex; align-items: baseline; margin-bottom: 10px; }\n");
    WEB_PRINTF(pReq, ".info-label { font-weight: bold; width: 140px; color: #555; }\n");
    WEB_PRINTF(pReq, ".info-value { color: #333; flex-grow: 1; }\n");
    WEB_PRINTF(pReq, ".badge { display: inline-block; padding: 5px 10px; border-radius: 20px; font-size: 14px; font-weight: 500; background-color: #e0e0e0; color: #333; }\n");
    WEB_PRINTF(pReq, ".badge-primary { background-color: #e3f2fd; color: #1565c0; }\n");
    WEB_PRINTF(pReq, ".badge-success { background-color: #e8f5e9; color: #2e7d32; }\n");
    WEB_PRINTF(pReq, ".badge-info { background-color: #e0f7fa; color: #00838f; }\n");
    WEB_PRINTF(pReq, "</style>\n");
    WEB_PRINTF(pReq, "</head>\n");
    WEB_PRINTF(pReq, "<body>\n");
    WEB_PRINTF(pReq, "<div class=\"container\">\n");
    WEB_PRINTF(pReq, "  <h1 class=\"page-title\">System Information</h1>\n");
    
    WEB_PRINTF(pReq, "  <div class=\"dashboard\">\n");
    
    // Device Card
    WEB_PRINTF(pReq, "    <div class=\"card\">\n");
    WEB_PRINTF(pReq, "      <div class=\"card-header\">\n");
    WEB_PRINTF(pReq, "        <h2 class=\"card-title\">Device Information</h2>\n");
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "      <div class=\"card-content\">\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">Device Type</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\"><span class=\"badge badge-primary\">ONE BOX SMART POE</span></div>\n");
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">MAC Address</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\">%s</div>\n", macStr);
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">Hardware Version</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\">06062025</div>\n");
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "    </div>\n");
    
    // Network Card
    WEB_PRINTF(pReq, "    <div class=\"card\">\n");
    WEB_PRINTF(pReq, "      <div class=\"card-header\">\n");
    WEB_PRINTF(pReq, "        <h2 class=\"card-title\">Network Configuration</h2>\n");
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "      <div class=\"card-content\">\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">IP Address</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\"><span class=\"badge badge-info\">%s</span></div>\n", ipStr);
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">Netmask</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\">%s</div>\n", netMaskStr);
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">Gateway</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\">%s</div>\n", gatewayStr);
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "    </div>\n");
    
    // Firmware Card
    WEB_PRINTF(pReq, "    <div class=\"card\">\n");
    WEB_PRINTF(pReq, "      <div class=\"card-header\">\n");
    WEB_PRINTF(pReq, "        <h2 class=\"card-title\">Software Information</h2>\n");
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "      <div class=\"card-content\">\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">Firmware Version</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\"><span class=\"badge badge-success\">%s</span></div>\n", firmVer);
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">Firmware Date</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\">%s</div>\n", firmDate);
    WEB_PRINTF(pReq, "        </div>\n");

    WEB_PRINTF(pReq, "        <div class=\"info-row\">\n");
    WEB_PRINTF(pReq, "          <div class=\"info-label\">FMD Version</div>\n");
    WEB_PRINTF(pReq, "          <div class=\"info-value\"><span id=\"fmd-version\" class=\"badge badge-info\">-</span></div>\n");
    WEB_PRINTF(pReq, "        </div>\n");
    
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "    </div>\n");
    
    WEB_PRINTF(pReq, "  </div>\n"); // End of dashboard
    WEB_PRINTF(pReq, "</div>\n"); // End of container
    WEB_PRINTF(pReq, "<script>\n");
    WEB_PRINTF(pReq, "async function updateFmdVersion(){\n");
    WEB_PRINTF(pReq, "  try {\n");
    WEB_PRINTF(pReq, "    const ip = window.location.hostname;\n");
    WEB_PRINTF(pReq, "    const r = await fetch(`http://${ip}:80/get_poe_status`);\n");
    WEB_PRINTF(pReq, "    const j = await r.json();\n");
    WEB_PRINTF(pReq, "    const el = document.getElementById('fmd-version');\n");
    WEB_PRINTF(pReq, "    if (el) { el.textContent = (j && j.F !== undefined) ? String(j.F) : '-'; }\n");
    WEB_PRINTF(pReq, "  } catch(e){}\n");
    WEB_PRINTF(pReq, "}\n");
    WEB_PRINTF(pReq, "document.addEventListener('DOMContentLoaded', function(){\n");
    WEB_PRINTF(pReq, "  updateFmdVersion();\n");
    WEB_PRINTF(pReq, "  setInterval(updateFmdVersion, 7000);\n");
    WEB_PRINTF(pReq, "});\n");
    WEB_PRINTF(pReq, "</script>\n");
    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");

    WEB_END(pReq);

    return SUCCESS;
}

#if __WEB_CABLE_DIAGNOSTIC__

int32 get_cable_diag(http_request_t *pReq)
{
//    int8                i;
//	char                strPort[4];
//    char                strCheck[48];
//    char                strResult[12];
//    char                strDescription[80];
//    uint8               *pValue;
//    sal_rtctResult_t    rtctResult;
//    sal_pPortmask_t     selectPmsk;
//    int32               ret;
//    uint32 j;

//    /* get selected portmask */
//    selectPmsk.bits[0] = 0;
//    for (i = 0; i < (MAX_PORT_ONBOARD - SAL_MAX_EXT_PORT); i++)
//    {
//        sprintf(strPort, "P%d", (int16)(i+1));    
//        WEB_VAR(pReq, strPort, &pValue);
//        if (pValue) /* enable */
//            selectPmsk.bits[0] |= 1UL << i;
//    }

//    WEB_BEGIN(pReq);

//    WEB_STATUS(pReq, HTTP_STATUS_OK);
//    WEB_HEADER(pReq, "Content-Type: text/html");

//    WEB_PRINTF(pReq, "<html>\n");
//    WEB_PRINTF(pReq, "<head>\n");
//    WEB_PRINTF(pReq, "<title>Port Setting</title>\n");
//    WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");
//    WEB_PRINTF(pReq, "</head>\n");
//    WEB_PRINTF(pReq, "<body>\n");
//    WEB_PRINTF(pReq, "<center>\n");
//    WEB_PRINTF(pReq, "<fieldset>\n");
//    WEB_PRINTF(pReq, "<legend>Cable Diagnostic</legend>\n");
//    WEB_PRINTF(pReq, "<form method=\"get\" action=\"poe.cgi\">\n");
//    WEB_PRINTF(pReq, "<table border=\"1\">\n");
//    WEB_PRINTF(pReq, "  <tr>\n");
//    WEB_PRINTF(pReq, "    <th width=\"60\">Check</th>\n");
//    WEB_PRINTF(pReq, "    <th width=\"80\">Port</th>\n");
//    WEB_PRINTF(pReq, "    <th width=\"100\">Test Result</th>\n");
//    WEB_PRINTF(pReq, "    <th width=\"440\">Cable Fault Distance</th>\n");
//    WEB_PRINTF(pReq, "  </tr>\n");

//    for (i = 0; i < (MAX_PORT_ONBOARD - SAL_MAX_EXT_PORT); i++)
//    {
//        if (selectPmsk.bits[0] & (1UL << i))
//        {
//            sal_port_rtct_start(i);

//        }
//    }
//    if(selectPmsk.bits[0] != 0)
//    {
//        for(j=0;j<0x9ffff;j++);
//    }

//    for (i = 0; i < (MAX_PORT_ONBOARD - SAL_MAX_EXT_PORT); i++)
//    {
//        if (selectPmsk.bits[0] & (1UL << i))
//        {
//            memset(&rtctResult, 0, sizeof(sal_rtctResult_t));
//            
//            ret = sal_port_rtctResult_get(i, &rtctResult);

//            sprintf(strCheck, "<input type=checkbox name=P%d checked>", (int16)(i+1));

//            if (SAL_ERR_FAILED == ret)
//            {
//                sprintf(strResult, "ERROR");
//                sprintf(strDescription, "Failed");
//            }
//            else if (SAL_ERR_NOT_SUPPORT == ret)
//            {
//                sprintf(strResult, "-");
//                sprintf(strDescription, "Not support.");
//            }
//            else
//            {
//                if (SAL_PORT_SPEED_1000 == rtctResult.linkType)
//                {
//                    if (rtctResult.ge_result.channelAShort | \
//                        rtctResult.ge_result.channelBShort)
//                    {
//                        sprintf(strResult, "Short");
//                    }
//                    else if (rtctResult.ge_result.channelAOpen | \
//                             rtctResult.ge_result.channelBOpen)
//                    {
//                        sprintf(strResult, "Open");
//                    }
//                    else if (rtctResult.ge_result.channelAMismatch | \
//                             rtctResult.ge_result.channelBMismatch)
//                    {
//                        sprintf(strResult, "Mismatch");
//                    }
//                    else
//                    {
//                        sprintf(strResult, "Normal");
//                    }
//                    sprintf(strDescription, "PairA:%u(M), PairB:%u(M), PairC:%u(M), PairD:%u(M)\n", \
//                        (rtctResult.ge_result.channelALen / 40), \
//                        (rtctResult.ge_result.channelBLen / 40), \
//                        (rtctResult.ge_result.channelCLen / 40), \
//                        (rtctResult.ge_result.channelDLen / 40));
//                }
//            }
//        }
//        else
//        {
//            sprintf(strCheck, "<input type=checkbox name=P%d>", (uint16)(i+1));
//            sprintf(strResult, "-");
//            sprintf(strDescription, "-");
//        }

//        /* Display */
//        WEB_PRINTF(pReq, "  <tr>\n");
//        WEB_PRINTF(pReq, "    <td>%s</td>\n", strCheck);
//        WEB_PRINTF(pReq, "    <td>Port %d</td>\n", (int16)(i + 1));
//        WEB_PRINTF(pReq, "    <td>%s</td>\n", strResult);
//        WEB_PRINTF(pReq, "    <td>%s</td>\n", strDescription);
//        WEB_PRINTF(pReq, "  </tr>\n");
//    }

//    WEB_PRINTF(pReq, "</table>\n");
//    WEB_PRINTF(pReq, "  <br style=\"line-height:50%%\">\n");
//    WEB_PRINTF(pReq, "  <input type=\"submit\" value=\"   Apply   \">\n");
//    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"page\" value=\"cable_diag\">\n");
//    WEB_PRINTF(pReq, "</form>\n");
//    WEB_PRINTF(pReq, "<br>\n");
//    WEB_PRINTF(pReq, "</fieldset>\n");
//    WEB_PRINTF(pReq, "</center>\n");
//    WEB_PRINTF(pReq, "</body>\n");
//    WEB_PRINTF(pReq, "</html>\n");

//    WEB_END(pReq);

//    return SUCCESS;
}

#endif

#define PING_COUNT   4

/*
 * Real ICMP ping (Echo Request/Reply).
 *
 * eth_down() drops IP packets when no ARP entry exists, so we must
 * resolve ARP first, then send ICMP. Non-blocking multi-phase via
 * meta-refresh (never wait inside the HTTP handler).
 *
 * Phases driven by query flag r:
 *   (none)  - start: ARP request
 *   r=1     - ARP ready? send ICMP #0
 *   r=2..N  - check previous reply, send next / finish
 */
int32 get_ping(struct http_request_s *pReq)
{
    static uint8      ping_active  = 0;
    static uint8      ping_phase   = 0;   /* 0=idle 1=wait ARP 2=ICMP in progress */
    static uint8      ping_sent    = 0;
    static uint8      ping_recv    = 0;
    static uint16     ping_ident   = 0;
    static uint16     ping_ident_seed = 0;
    static ip_addr_t  ping_target;
    static uint8      ping_log[PING_COUNT]; /* 0=pend 1=ok 2=timeout */

    uint8           *pValue;
    uint8           *pCheck;
    uint32          ip = 0;
    ip_addr_t       tip;
    eth_addr_t      mac;
    uint8           ipStr[16];
    const char      *pResult;
    uint8           tested = 0;
    uint8           waiting = 0;
    uint8           showTable = 0;
    uint8           idx;
    uint8           i;
    uint8           rphase = 0;

    ipStr[0] = '\0';
    pResult = "";
    pCheck = NULL;

    WEB_VAR(pReq, "ip", &pValue);
    if (pValue && pValue[0] != '\0')
    {
        tested = 1;
        if (str_to_ip(&ip, (char *)pValue) != 0)
        {
            pResult = "Invalid IP";
            ping_active = 0;
            ping_phase = 0;
        }
        else
        {
            tip.addr[0] = (uint8)((ip >> 24) & 0xFF);
            tip.addr[1] = (uint8)((ip >> 16) & 0xFF);
            tip.addr[2] = (uint8)((ip >> 8) & 0xFF);
            tip.addr[3] = (uint8)(ip & 0xFF);
            ip_to_str(ipStr, tip.addr);

            if (IS_IPADDR_EQUAL(tip.addr, this_ip))
            {
                pResult = "Reachable (this device)";
                ping_active = 0;
                ping_phase = 0;
            }
            else
            {
                showTable = 1;
                WEB_VAR(pReq, "r", &pCheck);
                if (pCheck && pCheck[0] >= '1' && pCheck[0] <= '9')
                    rphase = (uint8)(pCheck[0] - '0');

                if (rphase == 0)
                {
                    /* Fresh start */
                    ping_ident_seed++;
                    ping_ident  = ping_ident_seed;
                    ping_active = 1;
                    ping_sent = 0;
                    ping_recv = 0;
                    IPADDR_COPY(ping_target.addr, tip.addr);
                    for (i = 0; i < PING_COUNT; i++)
                        ping_log[i] = 0;

                    /* If ARP already known, send ICMP now; else ARP first */
                    if (LWPS_OK == etharp_entry_find(&tip, &mac))
                    {
                        icmp_echo_reply_clear();
                        if (LWPS_OK == icmp_echo_send(ping_target, ping_ident, 0))
                        {
                            ping_sent = 1;
                            ping_phase = 2;
                            waiting = 1;
                            pResult = "Testing...";
                        }
                        else
                        {
                            /* Send failed - force re-ARP */
                            (void)etharp_entry_clear(&tip);
                            (void)lwps_arp_request(tip);
                            ping_phase = 1;
                            waiting = 1;
                            pResult = "Resolving...";
                        }
                    }
                    else
                    {
                        (void)lwps_arp_request(tip);
                        ping_phase = 1;
                        waiting = 1;
                        pResult = "Resolving...";
                    }
                }
                else if (ping_active && ping_phase == 1)
                {
                    /* ARP result phase */
                    if (LWPS_OK == etharp_entry_find(&ping_target, &mac))
                    {
                        icmp_echo_reply_clear();
                        if (LWPS_OK == icmp_echo_send(ping_target, ping_ident, 0))
                        {
                            ping_sent = 1;
                            ping_phase = 2;
                            waiting = 1;
                            pResult = "Testing...";
                        }
                        else
                        {
                            ping_active = 0;
                            ping_phase = 0;
                            pResult = "Send failed";
                        }
                    }
                    else
                    {
                        ping_active = 0;
                        ping_phase = 0;
                        pResult = "Unreachable (no ARP)";
                        for (i = 0; i < PING_COUNT; i++)
                            ping_log[i] = 2;
                        ping_sent = PING_COUNT;
                    }
                }
                else if (ping_active && ping_phase == 2)
                {
                    /* Check previous ICMP reply */
                    if (ping_sent > 0)
                    {
                        idx = ping_sent - 1;
                        if (icmp_echo_reply_check(ping_ident, idx))
                        {
                            ping_recv++;
                            ping_log[idx] = 1;
                        }
                        else
                        {
                            ping_log[idx] = 2;
                        }
                        icmp_echo_reply_clear();
                    }

                    if (ping_sent < PING_COUNT)
                    {
                        (void)icmp_echo_send(ping_target, ping_ident, ping_sent);
                        ping_sent++;
                        waiting = 1;
                        pResult = "Testing...";
                    }
                    else
                    {
                        ping_active = 0;
                        ping_phase = 0;
                        pResult = (ping_recv > 0) ? "Completed" : "Unreachable (100% loss)";
                    }
                }
                else
                {
                    /* Stale refresh */
                    pResult = "Completed";
                    showTable = (ping_sent > 0) ? 1 : 0;
                }
            }
        }
    }
    else
    {
        ping_active = 0;
        ping_phase = 0;
    }

    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html><head><title>Ping Test</title>\n");
    WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");
    if (waiting)
        WEB_PRINTF(pReq,
            "<meta http-equiv=\"refresh\" content=\"2;url=info.cgi?page=ping&ip=%s&r=1\">\n",
            ipStr);
    WEB_PRINTF(pReq, "<style>\n");
    WEB_PRINTF(pReq, "table.pingres{border-collapse:collapse;margin:10px auto;}\n");
    WEB_PRINTF(pReq, "table.pingres td,table.pingres th{border:1px solid #ccc;padding:4px 10px;font-size:13px;}\n");
    WEB_PRINTF(pReq, ".ok{color:#2e7d32;font-weight:bold;}\n");
    WEB_PRINTF(pReq, ".fail{color:#c62828;font-weight:bold;}\n");
    WEB_PRINTF(pReq, ".pend{color:#999;}\n");
    WEB_PRINTF(pReq, "</style>\n");
    WEB_PRINTF(pReq, "<script>\n");
    WEB_PRINTF(pReq, "function lockTest(f){if(f._lk)return false;f._lk=1;f.testBtn.disabled=true;f.testBtn.value='Testing...';return true;}\n");
    WEB_PRINTF(pReq, "</script>\n");
    WEB_PRINTF(pReq, "</head><body><center>\n");
    WEB_PRINTF(pReq, "<fieldset><legend>IP Diagnostic - ICMP Ping</legend>\n");
    WEB_PRINTF(pReq, "<form method=\"get\" action=\"info.cgi\" onsubmit=\"return lockTest(this)\">\n");
    WEB_PRINTF(pReq, "IP: <input type=\"text\" name=\"ip\" value=\"%s\" maxlength=\"15\" %s>\n",
               ipStr, waiting ? "disabled" : "");
    WEB_PRINTF(pReq, "<input type=\"hidden\" name=\"page\" value=\"ping\">\n");
    if (waiting)
        WEB_PRINTF(pReq, "<input type=\"hidden\" name=\"ip\" value=\"%s\">\n", ipStr);
    WEB_PRINTF(pReq, "<input type=\"submit\" name=\"testBtn\" value=\"%s\" %s>\n",
               waiting ? "Testing..." : "Start Ping", waiting ? "disabled" : "");
    WEB_PRINTF(pReq, "</form>\n");

    if (tested)
        WEB_PRINTF(pReq, "<p><b>Status: %s</b></p>\n", pResult);

    if (showTable && ping_sent > 0)
    {
        WEB_PRINTF(pReq, "<table class=\"pingres\">\n");
        WEB_PRINTF(pReq, "<tr><th>Attempt</th><th>Target</th><th>Result</th></tr>\n");
        for (i = 0; i < ping_sent; i++)
        {
            WEB_PRINTF(pReq, "<tr><td>%d</td><td>%s</td>", (uint16)(i + 1), ipStr);
            if (ping_log[i] == 1)
                WEB_PRINTF(pReq, "<td class=\"ok\">Reply received</td></tr>\n");
            else if (ping_log[i] == 2)
                WEB_PRINTF(pReq, "<td class=\"fail\">Request timed out</td></tr>\n");
            else
                WEB_PRINTF(pReq, "<td class=\"pend\">Waiting...</td></tr>\n");
        }
        WEB_PRINTF(pReq, "</table>\n");

        WEB_PRINTF(pReq, "<p>Packets: Sent = %d, Received = %d, Lost = %d (%d%% loss)</p>\n",
                   (uint16)ping_sent,
                   (uint16)ping_recv,
                   (uint16)(ping_sent - ping_recv),
                   (uint16)(ping_sent ? (((ping_sent - ping_recv) * 100) / ping_sent) : 0));
    }

    WEB_PRINTF(pReq, "<p style=\"font-size:11px;color:#888\">Real ICMP Echo (Wireshark filter: icmp)</p>\n");
    WEB_PRINTF(pReq, "</fieldset></center></body></html>\n");

    WEB_END(pReq);
    return SUCCESS;
}
