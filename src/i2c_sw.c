#include "stm32f10x_i2c.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "wirelesstmp.h"
#include "i2c_SW.h"
//#include "mma8452.h"

uint32_t	I2CTickCounter, TimeOutCounter;
uint8_t I2C_buffer[50];
#ifdef DEBUG_MMA8452
uint8_t I2C_buffer_dbg[50], I2C_buffer1_dbg[20];
#endif


void InitI2C_SW(I2C_PinSource PinSource){
}

/*******************************************************************************
 *******************************************************************************/
uint8_t I2C_GenerateSTART_SW(void){
	//SDA_HI;
	SCL_SDA_HI;//SCL_HI;
	I2CTickCounter = 0;
	while((I2CTickCounter < CLK_LENGTH) && (TimeOutCounter < I2C_TIMEOUT))
		;
	SDA_LOW;
	I2CTickCounter = 0;
	while((I2CTickCounter < CLK_LENGTH) && (TimeOutCounter < I2C_TIMEOUT))
		;

	SCL_LOW;
	return 0;
}

/*******************************************************************************
 *******************************************************************************/
void I2C_GenerateSTOP_SW(void){
	I2C_PORT->BRR |= (I2C_SDA_PIN | I2C_SCL_PIN);// booth pins to HIGH state

	I2CTickCounter = 0;
	while((I2CTickCounter < CLK_LENGTH) && (TimeOutCounter < I2C_TIMEOUT))
		;

	I2CTickCounter = 0;
	SCL_HI;
	while((I2CTickCounter < CLK_LENGTH) && (TimeOutCounter < I2C_TIMEOUT))
		;
	SDA_HI;
}

/*******************************************************************************
 *******************************************************************************/
void I2C_GenerateACK_SW(Acknowledge Ack){
//	uint8_t State;
	SCL_LOW;
	I2CTickCounter = 0;
	while(I2CTickCounter < 1) //wait half of period
		;
	if(Ack)
		SDA_HI;//I2C_PORT->BSRR |= I2C_SDA_PIN; //generate NO ACK
	else
		SDA_LOW;//I2C_PORT->BRR |= I2C_SDA_PIN; // generate ACK

	I2CTickCounter = 0;
	while(I2CTickCounter < 1) //wait half of period
		;
	SCL_HI;//I2C_PORT->BSRR |= I2C_SCL_PIN;	//set clock to HIGH

	I2CTickCounter = 0;
	while(I2CTickCounter < CLK_LENGTH) //wait half of period
		;
	SCL_LOW;
	SDA_HI;
}

/*******************************************************************************
 *******************************************************************************/
Acknowledge I2C_GetACK_SW(void){
	uint8_t State;
//	SDA_HI; //no affect of CPU to ACK
	I2CTickCounter = 0;
	while(I2CTickCounter < CLK_LENGTH) //wait half of period
		;
	SCL_SDA_HI;//I2C_PORT->BSRR |= I2C_SCL_PIN;	//set clock to HIGH

	I2CTickCounter = 0;
	while(I2CTickCounter < CLK_LENGTH) //wait half of period
		;
	if((((I2C_PORT->IDR) & I2C_SDA_PIN) == I2C_SDA_PIN)) //check ACK
		State = NOACK; //no ACK
	else
		State = ACK;	// ACK was detected
	I2CTickCounter = 0;
	while(I2CTickCounter < CLK_LENGTH)// wait half of period
		;
	SCL_LOW;
	return State;
}

/*******************************************************************************
 *******************************************************************************/
void I2C_WriteByte_SW(uint8_t Byte){
	uint8_t i, DataRegister = Byte;
	SCL_LOW;
	for(i = 0; i < 8; i++){
		if(((DataRegister << i) & 0x80) == 0x80)
			SDA_HI;//I2C_PORT->BSRR |= I2C_SDA_PIN;
		else
			SDA_LOW;//I2C_PORT->BRR |= I2C_SDA_PIN; // set data bit
		I2CTickCounter = 0;
		while(I2CTickCounter < CLK_LENGTH) //wait half of period
			;
		SCL_HI;//I2C_PORT->BSRR |= I2C_SCL_PIN;	//set clock to HIGH
		I2CTickCounter = 0;
		while(I2CTickCounter < CLK_LENGTH)// wait half of period
			;
		SCL_LOW;//I2C_PORT->BRR |= I2C_SCL_PIN; //SCL to LOW
	}
}

/*******************************************************************************
 *******************************************************************************/
