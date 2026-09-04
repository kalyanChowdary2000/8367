#pragma STRING (far)
#include <local/config.h>
#include <reg_sfr.h>
#include <stdio.h>
#include <string.h>

#include <salutil.h>
#include <salsys.h>
#include <salflash.h>

#include "common.h"
#include "asicreg.h"
#include "web_user.h"


extern void verify_get_poe_data(const char* temp_data);
/* Variables Declaration */
extern void process_uart_string(void);
extern void send_str_web(char* tempData);
extern void update_json_string(const char *new_json);
/*
uint32 xdata s1;
uint32 xdata s2;
uint32 xdata s3;

uint32 vic_rand32(void){
	#define TAUSWORTHE(s,a,b,c,d) ((s&c)<<d) ^ (((s <<a) ^ s)>>b)
	s1 = TAUSWORTHE(s1, 13, 19, 4294967294UL, 12);
	s2 = TAUSWORTHE(s2, 2, 25, 4294967288UL, 4);
	s3 = TAUSWORTHE(s3, 3, 11, 4294967280UL, 17);
	
	return ( (s1^s2^s3) );
}


uint8 vic_rand8(void){
	return (vic_rand32()%0xFF);
}

void print_buf(uint8 *buf, uint32 len, char *msg) large{
	uint16 xdata i;
	int    xdata line_size = 8;
	
	if( NULL!=msg ){
		OSAL_PRINTF("%s\n", msg);
	}
	OSAL_PRINTF("[Begin:0x%04x_0x0000]    0     1     2     3     4     5     6     7", (uint16)buf);
	for(i=0;i<len;i++, buf++){
		if( 0==(i%8) ){
			OSAL_PRINTF("\n      [0x%04x_0x%04x]:", (uint16)buf, i);
		}
		OSAL_PRINTF("0x%02x  ", (uint16)(*buf));
	}
	OSAL_PRINTF("\n");
}

void print_fcbuf(uint8 const far *buf, uint32 len, char *msg) large{
	uint16 xdata i;
	int    xdata line_size = 8;
	
	if( NULL!=msg ){
		OSAL_PRINTF("%s\n", msg);
	}
	OSAL_PRINTF("[Begin:0x%04x_0x0000]    0     1     2     3     4     5     6     7", (uint16)buf);
	for(i=0;i<len;i++, buf++){
		if( 0==(i%8) ){
			OSAL_PRINTF("\n      [0x%04x_0x%04x]:", (uint16)buf, i);
		}
		OSAL_PRINTF("0x%02x  ", (uint16)(*buf));
	}
	OSAL_PRINTF("\n");
}
*/

void print_menu(void) large{
	OSAL_PRINTF("\n     ****************    %s    ****************\n", PROJECT_NAME);
	OSAL_PRINTF("     %c: Clear Screen\n", CLI_COMMAND_CLEAR);
    OSAL_PRINTF("     %c: Print IP\n", CLI_COMMAND_DUMP_IP);
	OSAL_PRINTF("     %c: Help Menu\n", CLI_COMMAND_HELP);
	OSAL_PRINTF("     %c: Enter Loader Mode\n", CLI_COMMAND_UPDATE);
    OSAL_PRINTF("     %c: Set MAC Address\n", CLI_COMMAND_MAC_SET);
    OSAL_PRINTF("     %c: Reboot\n", CLI_COMMAND_REBOOT);    
	OSAL_PRINTF("     ************************************************\n\n");
}

#if 0
static uint8 hex2dec(uint8 value) large{
	if( ('0'<=value) && ('9'>=value) ){
		return (value-'0');
	}else if( ('a'<=value) && ('f'>=value) ){
		return (value - 'a' + 10);
	}else if( ('A'<=value) && ('F'>=value) ){
		return (value - 'A' + 10);
	}
}
#endif

#if 0 /* function:getAddress */
/* NO ERROR CHECK */
uint32 getAddress(void) large{
	uint32 xdata addr;
	uint16 xdata i;
	uint8  xdata input_buf[32];
	
	OSAL_PRINTF("target address:");
	for(i=0;i<32;i++){
		input_buf[i] = getchar();
		if( '\n'==input_buf[i] || '\r'==input_buf[i] ){
			input_buf[i] = '\0';
			putchar('\n');
			break;
		}
		putchar(input_buf[i]);
	}
	
	addr = 0;
	/* NO ERROR CHECK */
	if( '0'==input_buf[0] && 'x'==input_buf[1] ){
		addr = hex2dec(input_buf[2]);
		for(i=3;input_buf[i]!='\0';i++){
			addr *= 16;
			addr += hex2dec(input_buf[i]);
		}
	}else{
		addr = input_buf[0] - '0';
		for(i=1;input_buf[i]!='\0';i++){
			addr *= 10;
			addr += input_buf[i];
		}
	}
	return addr;
}
#endif /* end of function:getAddress */

