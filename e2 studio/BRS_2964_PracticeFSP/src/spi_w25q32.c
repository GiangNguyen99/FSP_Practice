/*
 * spi_w25q32.c
 *
 *  Created on: Apr 12, 2024
 *      Author: giang.nguyen-hoang
 */

#include "spi_w25q32.h"

/* FreeRTOS includes. */
//#include "FreeRTOS.h"
//#include "task.h"
#define CS_PIN  BSP_IO_PORT_03_PIN_01
#define numBLOCK 64  // number of total blocks for 16Mb flash, 32x16x16 pages and 32x16x16x256 Bytes

/* Function macros to assert and de-assert Slave Select pins */
#define CS_ASSERT(x)     R_IOPORT_PinWrite(&g_ioport_ctrl, x, BSP_IO_LEVEL_HIGH);
#define CS_DE_ASSERT(x)  R_IOPORT_PinWrite(&g_ioport_ctrl, x, BSP_IO_LEVEL_LOW);

void csLOW(void);
void csHIGH(void);
void W25Q_Delay(uint32_t time);
void SPI_Write(uint8_t *data, uint16_t length);
void SPI_Read(uint8_t *data, uint32_t length);
void write_enable(void);
void write_disable(void);
//uint32_t bytestowrite(uint32_t size, uint16_t offset);
//uint32_t bytestomodify(uint32_t size, uint16_t offset);
//void float2Bytes(uint8_t *ftoa_bytes_temp, float float_variable);
//float Bytes2float(uint8_t *ftoa_bytes_temp);

void spi_w25q32_init(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Initialize the SPI module. */
    err = R_SPI_Open (&g_spi0_ctrl, &g_spi0_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

}
void spi_w25q32_deinit(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Initialize the SPI module. */
    err = R_SPI_Close (&g_spi0_ctrl);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);
}

volatile uint8_t g_spi_transfer_complete = 0;

void user_spi_callback(spi_callback_args_t *p_args)
{
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
    {
        g_spi_transfer_complete = 1;
    }

}

void W25Q_Delay(uint32_t time)
{
    R_BSP_SoftwareDelay (time, BSP_DELAY_UNITS_MILLISECONDS);
//    vTaskDelay (pdMS_TO_TICKS(time));
}

void csLOW(void)
{
    /* De-assert Slave select pin to stop data transfer */
    CS_DE_ASSERT(CS_PIN);
}

void csHIGH(void)
{
    /* Assert Slave select pin to start data transfer */
    CS_ASSERT(CS_PIN);
}

void SPI_Write(uint8_t *data, uint16_t length)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status
    err = R_SPI_Write (&g_spi0_ctrl, data, length, SPI_BIT_WIDTH_8_BITS);
    assert(FSP_SUCCESS == err);

    while (!g_spi_transfer_complete)
    {

    }
    g_spi_transfer_complete = 0;
}

void SPI_Read(uint8_t *data, uint32_t length)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status
    err = R_SPI_Read (&g_spi0_ctrl, data, length, SPI_BIT_WIDTH_8_BITS);
    assert(FSP_SUCCESS == err);
    while (!g_spi_transfer_complete)
    {

    }
    g_spi_transfer_complete = 0;
}

/**************************************************************************************************/

void W25Q_Reset(void)
{
    uint8_t tData[2];
    tData[0] = 0x66;  // enable Reset
    tData[1] = 0x99;  // Reset
    csLOW ();

    SPI_Write (tData, 2);
    csHIGH ();
    W25Q_Delay (100);
}

uint32_t W25Q_ReadID(void)
{
    uint8_t tData = 0x9F;  // Read JEDEC ID
    uint8_t rData[4];
    csLOW ();
    SPI_Write (&tData, 1);
    SPI_Read (rData, 4);
    csHIGH ();
    return ((rData[0] << 16) | (rData[1] << 8) | rData[2]);
}

