#include "main_thread0.h"
#include "common_utils.h"
#include "DS3231_HAL.h"
#include "gpt_pwm.h"
#include "adc_temperature.h"
#include "sci_uart.h"
#include "spi_w25q32.h"

//#define exercise 1
//#define exercise 2
//#define exercise 3
//#define exercise 4
//#define exercise 5
#define exercise 6

#ifndef exercise
#define exercise 1
#endif

volatile uint64_t g_captured_time = 0U;
volatile uint32_t g_capture_overflows = 0U;
float pulse_time;
uint32_t pulse_time_high = 0, pulse_time_low = 0;
volatile uint8_t state_pin = 0;
timer_info_t info0;

/* main thread entry function */

# if (exercise == 1 || exercise == 2)
void vTask1(void *pvParameters);

#endif

# if (exercise == 1 || exercise == 2 || exercise == 3 || exercise == 5)

void vTask2(void *pvParameters);

#endif

# if exercise == 4
void vTask3(void *pvParameters);
#endif

# if exercise == 6
void vTask4(void *pvParameters);
#endif

void user_callback_0(external_irq_callback_args_t *p_args);
void user_callback_1(external_irq_callback_args_t *p_args);
void system_init(void);

uint8_t convert(char c);

QueueHandle_t queue1_handle, queue2_handle;

/* pvParameters contains TaskHandle_t */
void main_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    /* TODO: add your own code here */
//    TaskHandle_t task1_handle, task2_handle, task3_handle;
# if (exercise == 1 || exercise == 2)
    TaskHandle_t task1_handle;
#endif

# if (exercise == 1 || exercise == 2 || exercise == 3 || exercise == 5)
    TaskHandle_t task2_handle;
#endif

# if exercise == 4
    TaskHandle_t task3_handle;
#endif
# if exercise == 6
    TaskHandle_t task4_handle;
#endif
    BaseType_t status;

# if (exercise == 1 || exercise == 2)
    RTC_time_t current_time;
    RTC_date_t current_date;

    current_time.hours = 1;
    current_time.minutes = 1;
    current_time.seconds = 1;
    current_time.am_pm = 1;
    current_time.time_format = TIME_FORMAT_12HRS_PM;

    current_date.day = SUNDAY
    ;
    current_date.date = 1;
    current_date.month = 1;
    current_date.year = 1;

    if (ds3231_init ())
    {
        while (1)
        {
            SEGGER_RTT_printf (0, "DS3231 INIT WAS FAILED\r\n");
            vTaskDelay (pdMS_TO_TICKS(1000));
        }
    }

    ds3231_set_current_time (&current_time);
    ds3231_set_current_date (&current_date);

#endif

    system_init ();

# if (exercise == 1 || exercise == 2)
    status = xTaskCreate (vTask1, "Task 1", 800,
    NULL,
                          5, &task1_handle);
    configASSERT(status == pdPASS);
#endif
# if exercise == 1 || exercise == 2 || exercise == 3 || exercise == 5
    status = xTaskCreate (vTask2, "Task 2", 500,
    NULL,
                          2, &task2_handle);
    configASSERT(status == pdPASS);
#endif

# if exercise == 4
    status = xTaskCreate (vTask3, "Task 3", 500,
    NULL,
                          3, &task3_handle);
    configASSERT(status == pdPASS);
#endif

# if exercise == 6
    status = xTaskCreate (vTask4, "Task 4", 500,
    NULL,
                          4, &task4_handle);
    configASSERT(status == pdPASS);
