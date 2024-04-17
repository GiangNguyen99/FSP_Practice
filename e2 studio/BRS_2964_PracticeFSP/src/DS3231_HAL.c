/*
 * DS3231_HAL.c
 *
 *  Created on: Apr 2, 2024
 *      Author: giang.nguyen-hoang
 */

#include "hal_data.h"
#include "SEGGER_RTT/SEGGER_RTT.h"

#include "DS3231_HAL.h"
#include<stdint.h>
#include<string.h>

uint32_t timeout_ms = I2C_TRANSACTION_BUSY_DELAY;

static uint8_t ds3231_read(uint8_t reg_addr);
static void ds3231_write(uint8_t value, uint8_t reg_addr);
//static fsp_err_t DS3231_ResetBus(void);
static uint8_t bcd_to_binary(uint8_t value);
static uint8_t binary_to_bcd(uint8_t value);

static i2c_master_event_t g_i2c_callback_event = 0;
void i2c_callback(i2c_master_callback_args_t *p_args)
{
    g_i2c_callback_event = p_args->event;
}

uint8_t ds3231_init(void)
{
    fsp_err_t err;

    //1. open i2c
    err = R_IIC_MASTER_Open (&g_i2c_master0_ctrl, &g_i2c_master0_cfg);
    if (err != FSP_SUCCESS)
    {
//        SEGGER_RTT_WriteString (0, "R_IIC_MASTER_Open failed.\r\n");
        while (1)
            ;
    }

//    //2. config slave address, address mode
//    err = R_IIC_MASTER_SlaveAddressSet (&g_i2c_master0_ctrl, DS3231_I2C_ADDRESS, I2C_MASTER_ADDR_MODE_7BIT);
//    if (err != FSP_SUCCESS)
//    {
//        SEGGER_RTT_WriteString (0, "R_IIC_MASTER_SlaveAddressSet failed.\r\n");
//        while (1)
//            ;
//    }

//3. config ds3231 in Control register
    ds3231_write (DS3231_CR_DI_SQW, DS3231_ADDR_CR);

    //4. Read : Oscillator Stop Flag (OSF) in status register (SR)
    uint8_t clock_state = ds3231_read (DS3231_ADDR_SR);

    //5.return OSF, if OSF = 1 oscillator was stopped
    return ((clock_state >> 7) & 0x1);
}

static void ds3231_write(uint8_t value, uint8_t reg_addr)
{
    uint8_t tx[2];
    tx[0] = reg_addr;
    tx[1] = value;
    fsp_err_t err;

//    uint8_t *const ptr = &tx[0];

//    err = DS3231_ResetBus();

    g_i2c_callback_event = I2C_MASTER_EVENT_ABORTED;
    err = R_IIC_MASTER_Write (&g_i2c_master0_ctrl, tx, 2, 0);
    if (err != FSP_SUCCESS)
    {
        while (1)
            ;
    }
    /* Since there is nothing else to do, block until Callback triggers*/
    while ((I2C_MASTER_EVENT_TX_COMPLETE != g_i2c_callback_event) && timeout_ms)
    {
        R_BSP_SoftwareDelay (1U, BSP_DELAY_UNITS_MILLISECONDS);
        timeout_ms--;
        ;
    }
    if (I2C_MASTER_EVENT_ABORTED == g_i2c_callback_event)
    {
        __BKPT(0);
    }

}

static uint8_t ds3231_read(uint8_t reg_addr)
{
    uint8_t data;
    fsp_err_t err;

//    uint8_t *const ptr = &reg_addr;

//    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

    g_i2c_callback_event = I2C_MASTER_EVENT_ABORTED;
    err = R_IIC_MASTER_Write (&g_i2c_master0_ctrl, &reg_addr, 1, 0);
    if (err != FSP_SUCCESS)
    {
        while (1)
            ;
    }
    while ((I2C_MASTER_EVENT_TX_COMPLETE != g_i2c_callback_event) && timeout_ms)
    {
        R_BSP_SoftwareDelay (1U, BSP_DELAY_UNITS_MILLISECONDS);
        timeout_ms--;
        ;
    }
    if (I2C_MASTER_EVENT_ABORTED == g_i2c_callback_event)
    {
        __BKPT(0);
    }
//    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    g_i2c_callback_event = I2C_MASTER_EVENT_ABORTED;

    err = R_IIC_MASTER_Read (&g_i2c_master0_ctrl, &data, 1, 0);
    if (err != FSP_SUCCESS)
    {
        while (1)
            ;
    }
    /* Since there is nothing else to do, block until Callback triggers*/
    while ((I2C_MASTER_EVENT_RX_COMPLETE != g_i2c_callback_event) && timeout_ms)
    {
        R_BSP_SoftwareDelay (1U, BSP_DELAY_UNITS_MILLISECONDS);
        timeout_ms--;
        ;
    }
    if (I2C_MASTER_EVENT_ABORTED == g_i2c_callback_event)
    {
        __BKPT(0);
    }

    return data;
}

//static fsp_err_t DS3231_ResetBus(void)
//{
//    fsp_err_t err;
//
//    for (int i = 0; i < 20; i++)
//    {
//        g_i2c_master0_ctrl.p_reg->ICCR1_b.CLO = 1;
//        while (g_i2c_master0_ctrl.p_reg->ICCR1_b.CLO)
//        {
//        }
//    }
//    err = R_IIC_MASTER_Abort (&g_i2c_master0_ctrl);
//    if (err != FSP_SUCCESS)
//    {
////        SEGGER_RTT_WriteString (0, "R_IIC_MASTER_Abort failed.\r\n");
//        return err;
//    }
//
//    return FSP_SUCCESS;
//}

