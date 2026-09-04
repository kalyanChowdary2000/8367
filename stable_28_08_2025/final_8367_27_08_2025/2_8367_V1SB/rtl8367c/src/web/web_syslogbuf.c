#pragma STRING (far)
/*
 * Copyright(c) Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : The handler of HTTP request to IP function
 *
 * Feature : Response the request related to IP function
 *
 */

/*
 * Include Files
 */
#include <string.h>
#include <stdio.h>

#include <salutil.h>
#include <salsys.h>
#include <salflash.h>
#include <flash_config.h>
#include <syslog/syslog.h>
#include <syslog/syslogdefs.h>
#include <sal/salsyslog.h>

#include <web/web_api.h>
#include "web_ip.h"
#include "web_util.h"   
#include <lib/lib_time.h>

#if 1
/*
 * Symbol Definition
 */
extern volatile uint32 data current_time;

/*
 * Data Declaration
 */

/*
 * Function Declaration
 */

static void int_to_str_syslog_severity(uint8 severity, struct http_request_s *pReq)
{
	switch(severity)
	{
		case SYSLOG_SEV_EMERG:
			 WEB_PRINTF(pReq, "    <td width=\"15\">%s</td>\n", "Emergency");
			break;
		case SYSLOG_SEV_ALERT:
			WEB_PRINTF(pReq, "	  <td width=\"15\">%s</td>\n", "Alert");
			break;
		case SYSLOG_SEV_ERR:
			WEB_PRINTF(pReq, "	  <td width=\"15\">%s</td>\n", "Error");
			break;
		case SYSLOG_SEV_CRIT:
			WEB_PRINTF(pReq, "	  <td width=\"15\">%s</td>\n", "Critical");
			break;
		case SYSLOG_SEV_WARNING:
			WEB_PRINTF(pReq, "	  <td width=\"15\">%s</td>\n", "Warning");
			break;
		case SYSLOG_SEV_NOTICE:
			WEB_PRINTF(pReq, "	  <td width=\"15\">%s</td>\n", "Notice");
			break;
		case SYSLOG_SEV_INFO:
			WEB_PRINTF(pReq, "	  <td width=\"15\">%s</td>\n", "Info");
			break;
		case SYSLOG_SEV_DEBUG:
			WEB_PRINTF(pReq, "	  <td width=\"15\">%s</td>\n", "Debug");
			break;
		default:
			break;
	}
	return;
}

