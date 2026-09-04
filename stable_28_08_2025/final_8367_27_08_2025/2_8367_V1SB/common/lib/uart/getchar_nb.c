#include <reg_sfr.h>

#include <stdio.h>

char getchar_nb(void){
	char c;	
	while(!RI){
		return(0xFF);
	}
	RI=0; 
	c=SBUF0; 
	return c; 
}

/* unused
char getchar(void) 
{ 
    char c;	
	while(!RI);
	RI=0; 
	c=SBUF0; 
	return c; 
}
*/

