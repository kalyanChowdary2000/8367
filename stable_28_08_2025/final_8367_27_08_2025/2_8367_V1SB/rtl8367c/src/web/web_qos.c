#pragma STRING(far)
/*
 * Copyright(c) Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : The handler of HTTP request to QoS function
 *
 * Feature : Response the request related to QoS function
 *
 */

/*
 * Include Files
 */
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <salqos.h>
#include <salerror.h>
#include <saltrunk.h>
#include <salsys.h>

#include <web/web_api.h>
#include "web_qos.h"
#include "web_util.h"

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Function Declaration
 */

int32 get_qos(struct http_request_s *pReq)
{
    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html>\n");
    WEB_PRINTF(pReq, "This is qos.cgi");
    WEB_PRINTF(pReq, "</html>\n");

    WEB_END(pReq);

    return SUCCESS;
}

int32 get_qos_portbasedpriority(struct http_request_s *pReq)
{
    //    sal_qos_pri2queue_t pri2qid;
    sal_pri_t pri;
    uint8 port = 0;
    uint8 logicPortStr[MAX_LOGIC_ID_LEN];
    //    uint8       i;
    uint8 queue_num;

    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<head>\n");
    //    WEB_PRINTF(pReq, "<title>Port-based Priority</title>\n");
    //    WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");
    //    WEB_PRINTF(pReq, "</head>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<body>\n");
    //    WEB_PRINTF(pReq, "<center>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<fieldset>\n");
    //    WEB_PRINTF(pReq, "<legend>Port-based Priority Setting</legend>\n");

    //    WEB_PRINTF(pReq, "<form method=\"post\" action=\"/qos.cgi?page=port_pri\">\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"100\">Port</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"portid\" multiple size=\"6\" >\n");

    //    for (port = 0; port < SAL_PORT_ID_CPU; ++port) {
    //            if (!(sal_trunk_logicId_exist(port))) {
    //            continue;
    //            }

    //            sal_trunk_logicId_toStr(logicPortStr, port);
    //            WEB_PRINTF(pReq, "          <option value=\"%d\">%s\n", (uint16)port, logicPortStr);
    //    }
    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"port_priority\" >\n");
    //    sal_qos_queueNum_get(0, &queue_num);

    //    for (pri= 0; pri < queue_num; ++pri) {
    //        WEB_PRINTF(pReq, "          <option value=\"%d\">%d\n", (uint16)pri*2, (uint16)pri*2);
    //    }

    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <br style=\"line-height:50%%\">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"submit\" value=\"      Apply      \">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"portprio\">\n");
    //    WEB_PRINTF(pReq, "</form>\n");

    //    WEB_PRINTF(pReq, "<hr>\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"100\">Port</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");

    //    //sal_qos_priMap_get(queue_num, &pri2qid);
    //    for (port = 0; port < SAL_PORT_ID_CPU; ++port) {
    //            if (!(sal_trunk_logicId_exist(port))) {
    //            continue;
    //            }

    //            sal_trunk_logicId_toStr(logicPortStr, port);
    //            sal_qos_portPri_get(port, &pri);
    //        WEB_PRINTF(pReq, "  <tr>\n");
    //        WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", logicPortStr);
    //        WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)pri);
    //        WEB_PRINTF(pReq, "  </tr>\n");
    //    }
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"portprios\">\n");
    //    WEB_PRINTF(pReq, "<br>\n");
    //    WEB_PRINTF(pReq, "</fieldset>\n");
    //    WEB_PRINTF(pReq, "<p>\n");
    //    WEB_PRINTF(pReq, "\n");

    //    WEB_PRINTF(pReq, "</center>\n");
    //    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");
    WEB_END(pReq);

    return SUCCESS;
}

int32 get_qos_packetscheduling(struct http_request_s *pReq)
{
    sal_qos_queue_weights_t qweights;
    uint8 port = 0;
    uint8 queue_num;
    uint8 i;

    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<head>\n");
    //    WEB_PRINTF(pReq, "<title>Queue Weight</title>\n");
    //    WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");

    //    WEB_PRINTF(pReq, "</head>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<body>\n");
    //    WEB_PRINTF(pReq, "<center>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<fieldset>\n");
    //    WEB_PRINTF(pReq, "<legend>Queue Weight Setting</legend>\n");

    //    WEB_PRINTF(pReq, "<form method=\"post\" action=\"/qos.cgi?page=que_weight\">\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th bgcolor=\"#99ccff\" >Priority Queue</th>\n");
    //    WEB_PRINTF(pReq, "      <th bgcolor=\"#99ccff\" >Weight</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");

    //    sal_qos_queueNum_get(0, &queue_num);
    //    WEB_PRINTF(pReq, "        <select name=\"queueid\" multiple size=\"%d\" >\n", (uint16)queue_num);
    //    for (i = 0; i < queue_num; ++i) {
    //        if (0 == i)
    //            WEB_PRINTF(pReq, "          <option value=\"%d\">%d(lowest)\n", (uint16)i, (uint16)(i+1));
    //        else if (queue_num-1 == i)
    //            WEB_PRINTF(pReq, "          <option value=\"%d\">%d(highest)\n", (uint16)i, (uint16)(i+1));
    //        else
    //            WEB_PRINTF(pReq, "          <option value=\"%d\">%d\n", (uint16)i, (uint16)(i+1));
    //    }
    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"weight\" >\n");
    //    WEB_PRINTF(pReq, "          <option value=\"0\">Strict priority\n");
    //    for (i = 1; i <= 15; ++i) {
    //            WEB_PRINTF(pReq, "          <option value=\"%d\">%d\n", (uint16)i, (uint16)i);
    //    }
    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <br style=\"line-height:50%%\">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"submit\" value=\"      Apply      \">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"qweight\">\n");
    //    WEB_PRINTF(pReq, "</form>\n");

    //    WEB_PRINTF(pReq, "<hr>\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th bgcolor=\"#99ccff\" width=\"120\">Priority Queue</th>\n");
    //    WEB_PRINTF(pReq, "      <th bgcolor=\"#99ccff\" width=\"150\">Weight</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");

    //    sal_qos_schedulingQueue_get(port, &qweights);
    //
    //    for (i= 0; i < queue_num; ++i) {
    //            WEB_PRINTF(pReq, "  <tr>\n");
    //            WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)(i + 1));
    //
    //        if (0 == qweights.weights[i])
    //            WEB_PRINTF(pReq, "    <td align=\"center\">Strict priority</td>\n");
    //        else {
    //            WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)qweights.weights[i]);
    //        }
    //        WEB_PRINTF(pReq, "  </tr>\n");
    //    }

    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"portprios\">\n");
    //    WEB_PRINTF(pReq, "<br>\n");
    //    WEB_PRINTF(pReq, "</fieldset>\n");
    //    WEB_PRINTF(pReq, "\n");

    //    WEB_PRINTF(pReq, "</center>\n");
    //    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");

    WEB_END(pReq);

    return SUCCESS;
}