void ds3231_set_current_time(RTC_time_t *rtc_time)
{
    uint8_t seconds, hrs;
    seconds = binary_to_bcd (rtc_time->seconds);
    seconds &=(uint8_t)( ~(1 << 7));
    ds3231_write (seconds, DS3231_ADDR_SEC);

    ds3231_write (binary_to_bcd (rtc_time->minutes), DS3231_ADDR_MIN);

    hrs = binary_to_bcd (rtc_time->hours);

    if (rtc_time->time_format == TIME_FORMAT_24HRS)
    {
        hrs &= (uint8_t)(~(1 << 6));
    }
    else
    {
        hrs |= (1 << 6);
        hrs = (uint8_t)((rtc_time->time_format == TIME_FORMAT_12HRS_PM) ? hrs | (1 << 5) : hrs & ~(1 << 5));
    }

    ds3231_write (hrs, DS3231_ADDR_HRS);

}

void ds3231_set_current_date(RTC_date_t *rtc_date)
{
    uint8_t mon;

    ds3231_write (binary_to_bcd (rtc_date->day), DS3231_ADDR_DAY);

    ds3231_write (binary_to_bcd (rtc_date->date), DS3231_ADDR_DATE);

    mon = binary_to_bcd (rtc_date->month);
    mon &= (uint8_t)~(1 << 7);
    ds3231_write (mon, DS3231_ADDR_MONTH);

    ds3231_write (binary_to_bcd (rtc_date->year), DS3231_ADDR_YEAR);

}

void ds3231_get_current_time(RTC_time_t *rtc_time)
{

    uint8_t seconds, hrs;

    seconds = ds3231_read (DS3231_ADDR_SEC);

    seconds &= (uint8_t)(~(1 << 7));

    rtc_time->seconds = bcd_to_binary (seconds);
    rtc_time->minutes = bcd_to_binary (ds3231_read (DS3231_ADDR_MIN));

    hrs = ds3231_read (DS3231_ADDR_HRS);
    if (hrs & (1 << 6))
    {
        //12 hr format
        rtc_time->time_format = !((hrs & (1 << 5)) == 0);
        hrs &= (uint8_t)(~(0x3 << 5)); //Clear 6 and 5
    }
    else
    {
        //24 hr format
        rtc_time->time_format = TIME_FORMAT_24HRS;
    }

    rtc_time->hours = bcd_to_binary (hrs);
}

void ds3231_get_current_date(RTC_date_t *rtc_date)
{
    rtc_date->day = bcd_to_binary (ds3231_read (DS3231_ADDR_DAY));
    rtc_date->date = bcd_to_binary (ds3231_read (DS3231_ADDR_DATE));
    rtc_date->month = bcd_to_binary (ds3231_read (DS3231_ADDR_MONTH));
    rtc_date->year = bcd_to_binary (ds3231_read (DS3231_ADDR_YEAR));

}

static uint8_t binary_to_bcd(uint8_t value)
{
    uint8_t m, n;
    uint8_t bcd;

    bcd = value;
    if (value >= 10)
    {
        m = value / 10;
        n = value % 10;
        bcd = (m << 4) | n;
    }

    return bcd;
}

static uint8_t bcd_to_binary(uint8_t value)
{
    uint8_t m, n;
    m = (uint8_t) ((value >> 4) * 10);
    n = value & (uint8_t) 0x0F;
    return (m + n);
}

void ds3231_set_time_alarm1(RTC_time_t *rtc_time, RTC_date_t *rtc_date, uint8_t mode_alarm)
{
    uint8_t seconds, minutes, hrs;
    uint8_t temp1 = 0;
    uint8_t day, date;

    seconds = binary_to_bcd (rtc_time->seconds);
    temp1 = mode_alarm & 0x01;
    seconds |= (temp1 << 7);
    ds3231_write (seconds, DS3231_ADDR_SEC_AL1);

    temp1 = 0;
    minutes = binary_to_bcd (rtc_time->minutes);
    temp1 = mode_alarm & 0x02;
    minutes |= (temp1 << 6);
    ds3231_write (binary_to_bcd (rtc_time->minutes), DS3231_ADDR_MIN_AL1);

    temp1 = 0;

    hrs = binary_to_bcd (rtc_time->hours);

    if (rtc_time->time_format == TIME_FORMAT_24HRS)
    {
        hrs &= (uint8_t)(~(1 << 6));
    }
    else
    {
        hrs |= (1 << 6);
        hrs = (uint8_t)((rtc_time->time_format == TIME_FORMAT_12HRS_PM) ? hrs | (1 << 5) : hrs & ~(1 << 5));
    }
    temp1 = mode_alarm & 0x04;
    hrs |= (temp1 << 5);
    ds3231_write (hrs, DS3231_ADDR_HRS_AL1);

    temp1 = 0;
    if (rtc_date->day)
    {
        day = binary_to_bcd (rtc_date->day);
        temp1 = mode_alarm & 0x10;
        day |= (temp1 << 2);
        temp1 = 0;
        temp1 = mode_alarm & 0x8;
        day |= (temp1 << 4);
        ds3231_write (day, DS3231_ADDR_DYDT_AL1);

    }
    else
    {
        date = binary_to_bcd (rtc_date->date);
        temp1 = mode_alarm & 0x10;
        date |= (temp1 << 2);
        temp1 = 0;
        temp1 = mode_alarm & 0x8;
        date |= (temp1 << 4);

        ds3231_write (date, DS3231_ADDR_DYDT_AL1);
    }
    ds3231_write (DS3231_CR_EN_AL1, DS3231_ADDR_CR);

}

void ds3231_clear_sr(void)
{
    ds3231_write (0x8, DS3231_ADDR_SR);
}
