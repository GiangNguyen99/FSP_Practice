/*
 * sci_uart.c
 *
 *  Created on: Apr 11, 2024
 *      Author: giang.nguyen-hoang
 */

#include "sci_uart.h"

/*
 * Private global variables
 */

/* Temporary buffer to save data from receive buffer for further processing */
static  uint8_t g_temp_buffer[DATA_LENGTH] =
{ RESET_VALUE };

/* Counter to update g_temp_buffer index */
static volatile uint8_t g_counter_var = RESET_VALUE;

/* Flag to check whether data is received or not */
static volatile uint8_t g_data_received_flag = false;

/* Flag for user callback */
volatile uint32_t g_transfer_complete = 0;
volatile uint32_t g_receive_complete = 0;
//volatile uint32_t g_rx_receive_data = 0;

uint32_t g_out_of_band_index = 0;

void sci_uart_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize UART channel with baud rate 115200 */
    err = R_SCI_UART_Open (&g_uart0_ctrl, &g_uart0_cfg);
    if (FSP_SUCCESS != err)
    {
        SEGGER_RTT_printf(0,"\r\n**  R_SCI_UART_Open API failed  **\r\n");
    }
}
void sci_uart_deinit(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Close module */
    err = R_SCI_UART_Close (&g_uart0_ctrl);
    if (FSP_SUCCESS != err)
    {
        SEGGER_RTT_printf(0,"\r\n**  R_SCI_UART_Close API failed  ** \r\n");
    }
}
void sci_uart_write_rtt(void)
{

    if(g_data_received_flag == true)
    {
    SEGGER_RTT_printf(0, "\n\rRX receive data: %s\n\r", g_temp_buffer);
    g_data_received_flag = false;
    memset(g_temp_buffer, RESET_VALUE, DATA_LENGTH);
    }
}
void user_uart0_callback(uart_callback_args_t *p_args)
{
    /* Handle the UART event */
    switch (p_args->event)
    {
        /* Received a character */
        case UART_EVENT_RX_CHAR:
        {
            if (p_args->data == CARRIAGE_ASCII)
            {
                /* If Enter is pressed by user, set flag to process the data */
                g_counter_var = RESET_VALUE;
                g_data_received_flag = true;
            }
            /* Read all data provided by user until enter button is pressed */
            else
            {
                g_temp_buffer[g_counter_var++] = (uint8_t ) p_args->data;

            }
            break;
        }

            /* Receive complete */
        case UART_EVENT_RX_COMPLETE:
        {
            g_receive_complete = 1;
            break;
        }
            /* Transmit complete */
        case UART_EVENT_TX_COMPLETE:
        {
            g_transfer_complete = 1;
            break;
        }
        default:
        {
            break;
        }
    }

}