int32 get_qos_internalPriority(struct http_request_s *pReq)
{
    uint16 int_pri_number;
    sal_qos_pri_selet_t int_pri;

    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<head>\n");
    //    WEB_PRINTF(pReq, "<title>Priority selection</title>\n");
    //    WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");
    //    WEB_PRINTF(pReq, "</head>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<body>\n");
    //    WEB_PRINTF(pReq, "<center>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<fieldset>\n");
    //    WEB_PRINTF(pReq, "<legend>Priority selection Setting</legend>\n");

    //    WEB_PRINTF(pReq, "<form method=\"post\" action=\"/qos.cgi?page=qos_int_pri\">\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Decision</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"internalPriority\" multiple size=\"6\" >\n");

    //    WEB_PRINTF(pReq, "          <option value=\"0\">Port\n");
    //    WEB_PRINTF(pReq, "          <option value=\"1\">1Q\n");
    //    WEB_PRINTF(pReq, "          <option value=\"2\">ACL\n");
    //    WEB_PRINTF(pReq, "          <option value=\"3\">DSCP\n");
    //    WEB_PRINTF(pReq, "          <option value=\"4\">CVLAN\n");
    //    WEB_PRINTF(pReq, "          <option value=\"5\">SVLAN\n");
    //    WEB_PRINTF(pReq, "          <option value=\"6\">DA\n");
    //    WEB_PRINTF(pReq, "          <option value=\"7\">SA\n");
    //
    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"decision\" >\n");

    //    for (int_pri_number = 0; int_pri_number < SAL_MAX_NUM_OF_PRIORITY; int_pri_number++) {
    //        WEB_PRINTF(pReq, "          <option value=\"%d\">%d\n", int_pri_number, (int_pri_number+1));
    //    }
    //
    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <br style=\"line-height:50%%\">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"submit\" value=\"      Apply      \">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"intPri\">\n");
    //    WEB_PRINTF(pReq, "</form>\n");

    //    WEB_PRINTF(pReq, "<hr>\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Decision</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");

    //    sal_qos_priSel_get(&int_pri);

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "Port");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.port_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "1Q");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.dot1q_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "ACL");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.acl_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "DSCP");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.dscp_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "CVLAN");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.cvlan_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "SVLAN");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.svlan_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "DA");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.dmac_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  <tr>\n");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%s</td>\n", "SA");
    //    WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri.smac_pri+1);
    //    WEB_PRINTF(pReq, "  </tr>\n");

    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"intPriDecision\">\n");
    //    WEB_PRINTF(pReq, "<br>\n");
    //    WEB_PRINTF(pReq, "</fieldset>\n");
    //    WEB_PRINTF(pReq, "<p>\n");
    //    WEB_PRINTF(pReq, "\n");

    //    WEB_PRINTF(pReq, "</center>\n");
    //    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");
    WEB_END(pReq);

    return SUCCESS;
}

int32 get_qos_dscpremapping(struct http_request_s *pReq)
{
    uint16 dscp;
    uint8 queue_num, pri;
    sal_dscp_t dscp_val;
    sal_pri_t int_pri;

    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<head>\n");
    //    WEB_PRINTF(pReq, "<title>Dscp remapping</title>\n");
    //    WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");
    //    WEB_PRINTF(pReq, "</head>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<body>\n");
    //    WEB_PRINTF(pReq, "<center>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<fieldset>\n");
    //    WEB_PRINTF(pReq, "<legend>DSCP remapping Setting</legend>\n");

    //    WEB_PRINTF(pReq, "<form method=\"post\" action=\"/qos.cgi?page=dscp_remap\">\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">DSCP value</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"dscp_value\" multiple size=\"6\" >\n");

    //    for (dscp = 0; dscp < SAL_MAX_VALUE_OF_DSCP; ++dscp) {
    //        WEB_PRINTF(pReq, "          <option value=\"%d\">%d\n", dscp, dscp);
    //    }
    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"dscp_priority\" >\n");

    //    sal_qos_queueNum_get(0, &queue_num);

    //    for (pri= 0; pri < queue_num; ++pri) {
    //            WEB_PRINTF(pReq, "          <option value=\"%d\">%d\n", (uint16)pri*2, (uint16)pri*2);
    //    }

    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <br style=\"line-height:50%%\">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"submit\" value=\"      Apply      \">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"dscppri\">\n");
    //    WEB_PRINTF(pReq, "</form>\n");

    //    WEB_PRINTF(pReq, "<hr>\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">DSCP value</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");

    //    for (dscp_val = 0; dscp_val < SAL_MAX_VALUE_OF_DSCP; ++dscp_val) {
    //        sal_qos_dscpPriRemap_get(dscp_val, &int_pri);
    //        WEB_PRINTF(pReq, "  <tr>\n");
    //        WEB_PRINTF(pReq, "    <td align=\"center\">%lu</td>\n", dscp_val);
    //        WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)int_pri);
    //        WEB_PRINTF(pReq, "  </tr>\n");
    //    }
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"dscpvalue\">\n");
    //    WEB_PRINTF(pReq, "<br>\n");
    //    WEB_PRINTF(pReq, "</fieldset>\n");
    //    WEB_PRINTF(pReq, "<p>\n");
    //    WEB_PRINTF(pReq, "\n");

    //    WEB_PRINTF(pReq, "</center>\n");
    //    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");
    WEB_END(pReq);

    return SUCCESS;
}