#endif

    queue1_handle = xQueueCreate(1, 50);
    if (queue1_handle == NULL)
    {
        /* Queue was not created and must not be used. */
    }

    queue2_handle = xQueueCreate(1, 50);
    if (queue2_handle == NULL)
    {
        /* Queue was not created and must not be used. */
    }
    while (1)
    {
        vTaskDelay (1);
    }
}
# if exercise == 1 || exercise == 2
void vTask1(void *pvParameters)
{
    (void) pvParameters;
    const char *pcTaskName = "task 1\r\n";
    RTC_time_t current_time;
    RTC_date_t current_date;
    uint8_t count = 0;
    DS3231_data_t ds3231_data_send, ds3231_data_receive_rtt;
//    DS3231_data_t ds3231_data_receive_rtt;

    while (1)
    {
        SEGGER_RTT_printf (0, pcTaskName);

        if (queue2_handle != NULL)
        {
            if (xQueueReceive (queue2_handle, &ds3231_data_receive_rtt, (TickType_t) 100) == pdPASS)
            {
                current_time.hours = ds3231_data_receive_rtt.time.hours;
                current_time.minutes = ds3231_data_receive_rtt.time.minutes;
                current_time.seconds = ds3231_data_receive_rtt.time.seconds;

                current_date.date = ds3231_data_receive_rtt.date.date;
                current_date.month = ds3231_data_receive_rtt.date.month;
                current_date.year = ds3231_data_receive_rtt.date.year;

                ds3231_set_current_time (&current_time);
                ds3231_set_current_date (&current_date);
            }
        }

        count++;
        ds3231_get_current_time (&current_time);
        ds3231_get_current_date (&current_date);

        if (count == 5)
        {
            count = 0;

            ds3231_data_send.time.hours = current_time.hours;
            ds3231_data_send.time.minutes = current_time.minutes;
            ds3231_data_send.time.seconds = current_time.seconds;

            ds3231_data_send.date.date = current_date.date;
            ds3231_data_send.date.month = current_date.month;
            ds3231_data_send.date.year = current_date.year;

            xQueueSend(queue1_handle, (void* )&ds3231_data_send, portMAX_DELAY);

        }

        vTaskDelay (pdMS_TO_TICKS(1000));

    }
}

#endif