//void W25Q_Read(uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData)
//{
//    uint8_t tData[5];
//    uint32_t memAddr = (startPage * 256) + offset;
//
//    if (numBLOCK < 512)   // Chip Size<256Mb
//    {
//        tData[0] = 0x03;  // enable Read
//        tData[1] = (memAddr >> 16) & 0xFF;  // MSB of the memory Address
//        tData[2] = (memAddr >> 8) & 0xFF;
//        tData[3] = (memAddr) & 0xFF; // LSB of the memory Address
//    }
//    else  // we use 32bit memory address for chips >= 256Mb
//    {
//        tData[0] = 0x13;  // Read Data with 4-Byte Address
//        tData[1] = (memAddr >> 24) & 0xFF;  // MSB of the memory Address
//        tData[2] = (memAddr >> 16) & 0xFF;
//        tData[3] = (memAddr >> 8) & 0xFF;
//        tData[4] = (memAddr) & 0xFF; // LSB of the memory Address
//    }
//
//    csLOW ();  // pull the CS Low
//    if (numBLOCK < 512)
//    {
//        SPI_Write (tData, 4);  // send read instruction along with the 24 bit memory address
//    }
//    else
//    {
//        SPI_Write (tData, 5);  // send read instruction along with the 32 bit memory address
//    }
//
//    SPI_Read (rData, size);  // Read the data
//    csHIGH ();  // pull the CS High
//}
//
//void W25Q_FastRead(uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData)
//{
//    uint8_t tData[6];
//    uint32_t memAddr = (startPage * 256) + offset;
//
//    if (numBLOCK < 512)   // Chip Size<256Mb
//    {
//        tData[0] = 0x0B;  // enable Fast Read
//        tData[1] = (memAddr >> 16) & 0xFF;  // MSB of the memory Address
//        tData[2] = (memAddr >> 8) & 0xFF;
//        tData[3] = (memAddr) & 0xFF; // LSB of the memory Address
//        tData[4] = 0;  // Dummy clock
//    }
//    else  // we use 32bit memory address for chips >= 256Mb
//    {
//        tData[0] = 0x0C;  // Fast Read with 4-Byte Address
//        tData[1] = (memAddr >> 24) & 0xFF;  // MSB of the memory Address
//        tData[2] = (memAddr >> 16) & 0xFF;
//        tData[3] = (memAddr >> 8) & 0xFF;
//        tData[4] = (memAddr) & 0xFF; // LSB of the memory Address
//        tData[5] = 0;  // Dummy clock
//    }
//
//    csLOW ();  // pull the CS Low
//    if (numBLOCK < 512)
//    {
//        SPI_Write (tData, 5);  // send read instruction along with the 24 bit memory address
//    }
//    else
//    {
//        SPI_Write (tData, 6);  // send read instruction along with the 32 bit memory address
//    }
//
//    SPI_Read (rData, size);  // Read the data
//    csHIGH ();  // pull the CS High
//}
//
void write_enable(void)
{
    uint8_t tData = 0x06;  // enable write
    csLOW ();
    SPI_Write (&tData, 1);
    csHIGH ();
    W25Q_Delay (5);  // 5ms delay
}

void write_disable(void)
{
    uint8_t tData = 0x04;  // disable write
    csLOW ();
    SPI_Write (&tData, 1);
    csHIGH ();
    W25Q_Delay (5);  // 5ms delay
}

//uint32_t bytestowrite(uint32_t size, uint16_t offset)
//{
//    if ((size + offset) < 256)
//        return size;
//    else
//        return 256 - offset;
//}
//
//uint32_t bytestomodify(uint32_t size, uint16_t offset)
//{
//    if ((size + offset) < 4096)
//        return size;
//    else
//        return 4096 - offset;
//}