int32 get_syslogbufsec(struct http_request_s *pReq)
{
	sal_enable_t	syslogStatus = 0;
	uint16 headIndex = 0;
	uint16 tailIndex = 0;
	sal_flash_syslog_buf_entry_t sysLogBufEntry;
	uint16 logId = 1;
	struct tm ts;
	uint32 timestamp;
	char timeStr[30]={""};

//	syslog_head_idx_get(&headIndex);	
//	syslog_head_idx_get(&tailIndex);
	sal_syslog_buf_head_idx_get(&headIndex);
	sal_syslog_buf_tail_idx_get(&tailIndex);

    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
	WEB_HEADER(pReq, "Content-Type: text/html");

	WEB_PRINTF(pReq, "<html>\n");
    WEB_PRINTF(pReq, "\n");
	WEB_PRINTF(pReq, "<head>\n");
	WEB_PRINTF(pReq, "<title>Syslog Buffer</title>\n");
	WEB_PRINTF(pReq, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\">\n");
	WEB_PRINTF(pReq, "</head>\n");
	WEB_PRINTF(pReq, "\n");
	WEB_PRINTF(pReq, "<body>\n");
	WEB_PRINTF(pReq, "<center>\n");
	WEB_PRINTF(pReq, "\n");
	WEB_PRINTF(pReq, "<fieldset>\n");
	WEB_PRINTF(pReq, "<legend>System Logs</legend>\n");
    WEB_PRINTF(pReq, "<form method=\"post\" name=\"syslogbufsec\" action=\"/syslogbuf.cgi?page=syslogbufsec\">\n");

	if(headIndex == tailIndex)
	{
		WEB_PRINTF(pReq, "<legend>No Syslog message to show..</legend>\n");
	}
	else
	{
		WEB_PRINTF(pReq, "<table border=\"1\">\n");

		WEB_PRINTF(pReq, "	<tr>\n");
		WEB_PRINTF(pReq, "	  <th width=\"200\">Syslog Clear</th>\n");

		WEB_PRINTF(pReq, "		<td width=\"150\">\n");
		WEB_PRINTF(pReq, "		  <select name=\"state\" style=\"width:150\">\n");

		WEB_PRINTF(pReq, "		<option value=\"0\" selected >No \n");				  
		WEB_PRINTF(pReq, "		<option value=\"1\" >Yes \n");

		WEB_PRINTF(pReq, "		  </select>\n");
		WEB_PRINTF(pReq, "		</td>\n");

		WEB_PRINTF(pReq, "	<tr>\n");
		WEB_PRINTF(pReq, "</table>\n");

		WEB_PRINTF(pReq, "	<input type=\"submit\" value=\"   Apply   \">\n");
		WEB_PRINTF(pReq, "	<input type=\"hidden\" name=\"cmd\" value=\"syslogbuf\">\n");
		WEB_PRINTF(pReq, "	<br style=\"line-height:50%%\">\n");


		WEB_PRINTF(pReq, "<br>\n");
		WEB_PRINTF(pReq, "<table border=\"1\">\n");

	//	WEB_PRINTF(pReq, "	  <th width=\"15\">%d</th>\n", (uint16)headIndex);
	//	WEB_PRINTF(pReq, "	  <th width=\"15\">%d</th>\n", (uint16)tailIndex);
		WEB_PRINTF(pReq, "	  <th width=\"15\">ID</th>\n");
		WEB_PRINTF(pReq, "	  <th width=\"45\">Time</th>\n");
		WEB_PRINTF(pReq, "	  <th width=\"15\">Severity</th>\n");
		WEB_PRINTF(pReq, "	  <th width=\"450\">Message Content</th>\n");
	//headIndex = 0;
//	tailIndex = 1;


		while(headIndex != tailIndex)
		{
			memset(&sysLogBufEntry, 0x00, sizeof(sal_flash_syslog_buf_entry_t));
			sal_flash_syslog_buf_get(&sysLogBufEntry, headIndex);
			timestamp = sysLogBufEntry.timestamp;

			// Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
			calcDate(&ts, timestamp);
			sprintf(timeStr, "%s %d %02d:%02d:%02d	  \0", months[ts.tm_mon], (uint16)ts.tm_mday, (uint16)ts.tm_hour, (uint16)ts.tm_min, (uint16)ts.tm_sec);

			WEB_PRINTF(pReq, "  <tr>\n");
	//		WEB_PRINTF(pReq, "    <td width=\"15\">%d</td>\n", (uint16)sysLogBufEntry.sysLogIdBuf);
			WEB_PRINTF(pReq, "    <td width=\"15\">%d</td>\n", (uint16)logId);
			WEB_PRINTF(pReq, "    <td width=\"45\">%s</td>\n", timeStr);
			int_to_str_syslog_severity(sysLogBufEntry.sysLogSevBuf, pReq);

			if(sysLogBufEntry.sysLogIdBuf == SYSLOG_MSG_PORT_LINK_DOWN || sysLogBufEntry.sysLogIdBuf == SYSLOG_MSG_PORT_LINK_UP)
			{
				WEB_PRINTF(pReq, "    <td width=\"450\">Port %d %s</td>\n", (uint16)sysLogBufEntry.addInfo, syslogmsgcontent[sysLogBufEntry.sysLogIdBuf]);
			}
			else
			{
				WEB_PRINTF(pReq, "    <td width=\"450\">%s</td>\n", syslogmsgcontent[sysLogBufEntry.sysLogIdBuf]);
			}
			
	//		WEB_PRINTF(pReq, "    <td align=\"left\" style=\"width:250px;\">%s</td align=\"center\">\n", "HAPPY");
			WEB_PRINTF(pReq, "  </tr>\n");
			logId++;
			headIndex++;
			if(headIndex == SAL_SYS_LOG_BUF_MAX_ENTRY)
			{
				headIndex = 0; //wrap around
			}
		}
		WEB_PRINTF(pReq, "</table>\n");
	}

	WEB_PRINTF(pReq, "</form>\n");
	WEB_PRINTF(pReq, "</fieldset>\n");
	WEB_PRINTF(pReq, "</center>\n");
	WEB_PRINTF(pReq, "</body>\n");
	WEB_PRINTF(pReq, "</html>\n");

    WEB_END(pReq);

    return SUCCESS;

}

int32 post_syslogbufsec(struct http_request_s *pReq)
{
    uint8           *pValue;
	sal_enable_t	syslogBufClear = 0;

    WEB_VAR(pReq, "state", &pValue);
    if (pValue) {
        syslogBufClear = atoi(pValue);
    }
	
    if (syslogBufClear == 1)
    {
		sal_sys_syslog_headIdx_set(DFLT_SYSLOG_BUF_HEAD_IDX, 1);
		sal_sys_syslog_tailIdx_set(DFLT_SYSLOG_BUF_TAIL_IDX, 1);

		//Clear buffer
		sal_syslog_buf_head_idx_set(DFLT_SYSLOG_BUF_HEAD_IDX);	
		sal_syslog_buf_tail_idx_set(DFLT_SYSLOG_BUF_TAIL_IDX);
		
		sal_flash_cfg_save();

		printmsg(pReq, "Syslog Messages Cleared..");
    }
      
    get_syslogbufsec(pReq);

    return SUCCESS;
}
#endif