int32 get_qos_pri2queueid(struct http_request_s *pReq)
{
    uint16 pri;
    uint8 queue_num, que;
    sal_qos_pri2queue_t priToQueueMap;

    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<head>\n");
    //    WEB_PRINTF(pReq, "<title>Priority to queue id mapping</title>\n");
    //    WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");
    //    WEB_PRINTF(pReq, "</head>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<body>\n");
    //    WEB_PRINTF(pReq, "<center>\n");
    //    WEB_PRINTF(pReq, "\n");
    //    WEB_PRINTF(pReq, "<fieldset>\n");
    //    WEB_PRINTF(pReq, "<legend>Priority to queue id Setting</legend>\n");

    //    WEB_PRINTF(pReq, "<form method=\"post\" action=\"/qos.cgi?page=pri_to_queueid\">\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Queue ID</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"priority_value\" multiple size=\"6\" >\n");

    //    for (pri = 0; pri < SAL_MAX_NUM_OF_PRIORITY; ++pri) {
    //        WEB_PRINTF(pReq, "          <option value=\"%d\">%d\n", pri, pri);
    //    }
    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "      <td align=\"center\">\n");
    //    WEB_PRINTF(pReq, "        <select name=\"priority_to_queueid\" >\n");

    //    sal_qos_queueNum_get(0, &queue_num);

    //    for (que = 0; que < queue_num; ++que) {
    //        WEB_PRINTF(pReq, "          <option value=\"%d\"> %d \n", (uint16)que, (uint16)que+1);
    //    }

    //    WEB_PRINTF(pReq, "        </select>\n");
    //    WEB_PRINTF(pReq, "      </td>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <br style=\"line-height:50%%\">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"submit\" value=\"      Apply      \">\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"pri_2_queueid\">\n");
    //    WEB_PRINTF(pReq, "</form>\n");

    //    WEB_PRINTF(pReq, "<hr>\n");
    //    WEB_PRINTF(pReq, "  <table border=\"1\">\n");
    //    WEB_PRINTF(pReq, "    <tr>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Priority</th>\n");
    //    WEB_PRINTF(pReq, "      <th width=\"130\">Queue ID</th>\n");
    //    WEB_PRINTF(pReq, "    </tr>\n");

    //    sal_qos_queueNum_get(0, &queue_num);
    //    sal_qos_priMap_get(queue_num, &priToQueueMap);

    //    for (pri = 0; pri < SAL_MAX_NUM_OF_PRIORITY; ++pri) {
    //        WEB_PRINTF(pReq, "  <tr>\n");
    //        WEB_PRINTF(pReq, "    <td align=\"center\">%d</td>\n", (uint16)pri);
    //        WEB_PRINTF(pReq, "    <td align=\"center\">%lu</td>\n", priToQueueMap.pri2queue[pri]+1);
    //        WEB_PRINTF(pReq, "  </tr>\n");
    //    }
    //    WEB_PRINTF(pReq, "  </table>\n");
    //    WEB_PRINTF(pReq, "  <input type=\"hidden\" name=\"cmd\" value=\"que_id\">\n");
    //    WEB_PRINTF(pReq, "<br>\n");
    //    WEB_PRINTF(pReq, "</fieldset>\n");
    //    WEB_PRINTF(pReq, "<p>\n");
    //    WEB_PRINTF(pReq, "\n");

    //    WEB_PRINTF(pReq, "</center>\n");
    //    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");
    WEB_END(pReq);

    return SUCCESS;
}

int32 post_qos_portbasedpriority(struct http_request_s *pReq)
{
    int32 ret;
    int16 i;
    uint8 port[SAL_MAX_LOGIC_COMBO_PORT];
    uint8 *pValue;
    uint8 settingCnt = 0, priority = 0;

    WEB_VAR(pReq, "portid", &pValue);
    if (pValue)
    {
        port[settingCnt++] = atoi(pValue);
    }
    while (pValue != NULL)
    {
        WEB_VAR(pReq, NULL, &pValue);
        if (pValue)
        {
            port[settingCnt++] = atoi(pValue);
        }
    }

    WEB_VAR(pReq, "port_priority", &pValue);
    if (pValue)
    {
        priority = atoi(pValue);
    }

    for (i = 0; i < settingCnt; ++i)
    {

        if (!(sal_trunk_logicId_exist(port[i])))
        {
            continue;
        }
#if 1

        if ((ret = sal_qos_portPri_set(port[i], priority)) != SAL_ERR_SUCCESS)
            return ret;
#else
        if ((ret = sal_qos_port2queue_set(port[i], qid)) != SAL_ERR_SUCCESS)
        {
            printerr(pReq, ret);
            return SUCCESS;
        }
#endif
    }

    get_qos_portbasedpriority(pReq);
    return SUCCESS;
}

int32 post_qos_queueweight(struct http_request_s *pReq)
{
    sal_qos_queue_weights_t qweights;
    int32 ret;
    int16 i;
    uint8 port;
    uint8 queueId[SAL_MAX_NUM_OF_QUEUE];
    uint8 *value;

    uint8 entryNum, weight;

    entryNum = 0;
    weight = 0;
    WEB_VAR(pReq, "queueid", &value);
    if (value)
    {
        queueId[entryNum++] = atoi(value);
    }
    while (value != NULL)
    {
        WEB_VAR(pReq, NULL, &value);
        if (value)
        {
            queueId[entryNum++] = atoi(value);
        }
    }

    WEB_VAR(pReq, "weight", &value);
    if (value)
    {
        weight = atoi(value);
    }

    for (port = 0; port < SAL_PORT_ID_CPU; ++port)
    {

        if (!(sal_trunk_logicId_exist(port)))
        {
            continue;
        }

        sal_qos_schedulingQueue_get(port, &qweights);
        for (i = 0; i < entryNum; i++)
        {
            qweights.weights[queueId[i]] = weight;
        }
        if ((ret = sal_qos_schedulingQueue_set(port, &qweights)) != SAL_ERR_SUCCESS)
        {
            printerr(pReq, ret);
            return SUCCESS;
        }
    }

    get_qos_packetscheduling(pReq);
    return SUCCESS;
}

