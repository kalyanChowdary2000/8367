/*
 * Copyright(c) Realtek Semiconductor Corporation, 2009
 * All rights reserved.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : The handler of HTTP request to forwarding function
 *
 * Feature : Response the related request of forwarding function
 *
 */
#pragma STRING(far)
#include <string.h>
#include <stdio.h>

#include <salutil.h>
#include <salflash.h>
#include <salsys.h>

#include <web/web_api.h>
#include "web_util.h"

#if __WEB_CFG_BACKUP__
/*
 * Symbol Definition
 */
//#define DBG_HTTP_RESTORE printf
//#define DBG_HTTP_UPDATE  printf
#define DBG_HTTP_RESTORE
#define DBG_HTTP_UPDATE 


/*
 * Data Declaration
 */
static http_image_stat_t g_firm_update_stat = HTTP_IMAGE_IDLE;
static int32 http_upload_result = 0;    /* for conf_restore */
static int32 http_upload_last_block = 0;    /* for conf_restore */

uint8 const far htm_config[] = "\
<html><head><title>Configuration Backup/Restore</title><style>\
body{font:13px Arial;background:#f9f9f9;margin:0;padding:20px 0;text-align:center}\
fieldset{width:380px;border:1px solid #ddd;padding:20px;margin:15px auto;background:#fff;border-radius:4px;box-shadow:0 1px 4px rgba(0,0,0,0.05)}\
legend{font:bold 16px Arial;color:#1e88e5;padding:0 10px}\
input[type=file]{border:1px solid #ddd;padding:8px;width:80%;margin:5px auto;border-radius:3px}\
input[type=submit]{background:#1e88e5;color:#fff;border:0;padding:8px 16px;cursor:pointer;border-radius:3px;font-weight:bold;margin-top:10px}\
</style>\
<script>\
function conf_backup(){window.location.href=\"/config_back.cgi?cmd=conf_backup\";}\
function CfmFwUpg(s){return confirm('It must reboot your device for the upgrade to take effect, continue?');}\
</script></head>\
<body>\
<fieldset>\
<legend>Backup Configuration</legend>\
<input type=\"submit\" value=\"Backup\" onclick=\"conf_backup();\">\
</fieldset>\
<fieldset>\
<legend>Restore Configuration</legend>\
<form method=\"post\" enctype=\"multipart/form-data\" action=\"/config_back.cgi?cmd=conf_restore\">\
<input type=\"file\" name=\"file\" size=\"60\" maxlength=\"128\">\
<div><input type=\"submit\" value=\"Restore\"></div>\
</form>\
</fieldset>\
</body></html>\n\
";

uint8 const far htm_must_reboot[] = "\
<html><head><title>Configuration Restored</title><style>\
body{font:13px Arial;background:#f9f9f9;margin:0;padding:25px 0;text-align:center}\
fieldset{width:380px;border:1px solid #ddd;padding:20px;margin:0 auto;background:#fff;border-radius:4px;box-shadow:0 1px 4px rgba(0,0,0,0.05)}\
legend{font:bold 16px Arial;color:#1e88e5;padding:0 10px}\
.msg{color:#444;margin:10px 0;line-height:1.5}\
.note{color:#4caf50;font-weight:bold}\
</style></head>\
<body>\
<fieldset>\
<legend>Configuration Restored</legend>\
<div class=\"msg\">Configuration has been restored successfully.</div>\
<div class=\"note\">Please reboot your device for changes to take effect.</div>\
</fieldset>\
</body></html>\n\
";

uint8 const far htm_conf_restore_failed[] = "\
<html><head><title>Restore Failed</title><style>\
body{font:13px Arial;background:#f9f9f9;margin:0;padding:25px 0;text-align:center}\
fieldset{width:380px;border:1px solid #ddd;padding:20px;margin:0 auto;background:#fff;border-radius:4px;box-shadow:0 1px 4px rgba(0,0,0,0.05)}\
legend{font:bold 16px Arial;color:#1e88e5;padding:0 10px}\
.error{color:#f44336;margin:10px 0;font-weight:bold}\
</style></head>\
<body>\
<fieldset>\
<legend>Restore Failed</legend>\
<div class=\"error\">Configuration restore failed!</div>\
<div>The uploaded file may be invalid or corrupted.</div>\
</fieldset>\
</body></html>\n\
";

