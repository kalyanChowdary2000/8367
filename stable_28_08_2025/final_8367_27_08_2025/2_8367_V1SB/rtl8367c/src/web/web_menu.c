#pragma STRING(far)

#include <web/web_api.h>
#include <web/web_err.h>
#include <web/http_debug.h>

#include "web_menu.h"

uint8 const far htm_menu[] = "\
<html>\n\
\n\
<head>\n\
<title>Menu</title>\n\
<STYLE type=\"text/css\">\n\
body {\n\
    font-size: 13px;\n\
    font-family: sans-serif, Geneva, Arial, Helvetica;\n\
    background: white;\n\
    font-weight:bold;\n\
    margin: 0;\n\
    padding: 0;\n\
}\n\
.header {\n\
    display: flex;\n\
    align-items: center;\n\
    background-color: #a09c9c;\n\
    padding: 0 20px;\n\
    height: 60px;\n\
}\n\
.logo-container {\n\
    flex: 0 0 auto;\n\
    margin-right: 20px;\n\
}\n\
.logo-container img {\n\
    height: 40px;\n\
    vertical-align: middle;\n\
}\n\
.title-container {\n\
    flex: 1;\n\
    text-align: center;\n\
    color: white;\n\
    font-size: 18px;\n\
    font-weight: bold;\n\
}\n\
.nav-menu {\n\
    background-color: #a09c9c;\n\
    width: 100%;\n\
    height: 40px;\n\
}\n\
.nav-menu ul {\n\
    list-style-type: none;\n\
    margin: 0;\n\
    padding: 0;\n\
    display: flex;\n\
}\n\
.nav-menu > ul > li {\n\
    position: relative;\n\
    display: inline-block;\n\
}\n\
.nav-menu > ul > li > a {\n\
    display: block;\n\
    color: white;\n\
    padding: 12px 16px;\n\
    text-decoration: none;\n\
}\n\
.nav-menu > ul > li:hover > a {\n\
    background-color: #aa0000;\n\
}\n\
.nav-menu ul ul {\n\
    display: none;\n\
    position: absolute;\n\
    top: 40px;\n\
    left: 0;\n\
    background-color: #f9f9f9;\n\
    min-width: 180px;\n\
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);\n\
    z-index: 1;\n\
    flex-direction: column;\n\
}\n\
.nav-menu ul li:hover > ul {\n\
    display: block;\n\
}\n\
.nav-menu ul ul li {\n\
    width: 100%;\n\
}\n\
.nav-menu ul ul li a {\n\
    color: #333;\n\
    padding: 12px 16px;\n\
    text-decoration: none;\n\
    display: block;\n\
    text-align: left;\n\
}\n\
.nav-menu ul ul li a:hover {\n\
    background-color: #ddd;\n\
}\n\
.content-container {\n\
    padding: 20px;\n\
    background-color: #fff;\n\
    margin: 20px auto;\n\
    max-width: 90%;\n\
    border-radius: 5px;\n\
    box-shadow: 0 0 10px rgba(0,0,0,0.1);\n\
}\n\
</STYLE>\n\
</head>\n\
<body>\n\
<div class=\"header\">\n\
    <div class=\"logo-container\">\n\
        <a href='http://www.opplin.com' target='_blank'>\n\
        <img src='/logo.png' border=0 alt='http://www.opplin.com'></a>\n\
    </div>\n\
    <div class=\"title-container\">SMART PoE</div>\n\
</div>\n\
<div class=\"nav-menu\">\n\
    <ul>\n\
        <li><a href=\"poe.cgi\" target=\"contentFrame\">Poe Setting</a></li>\n\
        <li><a href=\"info.cgi\" target=\"contentFrame\">Information</a></li>\n\
        <li><a href=\"ip.cgi\" target=\"contentFrame\">IP Setting</a></li>\n\
        <li><a href=\"vlan.cgi?page=static\" target=\"contentFrame\">Static VLAN</a></li>\n\
        <li><a href=\"vlan.cgi?page=port_based\" target=\"contentFrame\">VLAN Setting</a></li>\n\
        <li><a href=\"vlan.cgi?page=vlan_ip\" target=\"contentFrame\">VLAN IP</a></li>\n\
        <li><a href=\"user.cgi\" target=\"contentFrame\">User Account</a></li>\n\
        <li><a href=\"snmpsecurity.cgi?page=snmpsec\" target=\"contentFrame\">SNMP Settings</a></li>\n\
        <li><a href=\"#\">Tools</a>\n\
            <ul>\n\
                <li><a href=\"info.cgi?page=ping\" target=\"contentFrame\">IP Diagnostic</a></li>\n\
                <li><a href=\"fwug.cgi\" target=\"contentFrame\">Firmware Upgrade</a></li>\n\
                <li><a href=\"config_back.cgi\" target=\"contentFrame\">Configuration Backup</a></li>\n\
                <li><a href=\"reset.cgi\" target=\"contentFrame\">Reset</a></li>\n\
                <li><a href=\"save.cgi\" target=\"contentFrame\">Save</a></li>\n\
                <li><a href=\"reboot.cgi\" target=\"contentFrame\">Reboot</a></li>\n\
            </ul>\n\
        </li>\n\
    </ul>\n\
</div>\n\
<div class=\"content-container\">\n\
    <iframe id=\"contentFrame\" name=\"contentFrame\" src=\"poe.cgi\" style=\"width:100%; height:600px; border:none;\"></iframe>\n\
</div>\n\
</body>\n\
</html>\n\
";

int8 get_menu(struct http_request_s *pReq)
{
    DBG_LWPS_HTTP_PRINTF(("-------get_menu()-------\n"));

    WEB_BEGIN(pReq);

    WEB_STATUS(pReq, HTTP_STATUS_OK);
    WEB_HEADER(pReq, "Content-Type: text/html");

    WEB_DATA(pReq, htm_menu, sizeof(htm_menu) - 1);

    WEB_END(pReq);

    return SUCCESS;
}