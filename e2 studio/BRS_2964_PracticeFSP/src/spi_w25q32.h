/*
 * spi_w25q32.h
 *
 *  Created on: Apr 12, 2024
 *      Author: giang.nguyen-hoang
 */

#ifndef SPI_W25Q32_H_
#define SPI_W25Q32_H_

#include "common_utils.h"

void spi_w25q32_init(void);
void spi_w25q32_deinit(void);


/* Macros for Commands to be received through RTT inputs */
#define W25Q32_WRITE                      0x01
#define W25Q32_READ                       0x02

/* Macro for default buff size for reading through RTT */
#define BUFF_SIZE                       0x0F

/* Macros for menu options to be displayed */
#define MENUOPTION1       "\r\nMENU to Select\r\n"
#define MENUOPTION2       "Press 1 to Write string to Flash module\r\n"
#define MENUOPTION3       "Press 2 to Read string wrote from Flash module\r\n"
#define MENUOPTION4       "User Input :"



/**
 * @brief W25Q and W25X Series Drive (The W25Q family is a "superset" of the W25X family)
 *
 *              Number of supported device mounts (1)
 *
 *              Standard SPI  (¡Ì)      3ByteAddress (¡Ì)
 *              Dual     SPI  (x)      4ByteAddress (¡Ì)
 *              Quad     SPI  (x)
 * Note:
 * 1. 3 address mode and 4 address mode are not supported to switch between each
 *    other when the program is running, this version uses the macro to select the
 *    address mode.
 * 2. In the 3-address mode, the data with address over 0xFFFFFF will be accessed
 *    by pre-setting the extended registers by default.
 *
 */
#define W25QXX_FASTREAD                              0      /* 0 : No Fast Read Mode   ; 1 : Fast Read Mode */
#define W25QXX_4BADDR                                0      /* 0 : 3 Byte Address Mode ; 1 : 4 Byte Address Mode */
#define W25QXX_SUPPORT_SFDP                          0      /* 0 : No support SFDP     ; 1 : Support SFDP */

/**
 * @brief W25Qxx CMD
 */
#define W25Q_CMD_WEN                                 0x06
#define W25Q_CMD_VOLATILESREN                        0x50
#define W25Q_CMD_WDEN                                0x04
#define W25Q_CMD_POWEREN                             0xAB
#define W25Q_CMD_MANUFACTURER                        0x90
#define W25Q_CMD_JEDECID                             0x9F
#define W25Q_CMD_UNIQUEID                            0x4B
#define W25Q_CMD_READ                                0x03
#define W25Q_CMD_4BREAD                              0x13
#define W25Q_CMD_FASTREAD                            0x0B
#define W25Q_CMD_4BFASTREAD                          0x0C
#define W25Q_CMD_WPAGE                               0x02
#define W25Q_CMD_4BWPAGE                             0x12
#define W25Q_CMD_ESECTOR                             0x20
#define W25Q_CMD_4BESECTOR                           0x21
#define W25Q_CMD_E32KBLOCK                           0x52
#define W25Q_CMD_E64KBLOCK                           0xD8
#define W25Q_CMD_4BE64KBLOCK                         0xDC
#define W25Q_CMD_ECHIP                               0xC7   /* or 0x60 */
#define W25Q_CMD_RSREG1                              0x05
#define W25Q_CMD_WSREG1                              0x01
#define W25Q_CMD_RSREG2                              0x35
#define W25Q_CMD_WSREG2                              0x31
#define W25Q_CMD_RSREG3                              0x15
#define W25Q_CMD_WSREG3                              0x11
#define W25Q_CMD_REXTREG                             0xC8
#define W25Q_CMD_WEXTREG                             0xC5
#define W25Q_CMD_RSFDP                               0x5A
#define W25Q_CMD_ESECREG                             0x44
#define W25Q_CMD_WSECREG                             0x42
#define W25Q_CMD_RSECREG                             0x48
#define W25Q_CMD_WALLBLOCKLOCK                       0x7E
#define W25Q_CMD_WALLBLOCKUNLOCK                     0x98
#define W25Q_CMD_RBLOCKLOCK                          0x3D
#define W25Q_CMD_WSIGBLOCKLOCK                       0x36
#define W25Q_CMD_WSIGBLOCKUNLOCK                     0x39
#define W25Q_CMD_EWSUSPEND                           0x75
#define W25Q_CMD_EWRESUME                            0x7A
#define W25Q_CMD_POWERDEN                            0xB9
#define W25Q_CMD_4ByteAddrEN                         0xB7
#define W25Q_CMD_4ByteAddrDEN                        0xE9
#define W25Q_CMD_ENRESET                             0x66
#define W25Q_CMD_RESETDEV                            0x99
#define W25Q_DUMMY                                   0xA5