void W25Q_Erase_Sector(uint16_t Sector, uint16_t Block, uint16_t Num_sector)
{
    uint8_t tData[6] =
    { 0 };
    uint32_t memAddr = (uint32_t) Sector * 4096 + (uint32_t) Block * 65536;

    for (int var = 0; var < Num_sector; ++var)
    {
        write_enable ();

        tData[0] = 0x20;  // Erase sector
        tData[1] = (memAddr >> 16) & 0xFF;  // MSB of the memory Address
        tData[2] = (memAddr >> 8) & 0xFF;
        tData[3] = (memAddr) & 0xFF; // LSB of the memory Address

        csLOW ();
        SPI_Write (tData, 4);
        csHIGH ();

        W25Q_Delay (450);  // 450ms delay for sector erase

        write_disable ();

        memAddr += 4095;
    }

}

void W25Q_Erase_Block_64(uint16_t Block, uint16_t Num_block)
{

    uint8_t tData[6] =
    { 0 };
    uint32_t memAddr = (uint32_t) Block * 65536;
    for (int var = 0; var < Num_block; ++var)
    {
        write_enable ();

        tData[0] = 0xd8;  // Erase block 64kb
        tData[1] = (memAddr >> 16) & 0xFF;  // MSB of the memory Address
        tData[2] = (memAddr >> 8) & 0xFF;
        tData[3] = (memAddr) & 0xFF; // LSB of the memory Address

        csLOW ();
        SPI_Write (tData, 4);
        csHIGH ();

        W25Q_Delay (2000);  // 2000 ms delay for sector erase

        write_disable ();

        memAddr += 65535;
    }

}

void W25Q_Erase_Chip(void)
{
    uint8_t tData = 0xC7;

    write_enable ();

    csLOW ();
    SPI_Write (&tData, 1);
    csHIGH ();

    W25Q_Delay (80000);  // 80000 ms delay for chip erase

    write_disable ();

}

/*******************************************************************************************************************//**
 * @brief This function writes data.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
void W25Q_Page_Program(uint16_t Page, uint16_t Sector, uint16_t Block, uint8_t *data_write, uint16_t Size)
{
    uint8_t tData[6] =
    { 0 };
    uint32_t memAddr = (uint32_t) Page * 256 + (uint32_t) Sector * 4096 + (uint32_t) Block * 65536;
    uint16_t Num_page = Size / 256 + 1;

    uint16_t Num_sector = Size / 4096 + 1;
    W25Q_Erase_Sector (Sector, Block, Num_sector);
    for (int var = 0; var < Num_page; ++var)
    {
        write_enable ();

        tData[0] = 0x02;  // PAGE PROGRAM
        tData[1] = (memAddr >> 16) & 0xFF;  // MSB of the memory Address
        tData[2] = (memAddr >> 8) & 0xFF;
        tData[3] = (memAddr) & 0xFF; // LSB of the memory Address

        csLOW ();
        SPI_Write (tData, 4);
        if (Size <= 256)
        {
            SPI_Write (data_write, Size);
        }
        else
        {
            SPI_Write (data_write, 256);
            Size -= 256;
        }
        csHIGH ();

        W25Q_Delay (5);  // 5ms delay for page program (1 page)

        write_disable ();

        memAddr += 255;
    }

}

void W25Q_Read(uint16_t Page, uint16_t Sector, uint16_t Block, uint8_t *data_read, uint16_t Size)
{
    uint8_t tData[5];
    uint32_t memAddr = (uint32_t) Page * 256 + (uint32_t) Sector * 4096 + (uint32_t) Block * 65536;

    tData[0] = 0x03;  // enable Read
    tData[1] = (memAddr >> 16) & 0xFF;  // MSB of the memory Address
    tData[2] = (memAddr >> 8) & 0xFF;
    tData[3] = (memAddr) & 0xFF; // LSB of the memory Address

    csLOW ();  // pull the CS Low

    SPI_Write (tData, 4);  // send read instruction along with the 24 bit memory address

    SPI_Read (data_read, Size);  // Read the data
    csHIGH ();  // pull the CS High
}

/*******************************************************************************************************************//**
 * @brief     This function reads the command (input) from RTT and process the command(input).
 * @param[IN]   None
 * @retval FSP_SUCCESS                  Upon successful, action of adc start or adc stop takes place
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful start or stop
 ***********************************************************************************************************************/
