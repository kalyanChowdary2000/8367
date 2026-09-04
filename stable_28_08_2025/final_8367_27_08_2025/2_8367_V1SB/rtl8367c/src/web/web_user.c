#pragma STRING (far)
/*
 * Copyright(c) Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : The handler of HTTP request to user function
 *
 * Feature : Response the request related to user function
 *
 */

/*
 * Include Files
 */
#include <string.h>
#include <stdio.h>

#include <salflash.h>
#include <salsys.h>

#include <web/web_api.h>
#include "web_user.h"
#include <syslog/syslogdefs.h>
char m_u8_uart_data[64];
/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */


/*
 * Function Declaration
 */
int32 get_user(struct http_request_s *pReq)
{
    uint8 username[SAL_FLSH_USR_USERNAME_LEN + 1];
    uint8 password[SAL_FLSH_USR_PASSWORD_LEN + 1];

    sal_flash_sysAccount_get(username, password);

    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_PRINTF(pReq, "<html>\n");
    WEB_PRINTF(pReq, "<head>\n");
    WEB_PRINTF(pReq, "<title>User Account Setting</title>\n");
    
    // Minimal CSS
    WEB_PRINTF(pReq, "<style>\n");
    WEB_PRINTF(pReq, "body{font-family:Arial,sans-serif;background:#f4f7fa;margin:0;padding:20px;}\n");
    WEB_PRINTF(pReq, "h1{text-align:center;color:#333;margin-bottom:20px;}\n");
    WEB_PRINTF(pReq, ".card{background:white;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,.1);padding:20px;max-width:500px;margin:0 auto;}\n");
    WEB_PRINTF(pReq, ".form-group{margin-bottom:15px;}\n");
    WEB_PRINTF(pReq, "label{display:block;margin-bottom:5px;font-weight:bold;color:#555;}\n");
    WEB_PRINTF(pReq, "input[type=text],input[type=password]{width:100%%;padding:8px;border:1px solid #ddd;border-radius:4px;box-sizing:border-box;}\n");
    WEB_PRINTF(pReq, ".btn{background:#1e88e5;color:white;border:none;padding:10px 15px;border-radius:4px;cursor:pointer;font-size:14px;}\n");
    WEB_PRINTF(pReq, ".btn:hover{background:#1565c0;}\n");
    WEB_PRINTF(pReq, "</style>\n");
    
    // Simplified JavaScript validation
    WEB_PRINTF(pReq, "<script>\n");
    WEB_PRINTF(pReq, "function ConfirmAccount(){\n");
    WEB_PRINTF(pReq, "var form=document.accountform;\n");
    WEB_PRINTF(pReq, "if(form.mpass.value!=form.mpass2.value){alert(\"Passwords do not match.\");return false;}\n");
    WEB_PRINTF(pReq, "return true;\n");
    WEB_PRINTF(pReq, "}\n");
    WEB_PRINTF(pReq, "</script>\n");
    WEB_PRINTF(pReq, "</head>\n");
    
    // Modern body
    WEB_PRINTF(pReq, "<body>\n");
    WEB_PRINTF(pReq, "<h1>User Account Setting</h1>\n");
    WEB_PRINTF(pReq, "<div class=\"card\">\n");
    WEB_PRINTF(pReq, "<form method=\"post\" name=\"accountform\" action=\"user.cgi\" onSubmit=\"return ConfirmAccount()\">\n");
    
    // Username field
    WEB_PRINTF(pReq, "<div class=\"form-group\">\n");
    WEB_PRINTF(pReq, "<label for=\"username\">New Username</label>\n");
    WEB_PRINTF(pReq, "<input type=\"text\" id=\"username\" name=\"mname\" maxlength=\"16\" value=\"%s\">\n", username);
    WEB_PRINTF(pReq, "</div>\n");
    
    // Password field
    WEB_PRINTF(pReq, "<div class=\"form-group\">\n");
    WEB_PRINTF(pReq, "<label for=\"password\">New Password</label>\n");
    WEB_PRINTF(pReq, "<input type=\"password\" id=\"password\" name=\"mpass\" maxlength=\"%d\">\n", (uint16)SAL_FLSH_USR_PASSWORD_LEN);
    WEB_PRINTF(pReq, "</div>\n");
    
    // Confirm Password field
    WEB_PRINTF(pReq, "<div class=\"form-group\">\n");
    WEB_PRINTF(pReq, "<label for=\"confirmPassword\">Confirm Password</label>\n");
    WEB_PRINTF(pReq, "<input type=\"password\" id=\"confirmPassword\" name=\"mpass2\" maxlength=\"%d\">\n", (uint16)SAL_FLSH_USR_PASSWORD_LEN);
    WEB_PRINTF(pReq, "</div>\n");
    
    // Submit button
    WEB_PRINTF(pReq, "<div style=\"text-align:center;margin-top:20px;\">\n");
    WEB_PRINTF(pReq, "<input type=\"submit\" class=\"btn\" value=\"Apply Changes\">\n");
    WEB_PRINTF(pReq, "<input type=\"hidden\" name=\"cmd\" value=\"passwd\">\n");
    WEB_PRINTF(pReq, "</div>\n");
    
    WEB_PRINTF(pReq, "</form>\n");
    WEB_PRINTF(pReq, "</div>\n");
    WEB_PRINTF(pReq, "</body>\n");
    WEB_PRINTF(pReq, "</html>\n");

    WEB_END(pReq);

    return SUCCESS;
}


