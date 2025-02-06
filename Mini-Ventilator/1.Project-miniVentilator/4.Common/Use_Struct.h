#ifndef Use_Struct_H
#define Use_Struct_H

#include "Use_Enum.h"
#include "stm32f4xx_conf.h"

/* Serial port queue struct. */
#define UART_MAXBUFF	200
typedef struct{
	unsigned char tx_buff[UART_MAXBUFF]; 	
	unsigned char rx_buff[UART_MAXBUFF]; 	
	unsigned char rx_in_count; 			
	unsigned char rx_out_count; 			
	unsigned char tx_in_count; 			
	unsigned char tx_out_count; 			
}UART_Queue_t;

/* Blower state struct. */
typedef struct{
	unsigned int 	set_rpm; 	
	unsigned int 	actual_rpm; 	
	eBlower_State 	state;	 		
}Blower_State_t;

/* Machine runs parameters struct. */
typedef struct{
	int press_data; 	
	 		
}Run_Param_t;



/* Struct declaration. */
extern UART_Queue_t 			Blower_UartQueue; 	//Blower buff.
extern Blower_State_t			Blower_State;		
extern Run_Param_t				Run_Param;

#endif 
