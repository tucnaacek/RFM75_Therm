#ifndef WTEMPH
#define WTEMPH
//CPU STM32F207VGT6
//package LQFP100

#include "stdint.h"
#include "stdint-gcc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_spi.h"

// define data buffer lenght
#define     LEN     1
#define     BUF_LENGTH      50
#define     LED_ON     0
#define     LED_OFF    1

#define     TXMODE
//#define     RXMODE
//

#define     SW_SPI
#define     SW_I2C

#define f_size(fp) ((fp)->fsize)

#define BSWAP16(n) ((n) << 8 | ((n) >> 8 & 0x00FF))
#define BSWAP32(n) ((n) << 16 | ((n) >> 16 & 0x0000FFFF))

//#define NULL ( (void *) 0) defined in stddef.h

//Ports and periphery defs

/************************************************************************************************************************
 ************************************************************************************************************************/
#define	    MEM_GPIO		    GPIOA
#define     MEM_I2C             I2C1

/************************************************************************************************************************
    RFM module pin definition
 ************************************************************************************************************************/
#define     RFM_SPI             SPI1
#define     RFM_SPI_Direction   SPI_Direction_2Lines_FullDuplex
#define     RFM_SPI_Mode        SPI_Mode_Master
#define     RFM_SPI_DataSize    SPI_DataSize_8b
#define     RFM_SPI_Cpol        SPI_CPOL_Low
#define     RFM_SPI_Cpha        SPI_CPHA_1Edge
#define     RFM_SPI_Nss         SPI_NSS_Soft //| SPI_NSSInternalSoft_Set
#define     RFM_SPI_BaudRate    SPI_BaudRatePrescaler_256//256
#define     RFM_SPI_FirstBit    SPI_FirstBit_MSB
//#define     RFM_SPI_CRCPoly     7
#define     RFM_SPI_GPIO        GPIOA//SCK+MISO+MOSI
#define     RFM_CTRL_GPIO       GPIOA//NRST
#define     RFM_RCC_SPI         RCC_APB2Periph_SPI1
#define     RFM_RCC_SPI_GPIO    RCC_APB2Periph_GPIOA//RCC_AHB1Periph_GPIOARCC_APB2Periph_SPI1
#define     RFM_RCC_SPI_CTRL    RCC_APB2Periph_GPIOA//RCC_AHB1Periph_GPIOB
#define     RFM_SPI_GPIOOType   GPIO_Mode_AF_PP
#define     RFM_SPI_GPIOSpeed   GPIO_Speed_50MHz//50 // 100 //25
#define     RFM_SPI_GPIOPuPd    GPIO_PuPd_NOPULL
#define     RFM_SPI_GpioAltFcn  GPIO_AF_0//GPIO_AF_SPI1

//***********************************************************
#define     RFM_CE              GPIO_Pin_2
#define     CE                  RFM_CTRL_GPIO, RFM_CE
#define     RFM_CE_PIN_SRC      GPIO_PinSource2
//***********************************************************
#define     RFM_CSN             GPIO_Pin_4
#define     CSN                 RFM_CTRL_GPIO, RFM_CSN
#define     RFM_CSN_PIN_SRC     GPIO_PinSource4
//***********************************************************
#define     RFM_IRQ             GPIO_Pin_3
#define     IRQ                 RFM_CTRL_GPIO, RFM_REQN
//***********************************************************
#define     RFM_SCK             GPIO_Pin_5
#define     SCK                 RFM_SPI, RFM_SCK
#define     RFM_SCK_PIN_SRC     GPIO_PinSource5
//***********************************************************
#define     RFM_MOSI            GPIO_Pin_7
#define     MOSI                RFM_SPI, RFM_MOSI
#define     RFM_MOSI_PIN_SRC    GPIO_PinSource7
//***********************************************************
#define     RFM_MISO            GPIO_Pin_6
#define     MISO                RFM_SPI, RFM_MISO
#define     RFM_MISO_PIN_SRC    GPIO_PinSource6
//***********************************************************
#define     SHT_GPIO            GPIOB
#define     SHT_RCC_GPIO        RCC_APB2Periph_GPIOB// GPIOB//
#define     SHT_SDA             GPIO_Pin_7
#define     SHT_SDA_PIN_SRC     GPIO_PinSource7
#define     SHTSDA              SHT_GPIO, SHT_SDA
#define     SHT_SCK             GPIO_Pin_6
#define     SHT_SCK_PIN_SRC     GPIO_PinSource6
#define     SHTSCK              SHT_GPIO, SHT_SCK
//***********************************************************

#define     LEDPORT             GPIOB
#define     SYS_LED1            GPIO_Pin_13
#define     SYS_LED2            GPIO_Pin_14
#define     SYSLED1             LEDPORT, SYS_LED1
#define     SYSLED2             LEDPORT, SYS_LED2

//#define     SHT_SCK_GPIOOType   GPIO_OType_PP
#define     SHT_GPIOSpeed       GPIO_Speed_50MHz//50 // 100 //25
#define     SHT_SCK_GPIOPuPd    GPIO_PuPd_NOPULL
#define     SHT_SCK_GPIOMode    GPIO_Mode_Out_PP


//#define     SHT_SDA_GPIOOType   GPIO_OType_OD
#define     SHT_SDA_GPIOPuPd    GPIO_PuPd_NOPULL
#define     SHT_SDA_GPIOMode    GPIO_Mode_Out_OD

#define     ZERO5               0, 0, 0, 0, 0
#define     ZERO32              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define     DATA32              0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31

/************************************************************************************************************************
    24LC0x memory pin definition
 ************************************************************************************************************************/


/************************************************************************************************************************
    SHT1x pin definition
 ************************************************************************************************************************/



/************************************************************************************************************************
 ************************************************************************************************************************/
//#define		HSI_VALUE	8000000
//#define     HSE_VALUE    ((uint32_t)16000000)

#define 	RFM_HW_RESET 	0
#define     RFM_SW_RESET    1

#define 	TWO_PI 			(3.14159 * 2)
#define     ROOMNMB         10
/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
	SETTING_SUCCESFULL = 0,
	BAD_RANGE_PARAMETER = -1,
	BAD_OBJECT_POINTER = -2,
} FunctionState;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
    LOW = 0,
	HI = 1,
} State;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
    ACK = 0,
	NOACK = 1,
} Acknowledge;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
	NO_MEMORY_DETECTED,
	MEMORY_INITIALIZED,
	MEMORY_EMPTY
} MemoryStatus;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
	OFF = 0,
	ON = 1,
} OnOff;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
  MEM_ERROR = 0,
  MEASURE,
} Mode;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef union {
	struct {
		uint32_t LittleEndian:          1;
	}Fb;
	uint32_t flags;
} FlagBits;

FlagBits Flags;

/************************************************************************************************************************
 ************************************************************************************************************************/
//void InitGPIOs(GPIO_TypeDef *Portx, uint16_t Pins, GPIOMode_TypeDef Mode, GPIOOType_TypeDef Otype);

/************************************************************************************************************************
 ************************************************************************************************************************/
void InvertLEDs(GPIO_TypeDef *Portx, uint16_t Pin);
void SetPinValue(GPIO_TypeDef *Portx, uint16_t Pin, State LowHigh);
#endif /* INVERTORH */