# if exercise == 1 || exercise == 2 || exercise == 3 || exercise == 5
void vTask2(void *pvParameters)
{
# if (exercise == 1 || exercise == 2)
    /*exercis 1*/

    (void) pvParameters;
    const char *pcTaskName = "task 2\r\n";
    DS3231_data_t ds3231_data_receive, rtt_data_send_ds3231;
    char acIn[28];
    unsigned NumBytes = sizeof(acIn);

    while (1)
    {
        SEGGER_RTT_printf (0, pcTaskName);

        if (queue1_handle != NULL)
        {
            if (xQueueReceive (queue1_handle, &ds3231_data_receive, (TickType_t) 100) == pdPASS)
            {
                SEGGER_RTT_printf (0, "TIME: %d:%d:%d \r\n", ds3231_data_receive.time.hours,
                                   ds3231_data_receive.time.minutes, ds3231_data_receive.time.seconds);
                SEGGER_RTT_printf (0, "DATE: %d/%d/%d \r\n", ds3231_data_receive.date.date,
                                   ds3231_data_receive.date.month, ds3231_data_receive.date.year);
            }
        }

        if (SEGGER_RTT_HasKey ())
        {

            NumBytes = SEGGER_RTT_Read (0, &acIn[0], NumBytes);
            if (NumBytes)
            {
                rtt_data_send_ds3231.time.hours = (uint8_t)(convert(acIn[0]) * 10 + convert(acIn[1])) ;
                rtt_data_send_ds3231.time.minutes = (uint8_t)(convert(acIn[2]) * 10 + convert(acIn[3]));
                rtt_data_send_ds3231.time.seconds = (uint8_t)(convert(acIn[4]) * 10 + convert(acIn[5]));

                rtt_data_send_ds3231.date.date = (uint8_t)(convert(acIn[7]) * 10 + convert(acIn[8]));
                rtt_data_send_ds3231.date.month = (uint8_t)(convert(acIn[9]) * 10 + convert(acIn[10]));
                rtt_data_send_ds3231.date.year = (uint8_t)(convert(acIn[11]) * 10 + convert(acIn[12]));

                xQueueSend(queue2_handle, (void* )&rtt_data_send_ds3231, portMAX_DELAY);

            }

        }

        vTaskDelay (pdMS_TO_TICKS(100));

    }

#endif

# if exercise == 3
    /*
     *
     *
     *
     *
     *
     *              exercise 3
     *
     *
     *
     * */

    (void) pvParameters;
    const char *pcTaskName = "task 2\r\n";
    char acIn[14];
    unsigned NumBytes = sizeof(acIn);

    uint32_t frequency = 0, duty_cycle = 0;
    fsp_err_t err;
    while (1)
    {
        SEGGER_RTT_printf (0, pcTaskName);

        if (SEGGER_RTT_HasKey ())
        {

            NumBytes = SEGGER_RTT_Read (0, &acIn[0], NumBytes);
            if (NumBytes)
            {
                frequency = (uint32_t) (convert (acIn[2]) * 1000 + convert (acIn[3]) * 100 + convert (acIn[4]) * 10
                        + convert (acIn[5]));
                duty_cycle = (uint32_t) (convert (acIn[9]) * 100 + convert (acIn[10]) * 10 + convert (acIn[11]));
            }
            /*
             *
             *          FERQUENCY
             *
             * */

            /* Get the source clock frequency (in Hz). There are 3 ways to do this in FSP:
             *  - If the PCLKD frequency has not changed since reset, the source clock frequency is
             *    BSP_STARTUP_PCLKD_HZ >> timer_cfg_t::source_div
             *  - Use the R_GPT_InfoGet function (it accounts for the divider).
             *  - Calculate the current PCLKD frequency using R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) and right shift
             *    by timer_cfg_t::source_div.
             *
             * This example uses the 3rd option (R_FSP_SystemClockHzGet).
             */
            uint32_t pclkd_freq_hz = R_FSP_SystemClockHzGet (FSP_PRIV_CLOCK_PCLKD) >> g_timer3_pwm_cfg.source_div;
            /* Calculate the desired period based on the current clock. Note that this calculation could overflow if the
             * desired period is larger than UINT32_MAX / pclkd_freq_hz. A cast to uint64_t is used to prevent this. */
            uint32_t period_counts = (uint32_t) ((uint64_t) pclkd_freq_hz / frequency);
            /* Set the calculated period. */
            err = R_GPT_PeriodSet (&g_timer3_pwm_ctrl, period_counts);
            assert(FSP_SUCCESS == err);

            /*
             *
             *          DUTY CYCLE
             *
             * */
            /* Get the current period setting. */
            timer_info_t info_pwm;
            (void) R_GPT_InfoGet (&g_timer3_pwm_ctrl, &info_pwm);
//            uint32_t current_period_counts = info_pwm.period_counts;
            uint32_t current_period_counts = period_counts;
            /* Calculate the desired duty cycle based on the current period. Note that if the period could be larger than
             * UINT32_MAX / 100, this calculation could overflow. A cast to uint64_t is used to prevent this. The cast is
             * not required for 16-bit timers. */
            uint32_t duty_cycle_counts = (uint32_t) (((uint64_t) current_period_counts * duty_cycle) / 100);
            /* Set the calculated duty cycle. */
            err = R_GPT_DutyCycleSet (&g_timer3_pwm_ctrl, duty_cycle_counts, GPT_IO_PIN_GTIOCB);
            assert(FSP_SUCCESS == err);

        }
        vTaskDelay (pdMS_TO_TICKS(100));
    }

#endif

# if exercise == 5
    (void) pvParameters;
    const char *pcTaskName = "task 2\r\n";
    unsigned NumBytes = 30;
    char g_src[NumBytes];

    extern volatile uint32_t g_transfer_complete;
//    extern volatile uint32_t g_receive_complete;

    fsp_err_t err;


    while (1)
    {
        SEGGER_RTT_printf (0, pcTaskName);

        if (SEGGER_RTT_HasKey ())
        {
            NumBytes = SEGGER_RTT_Read (0, &g_src[0], NumBytes);
            if (NumBytes)
            {

                err = R_SCI_UART_Write (&g_uart0_ctrl, (uint8_t*) g_src, NumBytes);
                assert(FSP_SUCCESS == err);
                //wait tx comlete
                while (!g_transfer_complete)
                {
                }
                g_transfer_complete = 0;
                sci_uart_write_rtt ();

            }

        }
        vTaskDelay (pdMS_TO_TICKS(100));

    }

#endif
}
#endif

# if exercise == 4
void vTask3(void *pvParameters)
{
    (void) pvParameters;
    const char *pcTaskName = "task 3\r\n";
    int32_t temperature_c = 0;

    while (1)
    {
        SEGGER_RTT_printf (0, pcTaskName);

        temperature_c = adc_temperature_read_data ();
        SEGGER_RTT_printf (0, "\r\nTemperature_c: %d\r\n", temperature_c);

        vTaskDelay (pdMS_TO_TICKS(5000));
    }

}
#endif