int32 post_qos_internalPriority(struct http_request_s *pReq)
{
    int32 ret;
    int16 i;
    uint16 priSet[8];
    uint8 *pValue;
    uint8 settingCnt = 0;
    uint16 dec;
    sal_qos_pri_selet_t priDec;

    WEB_VAR(pReq, "internalPriority", &pValue);
    if (pValue)
    {
        priSet[settingCnt++] = atoi(pValue);
    }
    while (pValue != NULL)
    {
        WEB_VAR(pReq, NULL, &pValue);
        if (pValue)
        {
            priSet[settingCnt++] = atoi(pValue);
        }
    }

    WEB_VAR(pReq, "decision", &pValue);
    if (pValue)
    {
        dec = atoi(pValue);
    }

    sal_qos_priSel_get(&priDec);
    for (i = 0; i < settingCnt; i++)
    {
        if (priSet[i] == 0)
        {
            priDec.port_pri = (uint32)dec;
        }
        else if (priSet[i] == 1)
        {
            priDec.dot1q_pri = (uint32)dec;
        }
        else if (priSet[i] == 2)
        {
            priDec.acl_pri = (uint32)dec;
        }
        else if (priSet[i] == 3)
        {
            priDec.dscp_pri = (uint32)dec;
        }
        else if (priSet[i] == 4)
        {
            priDec.cvlan_pri = dec;
        }
        else if (priSet[i] == 5)
        {
            priDec.svlan_pri = (uint32)dec;
        }
        else if (priSet[i] == 6)
        {
            priDec.dmac_pri = (uint32)dec;
        }
        else if (priSet[i] == 7)
        {
            priDec.smac_pri = (uint32)dec;
        }
        else
        {
            get_qos_internalPriority(pReq);
            return SUCCESS;
        }
    }

    if ((ret = sal_qos_priSel_set(&priDec)) != SAL_ERR_SUCCESS)
    {
        printerr(pReq, ret);
        return SUCCESS;
    }
    get_qos_internalPriority(pReq);
    return SUCCESS;
}

int32 post_qos_dscpremapping(struct http_request_s *pReq)
{
    int32 ret;
    int16 i;
    uint32 j;
    uint32 dscp[SAL_MAX_VALUE_OF_DSCP];
    uint8 priValue;
    uint8 settingCnt = 0, qid = 0;
    uint8 *pValue;

    WEB_VAR(pReq, "dscp_value", &pValue);
    if (pValue)
    {
        dscp[settingCnt++] = atoi(pValue);
    }
    while (pValue != NULL)
    {
        WEB_VAR(pReq, NULL, &pValue);
        if (pValue)
        {
            dscp[settingCnt++] = atoi(pValue);
        }
    }

    WEB_VAR(pReq, "dscp_priority", &pValue);
    if (pValue)
    {
        priValue = atoi(pValue);
    }

    for (i = 0; i < settingCnt; ++i)
    {
        for (j = 0; j < SAL_MAX_VALUE_OF_DSCP; j++)
        {
            if (dscp[i] == j)
            {
                if ((ret = sal_qos_dscpPriRemap_set(j, priValue)) != SAL_ERR_SUCCESS)
                {
                    printerr(pReq, ret);
                    return SUCCESS;
                }
            }
        }
    }

    get_qos_dscpremapping(pReq);
    return SUCCESS;
}

int32 post_qos_pri2queueid(struct http_request_s *pReq)
{
    int32 ret;
    int16 i;
    uint8 j;
    uint16 priority[SAL_MAX_NUM_OF_PRIORITY], tmp;
    uint32 queueid;
    uint8 settingCnt = 0, qid = 0;
    uint8 *pValue;
    uint8 queue_num;
    sal_qos_pri2queue_t priToQue;

    WEB_VAR(pReq, "priority_value", &pValue);
    if (pValue)
    {
        tmp = atoi(pValue);
        priority[settingCnt++] = tmp;
    }
    while (pValue != NULL)
    {
        WEB_VAR(pReq, NULL, &pValue);
        if (pValue)
        {
            priority[settingCnt++] = atoi(pValue);
        }
    }

    WEB_VAR(pReq, "priority_to_queueid", &pValue);
    if (pValue)
    {
        queueid = atoi(pValue);
    }
    sal_qos_queueNum_get(0, &queue_num);
    sal_qos_priMap_get(queue_num, &priToQue);

    for (i = 0; i < settingCnt; ++i)
    {
        for (j = 0; j < SAL_MAX_NUM_OF_PRIORITY; j++)
        {
            if (j == priority[i])
            {
                priToQue.pri2queue[j] = queueid;
            }
        }
    }
    if ((ret = sal_qos_priMap_set(queue_num, &priToQue)) != SAL_ERR_SUCCESS)
    {
        printerr(pReq, ret);
        return SUCCESS;
    }

    get_qos_pri2queueid(pReq);
    return SUCCESS;
}