/**
 * @brief W25Qxx SIZE
 */
#define W25Qxx_PAGESIZE                              0x00100
#define W25Qxx_PAGEPOWER                             0x08
#define W25Qxx_SECTORSIZE                            0x01000
#define W25Qxx_SECTORPOWER                           0x0C
#define W25Qxx_BLOCKSIZE                             0x10000
#define W25Qxx_BLOCKPOWER                            0x10
#define W25Qxx_SECTURITYSIZE                         0x00300

/**
 * @brief W25Qxx Address Convert
 */
#define W25Qxx_PAGEADDR(ByteAddr)                    (uint32_t)(ByteAddr >> W25Qxx_PAGEPOWER)               /* Convert Byte address to page address */
#define W25Qxx_SECTORADDR(ByteAddr)                  (uint32_t)(ByteAddr >> W25Qxx_SECTORPOWER)             /* Convert Byte address to sector address */
#define W25Qxx_BLOCK64ADDR(ByteAddr)                 (uint32_t)(ByteAddr >> W25Qxx_BLOCKPOWER)              /* Convert Byte address to block64 address */
#define W25Qxx_BLOCK32ADDR(ByteAddr)                 (uint32_t)(ByteAddr >> (W25Qxx_BLOCKPOWER - 1))        /* Convert Byte address to block32 address */

/**
 * @brief W25Qxx Chip Type
 */
typedef enum
{
    UNKNOWN = 0x000000,
    W25X05 = 0xEFFF10,                               /* W25X05 */
    W25X10 = 0xEFFF11,                               /* W25X10 */
    W25Q20 = 0xEFFF12,                               /* W25Q20 */
    W25Q40 = 0xEFFF13,                               /* W25Q40 */
    W25Q80 = 0xEFFF14,                               /* W25Q80 */
    W25Q16 = 0xEFFF15,                               /* W25Q16 */
    W25Q32 = 0xEFFF16,                               /* W25Q32 */
    W25Q64 = 0xEFFF17,                               /* W25Q64 */
    W25Q128 = 0xEFFF18,                              /* W25Q128 ( 0xEF4018 0xEF7018 ) */
    W25Q256 = 0xEFFF19,                              /* W25Q256 */
    W25Q512 = 0xEFFF20,                              /* W25Q512 */
    W25Q01 = 0xEFFF21,                               /* W25Q01 */
    W25Q02 = 0xEFFF22                                /* W25Q02 */
} W25Qxx_CHIP;

/**
 * @brief W25Qxx Error Information
 */
typedef enum
{
    W25Qxx_ERR_NONE = 0x00,                          /* No error */
    W25Qxx_ERR_STATUS = 0x02,                        /* Device is suspended */
    W25Qxx_ERR_LOCK = 0x03,                          /* Device is locked */
    W25Qxx_ERR_INVALID = 0x04,                       /* Invalid value entered */
    W25Qxx_ERR_CHIPNOTFOUND = 0x05,                  /* Chip Model is not found */
    W25Qxx_ERR_BYTEADDRBOUND = 0x06,                 /* Byte address out of bounds */
    W25Qxx_ERR_PAGEADDRBOUND = 0x07,                 /* Page address out of bounds */
    W25Qxx_ERR_SECTORADDRBOUND = 0x08,               /* Sector address out of bounds */
    W25Qxx_ERR_BLOCK64ADDRBOUND = 0x09,              /* Block64 address out of bounds */
    W25Qxx_ERR_BLOCK32ADDRBOUND = 0x0A,              /* Block32 address out of bounds */
    W25Qxx_ERR_WPSMODE = 0x0B,                       /* Write protect mode is error */
    W25Qxx_ERR_HARDWARE = 0x0C                       /* SPI/QSPI BUS is hardware error */
} W25Qxx_ERR;