fsp_err_t read_process_input_from_RTT(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    uint8_t readBuff[BUFF_SIZE] =
    { RESET_VALUE };

    // Read user input
    uint32_t rByte = APP_READ(readBuff)
    ;

    /* function returns the converted integral number as an int value.*/
    int32_t inputRead = atoi ((char*) readBuff);
    unsigned NumBytes = 30;
    uint8_t g_src[NumBytes];
    uint8_t g_dest[NumBytes];
    memset (g_dest, RESET_VALUE, NumBytes);
    memset (g_src, RESET_VALUE, NumBytes);
    if (rByte > RESET_VALUE)
    {
        switch (inputRead)
        {
            case W25Q32_WRITE:
            {
                SEGGER_RTT_printf (0, "\n\rEnter string:\n\r");

                while (!SEGGER_RTT_HasKey ())
                {

                }
                NumBytes = SEGGER_RTT_Read (0, &g_src[0], NumBytes);
                if (NumBytes)
                {
                    W25Q_Page_Program (0, 0, 0, g_src, (uint16_t) NumBytes);
                    SEGGER_RTT_printf (0, "W25Q_Page_Program done!!\n\r");
                }
                APP_PRINT(MENUOPTION1);
                APP_PRINT(MENUOPTION2);
                APP_PRINT(MENUOPTION3);
                APP_PRINT(MENUOPTION4);
                break;
            }
            break;
            case W25Q32_READ:
            {
                W25Q_Read (0, 0, 0, g_dest, (uint16_t) NumBytes);
                SEGGER_RTT_printf (0, "\n\r Read date from flash: %s\n\r", g_dest);
                SEGGER_RTT_printf (0, "\n\rW25Q_Read done!!\n\r");
                memset (g_dest, RESET_VALUE, NumBytes);
                memset (g_src, RESET_VALUE, NumBytes);
                APP_PRINT(MENUOPTION1);
                APP_PRINT(MENUOPTION2);
                APP_PRINT(MENUOPTION3);
                APP_PRINT(MENUOPTION4);
                break;
            }

            default:
            {
                /* Menu for User Selection */
                APP_PRINT(MENUOPTION1);
                APP_PRINT(MENUOPTION2);
                APP_PRINT(MENUOPTION3);
                APP_PRINT(MENUOPTION4);
            }
            break;
        }
    }

    return err;
}

