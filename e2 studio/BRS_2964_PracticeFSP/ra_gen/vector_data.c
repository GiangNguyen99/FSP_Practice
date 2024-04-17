/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [1] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [2] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [3] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
            [4] = r_icu_isr, /* ICU IRQ4 (External pin interrupt 4) */
            [5] = gpt_counter_overflow_isr, /* GPT2 COUNTER OVERFLOW (Overflow) */
            [6] = gpt_capture_a_isr, /* GPT2 CAPTURE COMPARE A (Capture/Compare match A) */
            [7] = r_icu_isr, /* ICU IRQ0 (External pin interrupt 0) */
            [8] = adc_scan_end_isr, /* ADC0 SCAN END (End of A/D scanning operation) */
            [9] = adc_window_compare_isr, /* ADC0 WINDOW A (Window A Compare match interrupt) */
            [10] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [11] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [12] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [13] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [14] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [15] = spi_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [16] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [17] = spi_eri_isr, /* SPI1 ERI (Error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP0), /* IIC0 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP1), /* IIC0 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP2), /* IIC0 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP3), /* IIC0 ERI (Transfer error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ4,GROUP4), /* ICU IRQ4 (External pin interrupt 4) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_GPT2_COUNTER_OVERFLOW,GROUP5), /* GPT2 COUNTER OVERFLOW (Overflow) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_GPT2_CAPTURE_COMPARE_A,GROUP6), /* GPT2 CAPTURE COMPARE A (Capture/Compare match A) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ0,GROUP7), /* ICU IRQ0 (External pin interrupt 0) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_ADC0_SCAN_END,GROUP0), /* ADC0 SCAN END (End of A/D scanning operation) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_ADC0_WINDOW_A,GROUP1), /* ADC0 WINDOW A (Window A Compare match interrupt) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP2), /* SCI9 RXI (Receive data full) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP3), /* SCI9 TXI (Transmit data empty) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP4), /* SCI9 TEI (Transmit end) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP5), /* SCI9 ERI (Receive error) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SPI1_RXI,GROUP6), /* SPI1 RXI (Receive buffer full) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TXI,GROUP7), /* SPI1 TXI (Transmit buffer empty) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_SPI1_TEI,GROUP0), /* SPI1 TEI (Transmission complete event) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_SPI1_ERI,GROUP1), /* SPI1 ERI (Error) */
        };
        #endif
        #endif