uint8_t I2C_WriteSequnce_SW(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t TableIndex, uint8_t *Data, uint8_t DataNumber){
	uint8_t RetVal = 0;
	I2C_GenerateSTART_SW();//start communication

	I2C_WriteByte_SW(DeviceAddress); //send device address
	if(I2C_GetACK_SW() == NOACK)	// check acknowledge
		return 1;	//no acknowledge

	I2C_WriteByte_SW(RegisterAddress); //send device address
	if(I2C_GetACK_SW() == NOACK)	// check acknowledge
		return 1;	//no acknowledge

	while(DataNumber--){
		I2C_WriteByte_SW((Data[TableIndex++]));
		if(I2C_GetACK_SW() == NOACK)	{// check acknowledge
			RetVal = 1;//return 1;	//no acknowledge
			break;	//break loop
		}
	}

	I2C_GenerateSTOP_SW();// stop communication
	return RetVal;
}

/*******************************************************************************
 *******************************************************************************/
uint8_t I2C_ReadByte_SW(void){
	uint8_t i, Data;
	for(i = 0; i < 8; i++){
		I2CTickCounter = 0;
		while(I2CTickCounter < CLK_LENGTH)// wait half of period
			;
		I2CTickCounter = 0;
		SCL_HI;// I2C_PORT->BSRR |= I2C_SCL_PIN;	//set clock to HIGH
		while(I2CTickCounter < CLK_LENGTH) //wait half of period
			;

		if((((I2C_PORT->IDR) & I2C_SDA_PIN) == I2C_SDA_PIN)) //check ACK
			Data |= (0x80 >> i); //no ACK
		else
			Data &= ~(0x80 >> i);	// ACK was detected

		I2CTickCounter = 0;
		while(I2CTickCounter < CLK_LENGTH)// wait half of period
			;
		SCL_LOW;
	}
	return Data;
}

/*******************************************************************************
 *******************************************************************************/
uint8_t I2C_ReadSequence_SW(uint8_t Address, uint8_t RegisterAddress, uint8_t *DataBuff, uint8_t DataNumber){
	uint8_t Data;
	I2C_GenerateSTART_SW();//start communication
	I2C_WriteByte_SW(MMA8452_WADDR); //send device address
	if(I2C_GetACK_SW() == NOACK)	// check acknowledge
		return 1;	//no acknowledge

	I2C_WriteByte_SW(RegisterAddress); //send device address
	if(I2C_GetACK_SW() == NOACK)	// check acknowledge
		return 1;	//no acknowledge

	I2C_GenerateSTART_SW();// repeat start condition
	I2C_WriteByte_SW(MMA8452_RADDR); //send device address for data reading
	if(I2C_GetACK_SW() == NOACK)	// check acknowledge
		return 1;	//no acknowledge

/*******************************************************************************
 *******************************************************************************/
	while(DataNumber--){
		*(DataBuff++) = I2C_ReadByte_SW();
		if(DataNumber != 0)
			I2C_GenerateACK_SW(ACK);
	}
	I2C_GenerateACK_SW(NOACK);

	I2C_GenerateSTOP_SW();
	return Data;
}

/*******************************************************************************
 *******************************************************************************/
uint8_t I2C_WriteRegister_SW(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t RegisterData){
//	uint8_t i;
	I2C_GenerateSTART_SW();
	I2C_WriteByte_SW(DeviceAddress);
	if(I2C_GetACK_SW())
		return 1;//no ACKNOWLEDGE
	I2C_WriteByte_SW(RegisterAddress);
	if(I2C_GetACK_SW())
		return 1;//no ACKNOWLEDGE
	I2C_WriteByte_SW(RegisterData);
	if(I2C_GetACK_SW())
        return 1;//no ACKNOWLEDGE
	I2C_GenerateSTOP_SW();
	return 0;
}

/*******************************************************************************
 *******************************************************************************/
uint8_t I2C_ReadRegister_SW(uint8_t RegisterAddress){
    uint8_t TemporaryData;
    I2C_GenerateSTART_SW();
    I2C_WriteByte_SW(MMA8452_WADDR);
	if(I2C_GetACK_SW())
		return 1;//no ACKNOWLEDGE
    I2C_WriteByte_SW(RegisterAddress);
   	if(I2C_GetACK_SW())
		return 1;//no ACKNOWLEDGE
    I2C_GenerateSTART_SW();
    I2C_WriteByte_SW(MMA8452_RADDR);
	if(I2C_GetACK_SW())
		return 1;//no ACKNOWLEDGE
    TemporaryData = I2C_ReadByte_SW();
    I2C_GenerateACK_SW(NOACK);
    I2C_GenerateSTOP_SW();
return TemporaryData;
}