int32 get_http_config(http_request_t *pReq)
{
    uint8  buf[32];
    uint8  *pValue = NULL;
    uint8  *configPtr;
    uint16 configSize;    

    /* check command */
    WEB_VAR(pReq, "cmd", &pValue);
    if (pValue) {
        if (strcmp(pValue, "conf_backup") == 0) {
            printf("back up\n");
            WEB_BEGIN(pReq);
            
            WEB_STATUS(pReq, HTTP_STATUS_OK);
            WEB_HEADER(pReq, "Pragma: no-cache");
            
            sal_flash_cfgSize_get(&configSize);
            sprintf(buf, "Content-Length: %d", configSize);
        
            WEB_HEADER(pReq, buf);
            WEB_HEADER(pReq, "Content-Type: application/octet-stream; name=switch_cfg.bin");
            WEB_HEADER(pReq, "Content-Disposition: attachment; filename=switch_cfg.bin");
            WEB_HEADER(pReq, "Connection: close");

            sal_flash_cfgAddr_get(&configPtr);
            WEB_DATA(pReq, configPtr, configSize);

            WEB_END(pReq);
        } else {
            WEB_BEGIN(pReq);
            
            WEB_STATUS(pReq, HTTP_STATUS_NOT_FOUND);
            
            WEB_END(pReq);
        }
    }
    else
    {
        WEB_BEGIN(pReq);
        
        WEB_STATUS(pReq, HTTP_STATUS_OK);
        WEB_HEADER(pReq, "Content-Type: text/html");
        
        WEB_DATA(pReq, htm_config, sizeof(htm_config));
        
        WEB_END(pReq);
    }

    return SUCCESS;
}

int32 post_http_restore(http_request_t *pReq)
{
    uint16                      config_size;
    sal_flash_configValidity_t  isValid;

    /* write data into flash */
    if (0 == pReq->upload_offset)
    {
        /* first block */
        /* check the mark (do not consider breaking) */
        sal_flash_cfgValidity_get(pReq->upload_data,&isValid);
        if (SAL_FLASH_CONFIG_VALID == isValid)
        {
            http_upload_result = 0;     /* init: Normal state */

            sal_flash_cfg_set(0, pReq->upload_len, pReq->upload_data);
        }
        else
        {
            /* error (signature word is wrong) */
            http_upload_result = -1;   /* WRONG SIGNATURE */

            /*show failed webpage*/
            WEB_BEGIN(pReq);
            WEB_STATUS(pReq, HTTP_STATUS_OK);
            WEB_HEADER(pReq, "Content-Type: text/html");
            WEB_DATA(pReq, htm_conf_restore_failed, sizeof(htm_conf_restore_failed));
            WEB_END(pReq);
            
            return SUCCESS;
        }
    }
    else
    {
        if(http_upload_result != -1)
        {
            if (pReq->upload_offset < sizeof(ConfigCache) && (0 == http_upload_result))
            {
                if ((pReq->upload_offset + pReq->upload_len) >= sizeof(ConfigCache)) 
                {
                    /* last block */
                    http_upload_result = 1;    /* OK */

                    sal_flash_cfgSize_get(&config_size);
                    sal_flash_cfg_set(pReq->upload_offset, config_size - pReq->upload_offset, pReq->upload_data);
                    if(pReq->upload_len > (config_size - pReq->upload_offset))
                    {
                        http_upload_last_block = 1;
                    }
                }
                else
                {
                    sal_flash_cfg_set(pReq->upload_offset, pReq->upload_len, pReq->upload_data);
                }
            }
            else
            {
                http_upload_last_block = 1;
            }
        }
    }

    /* show webpage at ending */
    if (http_upload_last_block)
    {
        http_upload_last_block = 0;

        WEB_BEGIN(pReq);
                
        WEB_STATUS(pReq, HTTP_STATUS_OK);
        WEB_HEADER(pReq, "Content-Type: text/html");

        if (0 < http_upload_result)
        {
            sal_flash_cfg_save();
            WEB_DATA(pReq, htm_must_reboot, sizeof(htm_must_reboot));
        }
        else if (0 >= http_upload_result)
        {
            WEB_DATA(pReq, htm_conf_restore_failed, sizeof(htm_conf_restore_failed));
        }

        WEB_END(pReq);
    }

    return SUCCESS;
}

#endif    