void clearScreen(void) large{
	putchar(0x1B);
	putchar('[');
	putchar('2');
	putchar('J');
	putchar(0x1B);
	putchar('[');
	putchar('1');
	putchar(';');
	putchar('1');
	putchar('H');
	putchar(0);
}

void cliGetMacAddr(void) large{
    uint8      tmp;
    char      buffer[18];
    uint16    ptr;
    uint32    timeout;
    sal_mac_t mac;

    ptr = 0;
    timeout = 0xFFFFFFFFUL;

    while(1)
    {
        tmp = getchar_nb();

        /* no input, decrease timeout counter */
        if(0xFF == tmp)
        {
            timeout--;
            if(0 == timeout)
            {
                printf("Input timeout\n");
            }
        }
        else
        {
            if(0x0D == tmp)
            {
                buffer[17] = 0;
                buffer[ptr] = 0;
                if(0 == str_to_mac(mac.octet, buffer))
                {
                    sal_sys_mac_set(&mac);
                    printf("\nMAC address is set, save to flash?[y/n]: ");

                    timeout = 0xFFFFFFFFUL;
                    while(1)
                    {
                        tmp = getchar_nb();
                        if(0xFF == tmp)
                        {
                            timeout--;
                            if(0 == timeout)
                            {
                                printf("Input timeout, MAC address is not saved to flash\n");
                                break;
                            }
                        }
                        else
                        {
                            printf("%c \n", tmp);
                            if(tmp == 'y')
                            {
                                printf("MAC address is saved to Flash\n");
                                sal_flash_sysMac_set(&mac);                                
                                //sal_flash_cfg_save();
                            }
                            else
                            {
                                printf("MAC address is not saved to Flash\n");
                            }
                            break;
                        }
                    }
                }
                else
                {
                    printf("\nError input\n");
                }
                
                break;
            }
            else
            {
                printf("%c", tmp);
                buffer[ptr++] = tmp;
                timeout = 0xFFFFFFFF;
            }
        }
    }
}

uint8 cmd_select(void) large
{
	uint8 xdata cmd;
	
	//cmd = getchar();
	cmd = getchar_nb();
	return cmd;
}

void change_clock(uint16 xdata value) large{
	volatile uint32 xdata i;
	
	for(i=0;i<0x100;i++);
	if(value < 0x04) //8051 Clock >100Mhz
	{
	    //if 8051 Clock >100Mhz, SPI flash access clock is divided to half 8051 Clock
        setReg(DW8051_RDY, (DW8051_RDY_RATE(value)|(1<<8)) );
	}
	else
    {
        //if 8051 Clock < 100Mhz, SPI flash access clock is equal to 8051 Clock
	setReg(DW8051_RDY, (DW8051_RDY_RATE(value)) );
    }
	for(i=0;i<65535;i++);
	uart_init();
	timer_init();
	for(i=0;i<0x100;i++);
}

void set_8051RDY() large{
	volatile uint32 xdata tmp;
	tmp = getReg(DW8051_RDY);

	setReg(DW8051_RDY, tmp |0x1);
}

uint8 m_u8_uart_str[50];
uint8 m_u8_temp_cntr = 0;
void sys_uart_cmd()
{
    uint8 cmd;
    
    cmd = cmd_select();
	
    if( cmd!=0xFF )
    {
//        OSAL_PRINTF("%c\n", cmd);
			
				m_u8_uart_str[m_u8_temp_cntr++] = cmd;
				if(cmd == 0x0A || m_u8_temp_cntr >= 100)
				{
						m_u8_uart_str[m_u8_temp_cntr] = '\0';
						process_uart_string();
						m_u8_temp_cntr = 0;	
				}
				
			

			switch(cmd)
			{
				case CLI_COMMAND_HELP:
					print_menu();
					break;
				case CLI_COMMAND_CLEAR:
					clearScreen();
					break;
							case CLI_COMMAND_DUMP_IP:
						printf("  IP:%d.%d.%d.%d\n", (uint16)this_ip[0], (uint16)this_ip[1], (uint16)this_ip[2], (uint16)this_ip[3]);
									printf("Mask:%d.%d.%d.%d\n", (uint16)netmask[0], (uint16)netmask[1], (uint16)netmask[2], (uint16)netmask[3]);
									printf("  GW:%d.%d.%d.%d\n", (uint16)default_gateway.addr[0], (uint16)default_gateway.addr[1], (uint16)default_gateway.addr[2], (uint16)default_gateway.addr[3]);
									printf(" MAC:%02bX.%02bX.%02bX.%02bX.%02bX.%02bX\n", this_mac[0], this_mac[1], this_mac[2], this_mac[3], this_mac[4], this_mac[5]);
					break;
							case CLI_COMMAND_MAC_SET:
									printf("Format: XX:XX:XX:XX:XX:XX, press Enter to submit.\n");
									printf("input: ");
									cliGetMacAddr();
									break;
							default:
					break;
			};        
    }
}

