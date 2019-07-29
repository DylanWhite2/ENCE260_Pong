/**
    @file       ir.c
    @authors    Dylan White    29861039
                Alex Tompkins  79004580
                * TEAM334
    @date       7 Oct 2017
    @brief      Handles IR communication between the two boards playing.
*/


#include "system.h"
#include "ir_uart.h"
#include "ir.h"


/**
	Broadcasts a single byte of data using IR.
	@param data the byte of data to send.
*/
void sendIR(uint8_t data)
{
    ir_uart_putc(data);
}

/**
	Receives a single byte of data using IR. This function is non-blocking, and 
	if no data is received then it will return -1 (255 as an unsigned byte).
	@return the byte of data received or -1 if no data
*/
uint8_t recvIR(void)
{
    if (ir_uart_read_ready_p()) {
        return ir_uart_getc();
    } 
    else {
        return -1;
    }
}

/**
	Waits for and then receives a single byte of data using IR. This function 
	is blocking; it will wait until a byte of data is received before returning.
	@return the byte of data received
*/
uint8_t recvIRBlocking(void)
{
    return ir_uart_getc();
}