/**
 * @brief W25Qxx Status Information
 */
typedef enum
{
    W25Qxx_STATUS_IDLE = 0x01,
    W25Qxx_STATUS_BUSY = 0x02,
    W25Qxx_STATUS_SUSPEND = 0x04,
    W25Qxx_STATUS_BUSY_AND_SUSPEND = 0x08
} W25Qxx_STATUS;

/**
 * @brief W25Qxx StatusRegister Storage Mode
 */
typedef enum
{
    W25Qxx_VOLATILE = 0x00,                          /* StatusRegister Voliate Mode */
    W25Qxx_NON_VOLATILE = 0x01                       /* StatusRegister Non-Voliate Mode */
} W25Qxx_SRM;

/**
 * @brief W25Qxx Chip Parameter
 */
typedef struct
{
    W25Qxx_CHIP type;                                /* Chip type */
    uint32_t ProgrMaxTimePage;                       /* Program       max time (ms) */
    uint32_t EraseMaxTimeSector;                     /* Erase Sector  max time (ms) */
    uint32_t EraseMaxTimeBlock64;                    /* Erase Block64 max time (ms) */
    uint32_t EraseMaxTimeBlock32;                    /* Erase Block32 max time (ms) */
    uint32_t EraseMaxTimeChip;                       /* Erase Chip    max time (ms) */
} W25Qxx_INFO_t;

/**
 * @brief W25Qxx BUS Port
 */
typedef struct
{
    void (*spi_delayms)(uint32_t ms);
    void (*spi_cs_H)(void);
    void (*spi_cs_L)(void);
    uint8_t(*spi_rw)(uint8_t data);
} W25Qxx_PORT_t;

/**
 * @brief W25Qxx Chip Information
 */
typedef struct
{
    W25Qxx_INFO_t info;                              /* Chip Parameter */
    W25Qxx_PORT_t port;                              /* BUS Port */
    uint16_t IDManufacturer;                         /* Manufacturer ID */
    uint32_t IDJEDEC;                                /* JEDEC        ID */
    uint64_t IDUnique;                               /* Unique       ID */
    uint32_t numBlock;                               /* Block  number */
    uint32_t numPage;                                /* Page   number */
    uint32_t numSector;                              /* Sector number */
    uint16_t sizePage;                               /* Page   size (Byte) */
    uint32_t sizeSector;                             /* Sector size (Byte) */
    uint32_t sizeBlock;                              /* Block  size (Byte) */
    uint32_t sizeChip;                               /* Chip   size (Byte) */
    uint8_t StatusRegister1;                         /* StatusRegister 1 */
    uint8_t StatusRegister2;                         /* StatusRegister 2 */
    uint8_t StatusRegister3;                         /* StatusRegister 3 */
    uint8_t ExtendedRegister;                        /* ExtendedRegister */
} W25Qxx_t;





void W25Q_Reset (void);
uint32_t W25Q_ReadID (void);

void W25Q_Erase_Sector(uint16_t Sector, uint16_t Block, uint16_t Num_sector);
void W25Q_Erase_Block_64(uint16_t Block, uint16_t Num_block);
void W25Q_Erase_Chip(void);

void W25Q_Page_Program(uint16_t Page, uint16_t Sector, uint16_t Block, uint8_t *data_write, uint16_t Size);
void W25Q_Read(uint16_t Page, uint16_t Sector, uint16_t Block, uint8_t *data_read, uint16_t Size);
fsp_err_t read_process_input_from_RTT(void);
//void W25Q_Read (uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData);
//void W25Q_FastRead (uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData);
//
//
//void W25Q_Erase_Sector (uint16_t numsector);
//
//void W25Q_Write_Clean(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);
//void W25Q_Write (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);
//
//void W25Q_Write_Byte (uint32_t Addr, uint8_t data);
//uint8_t W25Q_Read_Byte (uint32_t Addr);
//
//float W25Q_Read_NUM (uint32_t page, uint16_t offset);
//void W25Q_Write_NUM (uint32_t page, uint16_t offset, float data);
//
//void W25Q_Read_32B (uint32_t page, uint16_t offset, uint32_t size, uint32_t *data);
//void W25Q_Write_32B (uint32_t page, uint16_t offset, uint32_t size, uint32_t *data);


#endif /* SPI_W25Q32_H_ */