uint8 m_u8_strlen;
void process_uart_string(void)
{
	uint8 temp_var;
	printf("%s\r\n",m_u8_uart_str);
	
	m_u8_strlen = strlen(m_u8_uart_str);
	
	send_str_web(m_u8_uart_str);
	
	if(m_u8_uart_str[0] == '{' && m_u8_uart_str[2] == 'A' && m_u8_uart_str[m_u8_strlen - 3] == '}')
	{
			send_str_web(m_u8_uart_str);
		  update_json_string(m_u8_uart_str);
		// verify_get_poe_data();
	}
	
	
//	if(m_u8_uart_str[0] == 's')
//	{
//		if(m_u8_uart_str[4] == 'A'  && m_u8_uart_str[5] == 'C')   //set_AC::xx;
//		{
//			uint8 ac_temp_str[5];
//			uint8 ac_temp_var;
//			ac_temp_str[0] = m_u8_uart_str[8];
//			ac_temp_str[1 = m_u8_uart_str[9];
//			ac_temp_str[2 = '\0';
//			ac_temp_var = atoi(ac_temp_str);
//		}
//		else if(m_u8_uart_str[4] == 'B'  && m_u8_uart_str[5] == 'A' && m_u8_uart_str[6] == 'T' && m_u8_uart_str[9] == 'R' && m_u8_uart_str[10] == 'Y')   //set_BATTERY::xx;
//		{
//			uint8 battery_temp_str[5];
//			uint8 battery_temp_var;
//			battery_temp_str[0] = battery_temp_str[8];
//			battery_temp_str[1] = battery_temp_str[9];
//			battery_temp_str[2] = '\0';
//			ac_temp_var = atoi(ac_temp_str);
//		}
//		else if(m_u8_uart_str[4] == 'B'  && m_u8_uart_str[5] == 'A' && m_u8_uart_str[6] == 'T' && m_u8_uart_str[8] == 'P' && m_u8_uart_str[10] == 'R')   //set_BAT_PER::xx;
//		{
//			
//		}
//		else if(m_u8_uart_str[4] == 'M'  && m_u8_uart_str[6] == 'O' && m_u8_uart_str[7] == 'D' && m_u8_uart_str[8] == 'E')   //set_MODE::xx;
//		{
//			
//		}
//	}
	for(temp_var = 0; temp_var < 50 ; temp_var++)
	{
			m_u8_uart_str[temp_var] = '\0';
	}
}


void verify_get_poe_data(const char* temp_data)
{
	static uint8 u8_cntr = 0;
	static uint8 u8_flag_open_braces =0;
	static uint8 u8_flag_A =0;
	static uint8 u8_flag_B =0;
	static uint8 u8_flag_C =0;
	static uint8 u8_flag_D =0;
	static uint8 u8_flag_E =0;
	static uint8 u8_flag_close_braces =0;

	while(temp_data[u8_cntr++] != '\0')
	{
		if(temp_data[u8_cntr] == '{')
		{
			u8_flag_open_braces = 1;
		}
		else if(temp_data[u8_cntr] == 'A')
		{
			u8_flag_A = 1;
		}
		else if(temp_data[u8_cntr] == 'B')
		{
			u8_flag_B = 1;
		}
		else if(temp_data[u8_cntr] == 'C')
		{
			u8_flag_C = 1;
		}
		else if(temp_data[u8_cntr] == 'D')
		{
			u8_flag_D = 1;
		}
		else if(temp_data[u8_cntr] == 'E')
		{
			u8_flag_E = 1;
		}
		else if(temp_data[u8_cntr] == '}')
		{
			u8_flag_close_braces = 1;
		}
		if(u8_flag_open_braces == 1 && u8_flag_A == 1 && u8_flag_B == 1 && u8_flag_C == 1 &&u8_flag_D == 1 && u8_flag_D == 1 && u8_flag_E == 1 && u8_flag_close_braces == 1 )
		{
			u8_cntr = 0;
			
			u8_flag_open_braces = 0;
			u8_flag_A = 0;
			u8_flag_B = 0;
			u8_flag_C = 0;
			u8_flag_D = 0;
			u8_flag_E = 0;
			u8_flag_close_braces = 0;
			send_str_web(m_u8_uart_str);
		}
		else
		{
				send_str_web("ERROR");
		}
	}
}