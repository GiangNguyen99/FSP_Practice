/*
 * sci_uart.h
 *
 *  Created on: Apr 11, 2024
 *      Author: giang.nguyen-hoang
 */

#ifndef SCI_UART_H_
#define SCI_UART_H_

#include "common_utils.h"

/* Macro definition */
#define CARRIAGE_ASCII            (13u)     /* Carriage return */
#define DATA_LENGTH               (30u)      /* Expected Input Data length */


void sci_uart_init(void);
void sci_uart_deinit(void);
void sci_uart_write_rtt(void);
#endif /* SCI_UART_H_ */