int32 post_user(struct http_request_s *pReq)
{
    uint8  *pValue;
    uint8  username[SAL_FLSH_USR_USERNAME_LEN + 1];
    uint8  password[SAL_FLSH_USR_PASSWORD_LEN + 1];

    /* get variables */
    WEB_VAR(pReq, "mname", &pValue);
    if (pValue) {
        //memcpy(username, pValue, SAL_FLSH_USR_USERNAME_LEN);
        strcpy(username, pValue);
    }
    
    WEB_VAR(pReq, "mpass", &pValue);
    if (pValue) {
        //memcpy(password, pValue, SAL_FLSH_USR_PASSWORD_LEN);
        strcpy(password, pValue);
    }

    /* Save user account */
    sal_flash_sysAccount_set(username, password);

	syslog_add_new_buf_entry(SYSLOG_MSG_PASSWORD_CHANGED, 0);
		
    get_user(pReq);

    return SUCCESS;
}

int32 get_poe_status(struct http_request_s *pReq)
{	
    WEB_BEGIN(pReq);	
				WEB_STATUS(pReq, HTTP_STATUS_OK);
				WEB_HEADER(pReq, "Content-Type: application/json");
				WEB_PRINTF(pReq, "%s",m_u8_uart_data);
    WEB_END(pReq);
	  //printf("get_STATUS;\r\n");
    return SUCCESS;
}
void send_str_web(char* tempData)
{
	strcpy(m_u8_uart_data, tempData);
}
int32 set_24(struct http_request_s *pReq)
{	
    WEB_BEGIN(pReq);


    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: application/json");
	
	

		//WEB_PRINTF(pReq, "{\"ac\":%d,\"battery\":%d,\"batteryPercentage\":%d,\"mode\":%d}",m_int_ac,m_int_battery,m_int_batteryPer,m_int_mode);
//    WEB_PRINTF(pReq, "{\"ac\":1,\"battery\":0,\"batteryPercentage\":50,\"mode\":2}");
		WEB_PRINTF(pReq, "{\"voltage\":24}"); 
    WEB_END(pReq);
		
		printf("set_POE_VOLT:24;\r\n");

    return SUCCESS;
}
int32 set_30(struct http_request_s *pReq)
{	
    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: application/json");
	
	

	//	WEB_PRINTF(pReq, "{\"ac\":%d,\"battery\":%d,\"batteryPercentage\":%d,\"mode\":%d}",m_int_ac,m_int_battery,m_int_batteryPer,m_int_mode);
//    WEB_PRINTF(pReq, "{\"ac\":1,\"battery\":0,\"batteryPercentage\":50,\"mode\":2}");
  WEB_PRINTF(pReq, "{\"voltage\":30}");
    WEB_END(pReq);
	
	//	get_details_from_fmd();
	
	printf("set_POE_VOLT:30;\r\n");

    return SUCCESS;
}
int32 set_48(struct http_request_s *pReq)
{	
    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: application/json");
	
	

		//WEB_PRINTF(pReq, "{\"ac\":%d,\"battery\":%d,\"batteryPercentage\":%d,\"mode\":%d}",m_int_ac,m_int_battery,m_int_batteryPer,m_int_mode);
//    WEB_PRINTF(pReq, "{\"ac\":1,\"battery\":0,\"batteryPercentage\":50,\"mode\":2}");
  WEB_PRINTF(pReq, "{\"voltage\":48}");
    WEB_END(pReq);
	printf("set_POE_VOLT:48;\r\n");
	//	get_details_from_fmd();

    return SUCCESS;
}
int32 set_56(struct http_request_s *pReq)
{	
    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: application/json");
	   WEB_PRINTF(pReq, "{\"voltage\":56}");
    WEB_END(pReq);
	printf("set_POE_VOLT:56;\r\n");
    return SUCCESS;
}

int32 set_0(struct http_request_s *pReq)
{	
    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: application/json");
	  WEB_PRINTF(pReq, "{\"voltage\":0}");
    WEB_END(pReq);
		printf("set_POE_VOLT:0;\r\n");
    return SUCCESS;
}