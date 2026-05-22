#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include <string.h>
#include <stdio.h>


void conf_uart(void);
void generate_triangle_in_memory(void);
void send_signal_ascii(void) ;

uint8_t holamundo[] = {0};



int main(void)
{

	conf_uart();

	printf("Aca empieza el codigo \n");

	while(1)
	{

		UART_Receive(UART0, holamundo, 2, BLOCKING);
		printf("gatillo: %d\n", holamundo[0]);
		printf("joystick: %d\n", holamundo[1]);


	}



}


void conf_uart(void)
{

	UART_PinConfig(UART_TX0_P0_2);
	UART_PinConfig(UART_RX0_P0_3);


	UART_CFG_T confuart0 =
	{
		.baudRate = 9600,
		.parity = UART_PARITY_NONE,
		.dataBits = UART_DBITS_8,
		.stopBits = UART_STOPBIT_1,
	};

	UART_Init(UART0, &confuart0);


}
