#ifndef I2CH
#define I2CH

#include "stdint.h"
#include "stdint-gcc.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cm3.h"

//#include "stm32f4xx_misc.h"
#include "loops.h"
#include "wirelesstmp.h"


#define CLK_LENGTH		1
#define	I2C_TIMEOUT		200


#ifdef STM407
  #define SDA_LOW			I2C_PORT->BRR = I2C_SDA_PIN
  #define SDA_HI			I2C_PORT->BSRR = I2C_SDA_PIN
  #define SCL_LOW			I2C_PORT->BRR = I2C_SCL_PIN
  #define SCL_HI			I2C_PORT->BSRR = I2C_SCL_PIN
  #define SCL_SDA_HI		(I2C_PORT->BSRR = I2C_SCL_PIN | I2C_SDA_PIN)
  #define SCL_SDA_LOW		I2C_PORT->BRR = I2C_SCL_PIN | I2C_SDA_PIN
#endif

  #define SDA_LOW			  I2C_PORT->BSRRH = I2C_SDA_PIN
  #define SDA_HI			  I2C_PORT->BSRRL = I2C_SDA_PIN
  #define SCL_LOW			  I2C_PORT->BSRRH = I2C_SCL_PIN
  #define SCL_HI			  I2C_PORT->BSRRL = I2C_SCL_PIN
  #define SCL_SDA_HI		(I2C_PORT->BSRRL = I2C_SCL_PIN | I2C_SDA_PIN)
  #define SCL_SDA_LOW		I2C_PORT->BSRRH = I2C_SCL_PIN | I2C_SDA_PIN


//function prototype

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
	ACK = 0,
	NOACK = 1,
} Acknowledge;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
    PB10_11 = 0,
    PB6_7 = 1,
    PB8_9 = 2
} I2C_PinSource;

/************************************************************************************************************************
 ************************************************************************************************************************/


void InitI2C_SW(I2C_PinSource PinSource);
uint8_t I2C_GenerateSTART_SW(void);
void I2C_GenerateSTOP_SW(void);
void I2C_GenerateACK_SW(Acknowledge Ack);
Acknowledge I2C_GetACK_SW(void); //state
void I2C_WriteByte_SW(uint8_t Byte);
uint8_t I2C_WriteRegister_SW(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t Data);
uint8_t I2C_WriteSequnce_SW(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t TableIndex, uint8_t *Data, uint8_t DataNumber);
uint8_t I2C_ReadByte_SW(void);
uint8_t I2C_ReadRegister_SW(uint8_t DeviceAddress, uint8_t RegisterAddres);
uint8_t I2C_ReadSequence_SW(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t *DataBuff, uint8_t DataNumber);
void I2C_WriteMultipleBytes_SW(uint8_t* Data, uint8_t NumberOfBytes);
void I2C_ReadMultipleBytes_SW(uint16_t DeviceAddress, uint8_t NumberOfBytes);


#endif
