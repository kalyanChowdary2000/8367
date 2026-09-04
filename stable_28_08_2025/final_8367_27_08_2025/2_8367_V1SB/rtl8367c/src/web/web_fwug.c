#pragma STRING (far)
/*
 * Copyright(c) Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : The handler of HTTP request to TFTP function
 *
 * Feature : Response the request related to TFTP function
 *
 */


/*
 * Include Files
 */
#include <string.h>
#include <stdio.h>

#include <salsys.h>
#include <web/http_types.h>
#include <web/web_api.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

uint8 const far htm_loader[] = "\
<html><head><title>FW</title><style>body{font:12px Arial;margin:0}fieldset{width:320px;border:1px solid #ddd;padding:5px;margin:10px auto}legend{font:bold 12px Arial}input[type=submit]{background:#1e88e5;color:#fff;border:0;padding:4px 8px;cursor:pointer}</style>\
<script>function c(s){return confirm('Enter Loader Mode?')}</script>\
<body><center>\
<fieldset><legend>Firmware</legend>\
<form method=post action=fwug.cgi onSubmit=\"return c(this)\">\
<div>Enter loader mode to upgrade firmware</div><p>\
<input type=submit value=\"Enter Loader Mode\">\
<input type=hidden name=cmd value=enter_loader>\
</form></fieldset>\
</center></body></html>\n\
";

uint8 const far htm_enter_loader[] = "\
<html><head><title>Loader</title><style>body{font:12px Arial;margin:0}fieldset{width:320px;border:1px solid #ddd;padding:5px;margin:10px auto}legend{font:bold 12px Arial}p{height:5px;margin:10px 0;background:#eee}b{height:100%;width:0;background:#1e88e5;display:block;animation:f 3s forwards}@keyframes f{to{width:100%}}</style>\
<script>setTimeout(\"top.location.reload()\",3000)</script>\
<body><center>\
<fieldset><legend>Loader</legend>\
<div>Entering Loader Mode...</div>\
<p><b></b></p>\
</fieldset>\
</center></body></html>\n\
";

/*
 * Function Declaration
 */
int32 get_fwug(struct http_request_s *pReq)
{
    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");
    WEB_DATA(pReq, htm_loader, sizeof(htm_loader));
    WEB_END(pReq);
    return SUCCESS;
}

int32 post_fwug(struct http_request_s *pReq)
{
    WEB_BEGIN(pReq);
    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");
    WEB_DATA(pReq, htm_enter_loader, sizeof(htm_enter_loader));
    WEB_END(pReq);
    sal_sys_loaderMode_set();
    return SUCCESS;
}