int32 get_poe(struct http_request_s *pReq)
{
    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html lang=\"en\">\n");
    WEB_PRINTF(pReq, "<head>\n");
    WEB_PRINTF(pReq, "<meta charset=\"UTF-8\">\n");
    WEB_PRINTF(pReq, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    WEB_PRINTF(pReq, "<title>POE Control Panel</title>\n");
    WEB_PRINTF(pReq, "<style>\n");
    WEB_PRINTF(pReq, "* { box-sizing: border-box; margin: 0; padding: 0; }\n");
    WEB_PRINTF(pReq, "body { font-family: Arial, sans-serif; background-color: #f4f7fa; }\n");
    WEB_PRINTF(pReq, ".container { max-width: 900px; margin: 20px auto; padding: 20px; }\n");
    WEB_PRINTF(pReq, ".page-title { text-align: center; margin-bottom: 30px; color: #333; font-size: 28px; }\n");
    WEB_PRINTF(pReq, ".dashboard { display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; }\n");
    WEB_PRINTF(pReq, ".card { background: white; border-radius: 10px; box-shadow: 0 5px 15px rgba(0,0,0,0.08); padding: 20px; min-width: 300px; }\n");
    WEB_PRINTF(pReq, ".card-header { margin-bottom: 15px; padding-bottom: 10px; border-bottom: 1px solid #eee; display: flex; justify-content: space-between; align-items: center; }\n");
    WEB_PRINTF(pReq, ".card-title { font-size: 18px; font-weight: 600; color: #333; }\n");
    WEB_PRINTF(pReq, ".card-content { display: flex; flex-direction: column; gap: 15px; }\n");

    WEB_PRINTF(pReq, ".status-indicator { display: flex; align-items: center; gap: 10px; }\n");
    WEB_PRINTF(pReq, ".status-light { width: 15px; height: 15px; border-radius: 50%; box-shadow: 0 0 5px rgba(0,0,0,0.2); transition: background-color 0.3s; }\n");
    WEB_PRINTF(pReq, ".status-label { font-size: 16px; color: #333; }\n");

    WEB_PRINTF(pReq, ".battery-container { margin-top: 10px; }\n");
    /* Simplify to a basic container with relative positioning */
    WEB_PRINTF(pReq, ".battery-meter { width: 100%%; height: 25px; border: 1px solid #ddd; border-radius: 15px; background-color: #f9f9f9; position: relative; overflow: hidden; }\n");
    /* Make fill a basic block element with absolute positioning */
    WEB_PRINTF(pReq, ".battery-part { position: absolute; left: 0; top: 0; height: 100%%; background-color: #e0e0e0; }\n");
    WEB_PRINTF(pReq, ".battery-label { position: absolute; top: 50%%; left: 50%%; transform: translate(-50%%, -50%%); font-size: 14px; color: #333; font-weight: bold; }\n");

    WEB_PRINTF(pReq, ".voltage-options { display: grid; grid-template-columns: repeat(2, 1fr); gap: 15px; margin-bottom: 20px; }\n");
    WEB_PRINTF(pReq, ".voltage-option { display: flex; align-items: center; }\n");
    WEB_PRINTF(pReq, ".custom-checkbox { width: 22px; height: 22px; border-radius: 5px; background-color: white; border: 2px solid #a0a0a0; transition: all 0.3s; cursor: pointer; display: inline-block; margin-right: 10px; }\n");
    WEB_PRINTF(pReq, ".button { background-color: #1e88e5; color: white; border: none; padding: 10px 15px; border-radius: 5px; font-size: 16px; cursor: pointer; transition: background-color 0.3s; margin-top: 10px; width: 100%%; }\n");
    WEB_PRINTF(pReq, ".button:hover { background-color: #1565c0; }\n");
    WEB_PRINTF(pReq, ".button:disabled { background-color: #ccc; cursor: not-allowed; }\n");
    WEB_PRINTF(pReq, "</style>\n");
    WEB_PRINTF(pReq, "</head>\n");

    // JavaScript code for functionality
    WEB_PRINTF(pReq, "<script>\n");
    WEB_PRINTF(pReq, "let fetchFlag = true;\n");
    WEB_PRINTF(pReq, "let fetchInterval;\n");
    WEB_PRINTF(pReq, "const ip = window.location.hostname;\n");

    WEB_PRINTF(pReq, "async function getPoeData() {\n");
    WEB_PRINTF(pReq, "  try {\n");
    WEB_PRINTF(pReq, "    let response = await fetch(`http://${ip}:80/get_poe_status`);\n");
    WEB_PRINTF(pReq, "    let res1 = await response.json();\n");
    WEB_PRINTF(pReq, "    let res = { \"ac\": res1.A, \"battery\": res1.B, \"batteryPercentage\": res1.C, \"mode\": res1.D, \"voltage\": res1.E };\n");

    // Add safety checks for all DOM updates
    WEB_PRINTF(pReq, "    // Update AC status with null checks\n");
    WEB_PRINTF(pReq, "    const acLight = document.getElementById('ac-light');\n");
    WEB_PRINTF(pReq, "    const acStatus = document.getElementById('ac-status');\n");
    WEB_PRINTF(pReq, "    if (acLight) {\n");
    WEB_PRINTF(pReq, "      acLight.style.backgroundColor = res.ac === 1 ? '#4caf50' : '#ff5252';\n");
    WEB_PRINTF(pReq, "    }\n");
    WEB_PRINTF(pReq, "    if (acStatus) {\n");
    WEB_PRINTF(pReq, "      acStatus.textContent = res.ac === 1 ? 'Online' : 'Offline';\n");
    WEB_PRINTF(pReq, "    }\n");

    WEB_PRINTF(pReq, "    // Update Battery status with null checks\n");
    WEB_PRINTF(pReq, "    const batteryLight = document.getElementById('battery-light');\n");
    WEB_PRINTF(pReq, "    const batteryStatus = document.getElementById('battery-status');\n");
    WEB_PRINTF(pReq, "    if (batteryLight) {\n");
    WEB_PRINTF(pReq, "      batteryLight.style.backgroundColor = res.battery === 1 ? '#4caf50' : '#ff5252';\n");
    WEB_PRINTF(pReq, "    }\n");
    WEB_PRINTF(pReq, "    if (batteryStatus) {\n");
    WEB_PRINTF(pReq, "      batteryStatus.textContent = res.battery === 1 ? 'Active' : 'Inactive';\n");
    WEB_PRINTF(pReq, "    }\n");

    WEB_PRINTF(pReq, "    // Update Battery Percentage with direct approach\n");
    WEB_PRINTF(pReq, "    const batteryFill = document.getElementById('battery-fill');\n");
    WEB_PRINTF(pReq, "    const batteryLabel = document.getElementById('battery-label');\n");
    WEB_PRINTF(pReq, "    if (batteryFill) {\n");
    WEB_PRINTF(pReq, "      // Force percentage to be a number and log values\n");
    WEB_PRINTF(pReq, "      const rawPercentage = res.batteryPercentage;\n");
    WEB_PRINTF(pReq, "      const percentage = Number(rawPercentage);\n");
    WEB_PRINTF(pReq, "      \n");
    WEB_PRINTF(pReq, "      // Set width directly with !important to override any cascading styles\n");
    WEB_PRINTF(pReq, "      batteryFill.setAttribute('style', `width: ${percentage}%% !important; background-color: ${percentage > 70 ? '#4caf50' : percentage > 30 ? '#ff9800' : '#ff5252'};`);\n");
    WEB_PRINTF(pReq, "      \n");
    WEB_PRINTF(pReq, "      // Update the battery percentage label as well\n");
    WEB_PRINTF(pReq, "      if (batteryLabel) {\n");
    WEB_PRINTF(pReq, "        batteryLabel.textContent = `${percentage}%%`;\n");
    WEB_PRINTF(pReq, "      }\n");
    WEB_PRINTF(pReq, "    }\n");

    // Update voltage selection with direct ID targeting
    WEB_PRINTF(pReq, "    // Update voltage selection\n");
    WEB_PRINTF(pReq, "    try {\n");
    WEB_PRINTF(pReq, "      const voltage = parseInt(res.voltage, 10);\n");

    WEB_PRINTF(pReq, "      // Reset all checkboxes first\n");
    WEB_PRINTF(pReq, "      const checkboxIds = ['voltage-0', 'voltage-24', 'voltage-30', 'voltage-48', 'voltage-56'];\n");
    WEB_PRINTF(pReq, "      checkboxIds.forEach(id => {\n");
    WEB_PRINTF(pReq, "        const checkbox = document.getElementById(id);\n");
    WEB_PRINTF(pReq, "        if (checkbox) {\n");
    WEB_PRINTF(pReq, "          checkbox.checked = false;\n");
    WEB_PRINTF(pReq, "          const customCheckbox = checkbox.nextElementSibling;\n");
    WEB_PRINTF(pReq, "          if (customCheckbox) {\n");
    WEB_PRINTF(pReq, "            customCheckbox.style.backgroundColor = 'white';\n");
    WEB_PRINTF(pReq, "            customCheckbox.style.borderColor = '#a0a0a0';\n");
    WEB_PRINTF(pReq, "          }\n");
    WEB_PRINTF(pReq, "        }\n");
    WEB_PRINTF(pReq, "      });\n");

    WEB_PRINTF(pReq, "      // Directly select the appropriate checkbox by ID\n");
    WEB_PRINTF(pReq, "      const voltageId = `voltage-${voltage}`;\n");
    WEB_PRINTF(pReq, "      \n");
    WEB_PRINTF(pReq, "      const targetCheckbox = document.getElementById(voltageId);\n");
    WEB_PRINTF(pReq, "      if (targetCheckbox) {\n");
    WEB_PRINTF(pReq, "        targetCheckbox.checked = true;\n");
    WEB_PRINTF(pReq, "        \n");
    WEB_PRINTF(pReq, "        const customCheckbox = targetCheckbox.nextElementSibling;\n");
    WEB_PRINTF(pReq, "        if (customCheckbox) {\n");
    WEB_PRINTF(pReq, "          customCheckbox.style.backgroundColor = '#4caf50';\n");
    WEB_PRINTF(pReq, "          customCheckbox.style.borderColor = '#4caf50';\n");
    WEB_PRINTF(pReq, "        }\n");
    WEB_PRINTF(pReq, "      } else {\n");
    WEB_PRINTF(pReq, "        // Fallback to value-based selection\n");
    WEB_PRINTF(pReq, "        const checkboxes = document.querySelectorAll('.checkbox-group input[type=\"checkbox\"]');\n");
    WEB_PRINTF(pReq, "        checkboxes.forEach(checkbox => {\n");
    WEB_PRINTF(pReq, "          const checkboxValue = parseInt(checkbox.value, 10);\n");
    WEB_PRINTF(pReq, "          \n");
    WEB_PRINTF(pReq, "          if (checkboxValue === voltage) {\n");
    WEB_PRINTF(pReq, "            checkbox.checked = true;\n");
    WEB_PRINTF(pReq, "            \n");
    WEB_PRINTF(pReq, "            const customCheckbox = checkbox.nextElementSibling;\n");
    WEB_PRINTF(pReq, "            if (customCheckbox) {\n");
    WEB_PRINTF(pReq, "              customCheckbox.style.backgroundColor = '#4caf50';\n");
    WEB_PRINTF(pReq, "              customCheckbox.style.borderColor = '#4caf50';\n");
    WEB_PRINTF(pReq, "            }\n");
    WEB_PRINTF(pReq, "          }\n");
    WEB_PRINTF(pReq, "        });\n");
    WEB_PRINTF(pReq, "      }\n");
    WEB_PRINTF(pReq, "    } catch (err) {}\n");
    WEB_PRINTF(pReq, "  } catch (error) {}\n");
    WEB_PRINTF(pReq, "}\n");

    WEB_PRINTF(pReq, "function handleVoltageSelection(event) {\n");
    WEB_PRINTF(pReq, "  const checkboxes = document.querySelectorAll('.checkbox-group input[type=\"checkbox\"]');\n");
    WEB_PRINTF(pReq, "  checkboxes.forEach(checkbox => {\n");
    WEB_PRINTF(pReq, "    if (checkbox !== event.target) {\n");
    WEB_PRINTF(pReq, "      checkbox.checked = false;\n");
    WEB_PRINTF(pReq, "      checkbox.nextElementSibling.style.backgroundColor = 'white';\n");
    WEB_PRINTF(pReq, "      checkbox.nextElementSibling.style.borderColor = '#a0a0a0';\n");
    WEB_PRINTF(pReq, "    }\n");
    WEB_PRINTF(pReq, "  });\n");
    WEB_PRINTF(pReq, "}\n");

    WEB_PRINTF(pReq, "function toggleCheckbox(checkbox) {\n");
    WEB_PRINTF(pReq, "  fetchFlag = false;\n");
    WEB_PRINTF(pReq, "  setTimeout(() => { fetchFlag = true; }, 6000);\n");
    WEB_PRINTF(pReq, "  const box = checkbox.nextElementSibling;\n");
    WEB_PRINTF(pReq, "  if (checkbox.checked) {\n");
    WEB_PRINTF(pReq, "    box.style.backgroundColor = '#4caf50';\n");
    WEB_PRINTF(pReq, "    box.style.borderColor = '#4caf50';\n");
    WEB_PRINTF(pReq, "  } else {\n");
    WEB_PRINTF(pReq, "    box.style.backgroundColor = 'white';\n");
    WEB_PRINTF(pReq, "    box.style.borderColor = '#a0a0a0';\n");
    WEB_PRINTF(pReq, "  }\n");
    WEB_PRINTF(pReq, "}\n");

    WEB_PRINTF(pReq, "async function applyVoltage() {\n");
    WEB_PRINTF(pReq, "  const selectedVoltage = document.querySelector('.checkbox-group input[type=\"checkbox\"]:checked');\n");
    WEB_PRINTF(pReq, "  if (selectedVoltage) {\n");
    WEB_PRINTF(pReq, "    try {\n");
    WEB_PRINTF(pReq, "      const button = document.getElementById('applyButton');\n");
    WEB_PRINTF(pReq, "      button.disabled = true;\n");
    WEB_PRINTF(pReq, "      button.textContent = 'Applying...';\n");

    WEB_PRINTF(pReq, "      let response;\n");
    WEB_PRINTF(pReq, "      if (selectedVoltage.value === 'OFF') {\n");
    WEB_PRINTF(pReq, "        response = await fetch(`http://${ip}/set_0`);\n");
    WEB_PRINTF(pReq, "      } else if (selectedVoltage.value === '24V') {\n");
    WEB_PRINTF(pReq, "        response = await fetch(`http://${ip}/set_24`);\n");
    WEB_PRINTF(pReq, "      } else if (selectedVoltage.value === '30V') {\n");
    WEB_PRINTF(pReq, "        response = await fetch(`http://${ip}/set_30`);\n");
    WEB_PRINTF(pReq, "      } else if (selectedVoltage.value === '48V') {\n");
    WEB_PRINTF(pReq, "        response = await fetch(`http://${ip}/set_48`);\n");
    WEB_PRINTF(pReq, "      } else if (selectedVoltage.value === '56V') {\n");
    WEB_PRINTF(pReq, "        response = await fetch(`http://${ip}/set_56`);\n");
    WEB_PRINTF(pReq, "      }\n");
    WEB_PRINTF(pReq, "      button.disabled = false;\n");
    WEB_PRINTF(pReq, "      button.textContent = 'Apply';\n");

    WEB_PRINTF(pReq, "      if (response.ok) {\n");
    WEB_PRINTF(pReq, "        const displayVoltage = selectedVoltage.value === '56V' ? '52V' : selectedVoltage.value;\n");
    WEB_PRINTF(pReq, "        alert(`Voltage set successfully to ${displayVoltage}`);\n");
    WEB_PRINTF(pReq, "      }\n");
    WEB_PRINTF(pReq, "    } catch (error) {\n");
    WEB_PRINTF(pReq, "      console.error(\"Error setting voltage:\", error);\n");
    WEB_PRINTF(pReq, "      alert(`Failed to set voltage: ${error.message}`);\n");
    WEB_PRINTF(pReq, "    }\n");
    WEB_PRINTF(pReq, "  } else {\n");
    WEB_PRINTF(pReq, "    alert('Please select a voltage option.');\n");
    WEB_PRINTF(pReq, "  }\n");
    WEB_PRINTF(pReq, "}\n");

    WEB_PRINTF(pReq, "function startTimer() {\n");
    WEB_PRINTF(pReq, "  fetchInterval = setInterval(() => {\n");
    WEB_PRINTF(pReq, "    if (fetchFlag) { getPoeData(); }\n");
    WEB_PRINTF(pReq, "  }, 7000);\n");
    WEB_PRINTF(pReq, "}\n");

    WEB_PRINTF(pReq, "document.addEventListener('DOMContentLoaded', function() {\n");
    WEB_PRINTF(pReq, "  // Show loading states\n");
    WEB_PRINTF(pReq, "  document.querySelectorAll('.custom-checkbox').forEach(box => {\n");
    WEB_PRINTF(pReq, "    box.style.backgroundColor = '#f0f0f0';\n");
    WEB_PRINTF(pReq, "    box.style.borderColor = '#cccccc';\n");
    WEB_PRINTF(pReq, "  });\n");

    WEB_PRINTF(pReq, "  // Ensure checkboxes are properly initialized\n");
    WEB_PRINTF(pReq, "  document.querySelectorAll('.checkbox-group input[type=\"checkbox\"]').forEach(checkbox => {\n");
    WEB_PRINTF(pReq, "    checkbox.checked = false;\n");
    WEB_PRINTF(pReq, "  });\n");

    WEB_PRINTF(pReq, "  // Initial data fetch with enough delay to ensure DOM is ready\n");
    WEB_PRINTF(pReq, "  setTimeout(function() {\n");
    WEB_PRINTF(pReq, "    getPoeData();\n");
    WEB_PRINTF(pReq, "  }, 300);\n");

    WEB_PRINTF(pReq, "  // Second fetch as backup\n");
    WEB_PRINTF(pReq, "  setTimeout(function() {\n");
    WEB_PRINTF(pReq, "    getPoeData();\n");
    WEB_PRINTF(pReq, "  }, 1500);\n");

    WEB_PRINTF(pReq, "  // Start periodic updates\n");
    WEB_PRINTF(pReq, "  startTimer();\n");
    WEB_PRINTF(pReq, "});\n");
    WEB_PRINTF(pReq, "</script>\n");

    // HTML Body
    WEB_PRINTF(pReq, "<body>\n");
    WEB_PRINTF(pReq, "<div class=\"container\">\n");
    WEB_PRINTF(pReq, "  <h1 class=\"page-title\">POE Control Panel</h1>\n");

    WEB_PRINTF(pReq, "  <div class=\"dashboard\">\n");

    // Power Status Card
    WEB_PRINTF(pReq, "    <div class=\"card\">\n");
    WEB_PRINTF(pReq, "      <div class=\"card-header\">\n");
    WEB_PRINTF(pReq, "        <h2 class=\"card-title\">Power Status</h2>\n");
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "      <div class=\"card-content\">\n");

    WEB_PRINTF(pReq, "        <div class=\"status-indicator\">\n");
    WEB_PRINTF(pReq, "          <div id=\"ac-light\" class=\"status-light\"></div>\n");
    WEB_PRINTF(pReq, "          <div class=\"status-label\">AC Power: <span id=\"ac-status\">Unknown</span></div>\n");
    WEB_PRINTF(pReq, "        </div>\n");

    WEB_PRINTF(pReq, "        <div class=\"status-indicator\">\n");
    WEB_PRINTF(pReq, "          <div id=\"battery-light\" class=\"status-light\"></div>\n");
    WEB_PRINTF(pReq, "          <div class=\"status-label\">Battery: <span id=\"battery-status\">Unknown</span></div>\n");
    WEB_PRINTF(pReq, "        </div>\n");

    WEB_PRINTF(pReq, "        <div class=\"battery-container\">\n");
    WEB_PRINTF(pReq, "          <div class=\"battery-meter\">\n");
    WEB_PRINTF(pReq, "            <div id=\"battery-fill\" class=\"battery-part\" style=\"width: 0%%;\"></div>\n");
    WEB_PRINTF(pReq, "            <div id=\"battery-label\" class=\"battery-label\">0%%</div>\n");
    WEB_PRINTF(pReq, "          </div>\n");
    WEB_PRINTF(pReq, "        </div>\n");
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "    </div>\n");

    // Voltage Control Card
    WEB_PRINTF(pReq, "    <div class=\"card\">\n");
    WEB_PRINTF(pReq, "      <div class=\"card-header\">\n");
    WEB_PRINTF(pReq, "        <h2 class=\"card-title\">Voltage Control</h2>\n");
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "      <div class=\"card-content\">\n");
    WEB_PRINTF(pReq, "        <div class=\"checkbox-group voltage-options\">\n");

    WEB_PRINTF(pReq, "          <label class=\"voltage-option\">\n");
    WEB_PRINTF(pReq, "            <input type=\"checkbox\" id=\"voltage-56\" value=\"56V\" style=\"display: none;\" onclick=\"handleVoltageSelection(event); toggleCheckbox(this)\">\n");
    WEB_PRINTF(pReq, "            <div class=\"custom-checkbox\" style=\"background-color: #f0f0f0; border-color: #cccccc;\"></div> 52V\n");
    WEB_PRINTF(pReq, "          </label>\n");

    WEB_PRINTF(pReq, "          <label class=\"voltage-option\">\n");
    WEB_PRINTF(pReq, "            <input type=\"checkbox\" id=\"voltage-48\" value=\"48V\" style=\"display: none;\" onclick=\"handleVoltageSelection(event); toggleCheckbox(this)\">\n");
    WEB_PRINTF(pReq, "            <div class=\"custom-checkbox\" style=\"background-color: #f0f0f0; border-color: #cccccc;\"></div> 48V\n");
    WEB_PRINTF(pReq, "          </label>\n");

    WEB_PRINTF(pReq, "          <label class=\"voltage-option\">\n");
    WEB_PRINTF(pReq, "            <input type=\"checkbox\" id=\"voltage-30\" value=\"30V\" style=\"display: none;\" onclick=\"handleVoltageSelection(event); toggleCheckbox(this)\">\n");
    WEB_PRINTF(pReq, "            <div class=\"custom-checkbox\" style=\"background-color: #f0f0f0; border-color: #cccccc;\"></div> 30V\n");
    WEB_PRINTF(pReq, "          </label>\n");

    WEB_PRINTF(pReq, "          <label class=\"voltage-option\">\n");
    WEB_PRINTF(pReq, "            <input type=\"checkbox\" id=\"voltage-24\" value=\"24V\" style=\"display: none;\" onclick=\"handleVoltageSelection(event); toggleCheckbox(this)\">\n");
    WEB_PRINTF(pReq, "            <div class=\"custom-checkbox\" style=\"background-color: #f0f0f0; border-color: #cccccc;\"></div> 24V\n");
    WEB_PRINTF(pReq, "          </label>\n");
    WEB_PRINTF(pReq, "          <label class=\"voltage-option\" style=\"grid-column:1 / -1;\">\n");
    WEB_PRINTF(pReq, "            <input type=\"checkbox\" id=\"voltage-0\" value=\"OFF\" style=\"display: none;\" onclick=\"handleVoltageSelection(event); toggleCheckbox(this)\">\n");
    WEB_PRINTF(pReq, "            <div class=\"custom-checkbox\" style=\"background-color: #f0f0f0; border-color: #cccccc;\"></div> Off\n");
    WEB_PRINTF(pReq, "          </label>\n");
    WEB_PRINTF(pReq, "        </div>\n");
		
		
    WEB_PRINTF(pReq, "        <button id=\"applyButton\" class=\"button\" onclick=\"applyVoltage()\">Apply</button>\n");
    WEB_PRINTF(pReq, "      </div>\n");
    WEB_PRINTF(pReq, "    </div>\n");

    WEB_PRINTF(pReq, "  </div>\n"); // End of dashboard
    WEB_PRINTF(pReq, "</div>\n");   // End of container
    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");

    WEB_END(pReq);
    return SUCCESS;
}