//void W25Q_Write_Clean(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)
//{
//    uint8_t tData[266];
//    uint32_t startPage = page;
//    uint32_t endPage = startPage + ((size + offset - 1) / 256);
//    uint32_t numPages = endPage - startPage + 1;
//
//    uint16_t startSector = startPage / 16;
//    uint16_t endSector = endPage / 16;
//    uint16_t numSectors = endSector - startSector + 1;
//    for (uint16_t i = 0; i < numSectors; i++)
//    {
//        W25Q_Erase_Sector (startSector++);
//    }
//
//    uint32_t dataPosition = 0;
//
//    // write the data
//    for (uint32_t i = 0; i < numPages; i++)
//    {
//        uint32_t memAddr = (startPage * 256) + offset;
//        uint16_t bytesremaining = bytestowrite (size, offset);
//        uint32_t indx = 0;
//
//        write_enable ();
//
//        if (numBLOCK < 512)   // Chip Size<256Mb
//        {
//            tData[0] = 0x02;  // page program
//            tData[1] = (memAddr >> 16) & 0xFF;  // MSB of the memory Address
//            tData[2] = (memAddr >> 8) & 0xFF;
//            tData[3] = (memAddr) & 0xFF; // LSB of the memory Address
//
//            indx = 4;
//        }
//
//        else // we use 32bit memory address for chips >= 256Mb
//        {
//            tData[0] = 0x12;  // page program with 4-Byte Address
//            tData[1] = (memAddr >> 24) & 0xFF;  // MSB of the memory Address
//            tData[2] = (memAddr >> 16) & 0xFF;
//            tData[3] = (memAddr >> 8) & 0xFF;
//            tData[4] = (memAddr) & 0xFF; // LSB of the memory Address
//
//            indx = 5;
//        }
//
//        uint16_t bytestosend = bytesremaining + indx;
//
//        for (uint16_t i = 0; i < bytesremaining; i++)
//        {
//            tData[indx++] = data[i + dataPosition];
//        }
//
//        if (bytestosend > 250)
//        {
//            csLOW ();
//            SPI_Write (tData, 100);
//            SPI_Write (tData + 100, bytestosend - 100);
//            csHIGH ();
//
//        }
//
//        else
//        {
//            csLOW ();
//            SPI_Write (tData, bytestosend);
//            csHIGH ();
//        }
//
//        startPage++;
//        offset = 0;
//        size = size - bytesremaining;
//        dataPosition = dataPosition + bytesremaining;
//
//        W25Q_Delay (5);
//        write_disable ();
//
//    }
//}
//
//void W25Q_Write(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)
//{
//    uint16_t startSector = page / 16;
//    uint16_t endSector = (page + ((size + offset - 1) / 256)) / 16;
//    uint16_t numSectors = endSector - startSector + 1;
//
//    uint8_t previousData[4096];
//    uint32_t sectorOffset = ((page % 16) * 256) + offset;
//    uint32_t dataindx = 0;
//
//    for (uint16_t i = 0; i < numSectors; i++)
//    {
//        uint32_t startPage = startSector * 16;
//        W25Q_FastRead (startPage, 0, 4096, previousData);
//
//        uint16_t bytesRemaining = bytestomodify (size, sectorOffset);
//        for (uint16_t i = 0; i < bytesRemaining; i++)
//        {
//            previousData[i + sectorOffset] = data[i + dataindx];
//        }
//
//        W25Q_Write_Clean (startPage, 0, 4096, previousData);
//
//        startSector++;
//        sectorOffset = 0;
//        dataindx = dataindx + bytesRemaining;
//        size = size - bytesRemaining;
//    }
//}
//
//uint8_t W25Q_Read_Byte(uint32_t Addr)
//{
//    uint8_t tData[5];
//    uint8_t rData;
//
//    if (numBLOCK < 512)   // Chip Size<256Mb
//    {
//        tData[0] = 0x03;  // enable Read
//        tData[1] = (Addr >> 16) & 0xFF;  // MSB of the memory Address
//        tData[2] = (Addr >> 8) & 0xFF;
//        tData[3] = (Addr) & 0xFF; // LSB of the memory Address
//    }
//    else  // we use 32bit memory address for chips >= 256Mb
//    {
//        tData[0] = 0x13;  // Read Data with 4-Byte Address
//        tData[1] = (Addr >> 24) & 0xFF;  // MSB of the memory Address
//        tData[2] = (Addr >> 16) & 0xFF;
//        tData[3] = (Addr >> 8) & 0xFF;
//        tData[4] = (Addr) & 0xFF; // LSB of the memory Address
//    }
//
//    csLOW ();  // pull the CS Low
//    if (numBLOCK < 512)
//    {
//        SPI_Write (tData, 4);  // send read instruction along with the 24 bit memory address
//    }
//    else
//    {
//        SPI_Write (tData, 5);  // send read instruction along with the 32 bit memory address
//    }
//
//    SPI_Read (&rData, 1);  // Read the data
//    csHIGH ();  // pull the CS High
//
//    return rData;
//}
//
//void W25Q_Write_Byte(uint32_t Addr, uint8_t data)
//{
//    uint8_t tData[6];
//    uint8_t indx;
//
//    if (numBLOCK < 512)   // Chip Size<256Mb
//    {
//        tData[0] = 0x02;  // page program
//        tData[1] = (Addr >> 16) & 0xFF;  // MSB of the memory Address
//        tData[2] = (Addr >> 8) & 0xFF;
//        tData[3] = (Addr) & 0xFF; // LSB of the memory Address
//        tData[4] = data;
//        indx = 5;
//    }
//    else  // we use 32bit memory address for chips >= 256Mb
//    {
//        tData[0] = 0x12;  // Write Data with 4-Byte Address
//        tData[1] = (Addr >> 24) & 0xFF;  // MSB of the memory Address
//        tData[2] = (Addr >> 16) & 0xFF;
//        tData[3] = (Addr >> 8) & 0xFF;
//        tData[4] = (Addr) & 0xFF; // LSB of the memory Address
//        tData[5] = data;
//        indx = 6;
//    }
//
//    if (W25Q_Read_Byte (Addr) == 0xFF)
//    {
//        write_enable ();
//        csLOW ();
//        SPI_Write (tData, indx);
//        csHIGH ();
//
//        W25Q_Delay (5);
//        write_disable ();
//    }
//}
//
//uint8_t tempBytes[4];
//
//void float2Bytes(uint8_t *ftoa_bytes_temp, float float_variable)
//{
//    union
//    {
//        float a;
//        uint8_t bytes[4];
//    } thing;
//
//    thing.a = float_variable;
//
//    for (uint8_t i = 0; i < 4; i++)
//    {
//        ftoa_bytes_temp[i] = thing.bytes[i];
//    }
//
//}
//
//float Bytes2float(uint8_t *ftoa_bytes_temp)
//{
//    union
//    {
//        float a;
//        uint8_t bytes[4];
//    } thing;
//
//    for (uint8_t i = 0; i < 4; i++)
//    {
//        thing.bytes[i] = ftoa_bytes_temp[i];
//    }
//
//    float float_variable = thing.a;
//    return float_variable;
//}
//
//void W25Q_Write_NUM(uint32_t page, uint16_t offset, float data)
//{
//    float2Bytes (tempBytes, data);
//
////  /* write using single byte function */
////  uint32_t Addr = (page*256)+offset;
////  for (int i=0; i<4; i++)
////  {
////      W25Q_Write_Byte(i+Addr, tempBytes[i]);
////  }
//
//    /* Write using sector update function */
//    W25Q_Write (page, offset, 4, tempBytes);
//}
//
//float W25Q_Read_NUM(uint32_t page, uint16_t offset)
//{
//    uint8_t rData[4];
//    W25Q_Read (page, offset, 4, rData);
//    return (Bytes2float (rData));
//}
//
//void W25Q_Write_32B(uint32_t page, uint16_t offset, uint32_t size, uint32_t *data)
//{
//    uint8_t data8[size * 4];
//    uint32_t indx = 0;
//
//    for (uint32_t i = 0; i < size; i++)
//    {
//        data8[indx++] = data[i] & 0xFF;   // extract LSB
//        data8[indx++] = (data[i] >> 8) & 0xFF;
//        data8[indx++] = (data[i] >> 16) & 0xFF;
//        data8[indx++] = (data[i] >> 24) & 0xFF;
//    }
//
//    W25Q_Write (page, offset, indx, data8);
//}
//
//void W25Q_Read_32B(uint32_t page, uint16_t offset, uint32_t size, uint32_t *data)
//{
//    uint8_t data8[size * 4];
//    uint32_t indx = 0;
//
//    W25Q_FastRead (page, offset, size * 4, data8);
//
//    for (uint32_t i = 0; i < size; i++)
//    {
//        data[i] = (data8[indx++]) | (data8[indx++] << 8) | (data8[indx++] << 16) | (data8[indx++] << 24);
//    }
//}