# if exercise == 6
void vTask4(void *pvParameters)
{
    (void) pvParameters;
//    const char *pcTaskName = "task 4\r\n";
//    static volatile uint32_t w25q32_id = 0;
    fsp_err_t err = FSP_SUCCESS;     // Error status


    /* Menu for User Selection */
    APP_PRINT(MENUOPTION1);
    APP_PRINT(MENUOPTION2);
    APP_PRINT(MENUOPTION3);
    APP_PRINT(MENUOPTION4);
    while (1)
    {
//        SEGGER_RTT_printf (0, pcTaskName);

//        w25q32_id = W25Q_ReadID ();
        if (APP_CHECK_DATA)
        {
            err = read_process_input_from_RTT ();
            /* handle error */
            if (FSP_SUCCESS != err)
            {
                spi_w25q32_deinit ();
//                APP_ERR_TRAP(err);
            }
        }
//        SEGGER_RTT_printf (0, "\r\nW25Q_ReadID: %d\r\n", w25q32_id);
        vTaskDelay (pdMS_TO_TICKS(100));
    }

}

#endif

void system_init(void)
{

# if exercise == 2 || exercise == 3
    fsp_err_t err;
    /* Initializes the module timer 0. */
    err = R_GPT_Open (&g_timer0_ctrl, &g_timer0_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Initializes the module timer 1. */
    err = R_GPT_Open (&g_timer1_ctrl, &g_timer1_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Enable the timer. */
    err = R_GPT_Enable (&g_timer0_ctrl);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Enable the timer. */
    (void) R_GPT_Enable (&g_timer1_ctrl);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Start the timer. */
    err = R_GPT_Start (&g_timer0_ctrl);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Start the timer. */
    (void) R_GPT_Start (&g_timer1_ctrl);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Initializes the module. */
    err = R_GPT_Open (&g_timer2_ctrl, &g_timer2_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Start the timer. */
    (void) R_GPT_Start (&g_timer2_ctrl);
#endif

# if exercise == 3
    /* Initializes the PWM module. */
    init_gpt_pwm ();
#endif

# if exercise == 4
    /* Initializes the ADC module. */
    adc_temperature_init ();
#endif
# if exercise == 5
    /* Initializes the SCI_UART module. */
    sci_uart_init ();
#endif
# if exercise == 6
    /* Initializes the SPI W25Q32 module. */
    spi_w25q32_init ();
#endif
//----------------------------------------------------------
//                  Config ISR external
//

# if exercise == 2
    /* Configure the external interrupt. */
    err = R_ICU_ExternalIrqOpen (&g_external_irq0_ctrl, &g_external_irq0_cfg);
    assert(FSP_SUCCESS == err);
    /* Enable the external interrupt. */
    /* Enable not required when used with ELC or DMAC. */
    err = R_ICU_ExternalIrqEnable (&g_external_irq0_ctrl);
    assert(FSP_SUCCESS == err);
#endif

# if exercise == 3
    /* Configure the external interrupt. */
    err = R_ICU_ExternalIrqOpen (&g_external_irq1_ctrl, &g_external_irq1_cfg);
    assert(FSP_SUCCESS == err);
    /* Enable the external interrupt. */
    /* Enable not required when used with ELC or DMAC. */
    err = R_ICU_ExternalIrqEnable (&g_external_irq1_ctrl);
    assert(FSP_SUCCESS == err);
#endif
    //-----------------------------------------------------------
}

void user_callback_0(external_irq_callback_args_t *p_args)
{
    (void) p_args;
# if exercise == 2
    bsp_io_level_t swq_status;
    timer_status_t status0, status1;
    timer_info_t info;
    fsp_err_t err;
    if (p_args->channel == 4)
    {
        R_IOPORT_PinRead (&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, &swq_status);
        if (BSP_IO_LEVEL_HIGH == swq_status)
        {
            //   time_high_stop
            SEGGER_RTT_printf (0, "High\r\n");

            /* Reset the timer. */
            (void) R_GPT_Reset (&g_timer0_ctrl);
            /* Start the timer. */
            err = R_GPT_Start (&g_timer0_ctrl);
            /* Handle any errors. This function should be defined by the user. */
            assert(FSP_SUCCESS == err);

            /* (Optional) Get the current period if not known. */
            (void) R_GPT_InfoGet (&g_timer1_ctrl, &info0);

            volatile uint64_t clock_frequency = info0.clock_frequency;

            /* Read the current counter value. Counter value is in status.counter. */
            (void) R_GPT_StatusGet (&g_timer1_ctrl, &status1);

            /* Calculate the pulse time */
            pulse_time = (float) (((float) status1.counter) / ((float) clock_frequency));

            pulse_time_low = (uint32_t) (pulse_time * 1000.0);

            /* Stop the timer*/
            (void) R_GPT_Stop (&g_timer1_ctrl);
        }
        else
        {

            SEGGER_RTT_printf (0, "Low\r\n");

            /* Reset the timer. */
            (void) R_GPT_Reset (&g_timer1_ctrl);

            /* Start the timer. */
            (void) R_GPT_Start (&g_timer1_ctrl);

            /* (Optional) Get the current period if not known. */

            (void) R_GPT_InfoGet (&g_timer0_ctrl, &info);
            volatile uint64_t clock_frequency = info.clock_frequency;

            /* Read the current counter value. Counter value is in status.counter. */
            (void) R_GPT_StatusGet (&g_timer0_ctrl, &status0);

            /* Calculate the pulse time */
            pulse_time = (float) (((float) status0.counter) / ((float) clock_frequency));

            pulse_time_high = (uint32_t) (pulse_time * 1000.0);

            /* Stop the timer*/
            (void) R_GPT_Stop (&g_timer0_ctrl);
        }
    }
#endif
}

# if exercise == 2 || exercise == 3
void user_timer2_callback(timer_callback_args_t *p_args)
{
    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        SEGGER_RTT_printf (0, "\rpulse_time_low: %d\r\n", pulse_time_low);
        SEGGER_RTT_printf (0, "\rpulse_time_high: %d\r\n\n", pulse_time_high);
    }
}
#endif

void user_callback_1(external_irq_callback_args_t *p_args)
{
    (void) p_args;
# if exercise == 3
    bsp_io_level_t pwm_status;
    timer_status_t status0, status1;
    timer_info_t info;
    fsp_err_t err;
    if (p_args->channel == 0)
    {
        R_IOPORT_PinRead (&g_ioport_ctrl, BSP_IO_PORT_01_PIN_05, &pwm_status);
        if (BSP_IO_LEVEL_HIGH == pwm_status)
        {
            //   time_high_stop
            SEGGER_RTT_printf (0, "High\r\n");

            /* Reset the timer. */
            (void) R_GPT_Reset (&g_timer0_ctrl);
            /* Start the timer. */
            err = R_GPT_Start (&g_timer0_ctrl);
            /* Handle any errors. This function should be defined by the user. */
            assert(FSP_SUCCESS == err);

            /* (Optional) Get the current period if not known. */
            (void) R_GPT_InfoGet (&g_timer1_ctrl, &info0);

            volatile uint64_t clock_frequency = info0.clock_frequency;

            /* Read the current counter value. Counter value is in status.counter. */
            (void) R_GPT_StatusGet (&g_timer1_ctrl, &status1);

            /* Calculate the pulse time */
            pulse_time = (float) (((float) status1.counter) / ((float) clock_frequency));

            pulse_time_low = (uint32_t) (pulse_time * 1000.0);

            /* Stop the timer*/
            (void) R_GPT_Stop (&g_timer1_ctrl);
        }
        else
        {

            SEGGER_RTT_printf (0, "Low\r\n");

            /* Reset the timer. */
            (void) R_GPT_Reset (&g_timer1_ctrl);

            /* Start the timer. */
            (void) R_GPT_Start (&g_timer1_ctrl);

            /* (Optional) Get the current period if not known. */

            (void) R_GPT_InfoGet (&g_timer0_ctrl, &info);
            volatile uint64_t clock_frequency = info.clock_frequency;

            /* Read the current counter value. Counter value is in status.counter. */
            (void) R_GPT_StatusGet (&g_timer0_ctrl, &status0);

            /* Calculate the pulse time */
            pulse_time = (float) (((float) status0.counter) / ((float) clock_frequency));

            pulse_time_high = (uint32_t) (pulse_time * 1000.0);

            /* Stop the timer*/
            (void) R_GPT_Stop (&g_timer0_ctrl);
        }
    }
#endif
}

uint8_t convert(char c)
{
    return (uint8_t) c - 48;
}

