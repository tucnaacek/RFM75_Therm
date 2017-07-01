#ifndef SHT1XH
#define SHT1XH

// Registers mapping

#include "stdint.h"
#include "stdint-gcc.h"
#include "wirelesstmp.h"

#define		SHT_RADDR	0x//0xa1	//0x1D
#define		SHT_WADDR	0x//0xa0	//0x1C

/************************************************************************************************************************
 ************************************************************************************************************************/
#ifdef STM32F051
    #define SHT_SDA_LOW			  SHT_GPIO->BSRRH = SHT_SDA
    #define SHT_SDA_HI			  SHT_GPIO->BSRRL = SHT_SDA
    #define SHT_SCK_LOW			  SHT_GPIO->BSRRH = SHT_SCK
    #define SHT_SCK_HI			  SHT_GPIO->BSRRL = SHT_SCK
    #define SHT_SCK_SDA_HI		(SHT_GPIO->BSRRL = SHT_SCK | SHT_SDA)
    #define SHT_SCK_SDA_LOW		SHT_GPIO->BSRRH = SHT_SCK | SHT_SDA
#endif

    #define SHT_SDA_LOW			  SHT_GPIO->BRR |= SHT_SDA
    #define SHT_SDA_HI			  SHT_GPIO->BSRR |= SHT_SDA
    #define SHT_SCK_LOW			  SHT_GPIO->BRR |= SHT_SCK
    #define SHT_SCK_HI			  SHT_GPIO->BSRR |= SHT_SCK
    #define SHT_SCK_SDA_HI		SHT_GPIO->BSRR |= (SHT_SCK | SHT_SDA)
    #define SHT_SCK_SDA_LOW		SHT_GPIO->BRR |= (SHT_SCK | SHT_SDA)

typedef volatile struct {
    uint8_t Status;
	uint8_t DeviceAddress;
	uint16_t Temperature;   /* File copy buffer */
	uint16_t Humidity;
} SHT_Struct;

SHT_Struct *p_SHT_Struct;


/**************************************************
SHT1x Commands definition
 *************************************************/
#define     RESERVED        0x00
#define     TEMP_MEAS       0x03
#define     HUM_MEAS        0x05
#define     READ_STATUS     0x07
#define     WRITE_STATUS    0x06
#define     SOFT_RESET      0x1e

void SHT_Init(void);
void SHT_GenerateSTART(void);
Acknowledge SHT_GetACK(void);
void SHT_ResetConnection(void);
void SHT_WriteStatus(uint8_t Status);
uint8_t SHT_ReadStatus(void);
void SHT_WriteByte(uint8_t Byte);
uint16_t SHT_ReadTemperature(void);
uint16_t SHT_ReadTHumidity(void);
uint16_t SHT_Read16(void);
void SHT_Write16(void);
#endif
