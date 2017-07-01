#include "loops.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "wirelesstmp.h"
//#include "i2c_SW.h"
//
//#include "core_cmFunc.h"
//#include "core_cmInstr.h"
//#include "core_cm3.h"
//
#include "sht1x.h"
//
//*******************************************************************************
// *******************************************************************************/
void SHT_InitInterface(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(SHT_RCC_GPIO, ENABLE);/* Enable GPIO clock */
// NRF SDA pin
    GPIO_InitStructure.GPIO_Pin = SHT_SDA;// | LCD_MCO;/* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = SHT_GPIOSpeed;//GPIO_Speed_50MHz;
    GPIO_Init(SHT_GPIO, &GPIO_InitStructure);
// NRF SCK pin
    GPIO_InitStructure.GPIO_Pin = SHT_SCK;// | LCD_MCO;/* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = SHT_GPIOSpeed;//GPIO_Speed_50MHz;
    GPIO_Init(SHT_GPIO, &GPIO_InitStructure);
}

//*******************************************************************************
// *******************************************************************************/
void SHT_GenerateSTART(void){
	SHT_SCK_SDA_HI;//both to Hi
	Loop(1);
	SHT_SDA_LOW;//;
	Loop(0);
	SHT_SCK_LOW;//;
	Loop(1);
	SHT_SCK_HI;//;
	Loop(1);
	SHT_SDA_HI;
	SHT_SCK_LOW;
}

//*******************************************************************************
// *******************************************************************************/
void SHT_ResetConnection(void){
    uint8_t i;
    SHT_SCK_LOW;
    SHT_SDA_HI;
    for(i = 0; i < 9; i++){
        Loop(1);
        SHT_SCK_HI;
        Loop(1);
        SHT_SCK_LOW;
    }
    SHT_GenerateSTART();
}
//*******************************************************************************
//*******************************************************************************/

//
//
//void SHT_WriteStatus(uint8_t Status);
//uint8_t SHT_ReadStatus(void);
//
//
//void SHT_MeasureTMP(void){
//}
//
//
//void SHT_MeasureHUM(void){
//}
//
uint16_t SHT_ReadTemperature(void){
    uint8_t Temp;
    SHT_GenerateSTART();
    SHT_WriteByte(TEMP_MEAS);
    if(SHT_GetACK())
        return 1;  //no ACK
    while((((SHT_GPIO->IDR) & SHT_SDA) == SHT_SDA))//wait for conversion complete
        ;
//    Temp =
    return Temp;
}
//
//uint16_t SHT_ReadTHumidity(void){
//    uint16_t Hum;
//    SHT_GenerateSTART();
//    SHT_MeasureHUM();
//
//    return Hum;
//}
//
//
///*******************************************************************************
// *******************************************************************************/
//void SHT_GenerateACK(Acknowledge Ack){
////	uint8_t State;
//	SHT_SCK_LOW;
//	SHTTickCounter = 0;
//	while(SHTTickCounter < 1) //wait half of period
//		;
//	if(Ack)
//		SHT_SDA_HI;//SHT_PORT->BSRR |= SHT_SDA_PIN; //generate NO ACK
//	else
//		SHT_SDA_LOW;//SHT_PORT->BRR |= SHT_SDA_PIN; // generate ACK
//
//	SHTTickCounter = 0;
//	while(SHTTickCounter < 1) //wait half of period
//		;
//	SHT_SCK_HI;//SHT_PORT->BSRR |= SHT_SCK_PIN;	//set clock to HIGH
//
//	SHTTickCounter = 0;
//	while(SHTTickCounter < CLK_LENGTH) //wait half of period
//		;
//	SHT_SCK_LOW;
//	SHT_SDA_HI;
//}
//
///*******************************************************************************
// *******************************************************************************/
Acknowledge SHT_GetACK(void){
	uint8_t State;
	SHT_SDA_HI; //no effect of CPU to ACK
//	SHTTickCounter = 0;
//	while(SHTTickCounter < CLK_LENGTH) //wait half of period
//		;
Loop(1);
	SHT_SCK_HI;//SHT_PORT->BSRR |= SHT_SCK_PIN;	//set clock to HIGH

//	SHTTickCounter = 0;
//	while(SHTTickCounter < CLK_LENGTH) //wait half of period
//		;
	if((((SHT_GPIO->IDR) & SHT_SDA) == SHT_SDA)) //check ACK
		State = NOACK; //no ACK
	else
		State = ACK;	// ACK was detected
//	SHTTickCounter = 0;
//	while(SHTTickCounter < CLK_LENGTH)// wait half of period
//		;
Loop(1);
	SHT_SCK_LOW;
	return State;
}


//
//*******************************************************************************
//*******************************************************************************/
uint8_t SHT_ReadByte(){
	uint8_t i, Data;
	SHT_SCK_LOW;
	for(i = 0; i < 8; i++){
		Loop(5);
		SHT_SCK_HI;//SHT_PORT->BSRR |= SHT_SCK_PIN;	//set clock to HIGH
		if(1)
			Data |= (0x01 << i);
		else
			Data &= (~0x01 << i);
		Loop(5);//SHTTickCounter = 0;
		SHT_SCK_LOW;//SHT_PORT->BRR |= SHT_SCK_PIN; //SCK to LOW
	}
	return Data;
}

//*******************************************************************************
//*******************************************************************************/
void SHT_WriteByte(uint8_t Byte){
	uint8_t i, DataRegister = Byte;
	SHT_SCK_LOW;
	for(i = 0; i < 8; i++){
		if(((DataRegister << i) & 0x80) == 0x80)
			SHT_SDA_HI;//SHT_PORT->BSRR |= SHT_SDA_PIN;
		else
			SHT_SDA_LOW;//SHT_PORT->BRR |= SHT_SDA_PIN; // set data bit
		Loop(5);//SHTTickCounter = 0;
		//while(SHTTickCounter < CLK_LENGTH) //wait half of period
		//	;
		SHT_SCK_HI;//SHT_PORT->BSRR |= SHT_SCK_PIN;	//set clock to HIGH
		Loop(5);//SHTTickCounter = 0;
		//while(SHTTickCounter < CLK_LENGTH)// wait half of period
		//	;
		SHT_SCK_LOW;//SHT_PORT->BRR |= SHT_SCK_PIN; //SCK to LOW
	}
}
//
///*******************************************************************************
// *******************************************************************************/
//uint8_t SHT_WriteSequnce_SW(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t TableIndex, uint8_t *Data, uint8_t DataNumber){
//	uint8_t RetVal = 0;
//	SHT_GenerateSTART_SW();//start communication
//
//	SHT_WriteByte_SW(DeviceAddress); //send device address
//	if(SHT_GetACK_SW() == NOACK)	// check acknowledge
//		return 1;	//no acknowledge
//
//	SHT_WriteByte_SW(RegisterAddress); //send device address
//	if(SHT_GetACK_SW() == NOACK)	// check acknowledge
//		return 1;	//no acknowledge
//
//	while(DataNumber--){
//		SHT_WriteByte_SW((Data[TableIndex++]));
//		if(SHT_GetACK_SW() == NOACK)	{// check acknowledge
//			RetVal = 1;//return 1;	//no acknowledge
//			break;	//break loop
//		}
//	}
//
//	SHT_GenerateSTOP_SW();// stop communication
//	return RetVal;
//}
//
///*******************************************************************************
// *******************************************************************************/
//uint8_t SHT_ReadByte_SW(void){
//	uint8_t i, Data;
//	for(i = 0; i < 8; i++){
//		SHTTickCounter = 0;
//		while(SHTTickCounter < CLK_LENGTH)// wait half of period
//			;
//		SHTTickCounter = 0;
//		SHT_SCK_HI;// SHT_PORT->BSRR |= SHT_SCK_PIN;	//set clock to HIGH
//		while(SHTTickCounter < CLK_LENGTH) //wait half of period
//			;
//
//		if((((SHT_PORT->IDR) & SHT_SDA_PIN) == SHT_SDA_PIN)) //check ACK
//			Data |= (0x80 >> i); //no ACK
//		else
//			Data &= ~(0x80 >> i);	// ACK was detected
//
//		SHTTickCounter = 0;
//		while(SHTTickCounter < CLK_LENGTH)// wait half of period
//			;
//		SHT_SCK_LOW;
//	}
//	return Data;
//}
//
///*******************************************************************************
// *******************************************************************************/
//uint8_t SHT_ReadSequence_SW(uint8_t Address, uint8_t RegisterAddress, uint8_t *DataBuff, uint8_t DataNumber){
//	uint8_t Data;
//	SHT_GenerateSTART_SW();//start communication
//	SHT_WriteByte_SW(MMA8452_WADDR); //send device address
//	if(SHT_GetACK_SW() == NOACK)	// check acknowledge
//		return 1;	//no acknowledge
//
//	SHT_WriteByte_SW(RegisterAddress); //send device address
//	if(SHT_GetACK_SW() == NOACK)	// check acknowledge
//		return 1;	//no acknowledge
//
//	SHT_GenerateSTART_SW();// repeat start condition
//	SHT_WriteByte_SW(MMA8452_RADDR); //send device address for data reading
//	if(SHT_GetACK_SW() == NOACK)	// check acknowledge
//		return 1;	//no acknowledge
//
///*******************************************************************************
// *******************************************************************************/
//	while(DataNumber--){
//		*(DataBuff++) = SHT_ReadByte_SW();
//		if(DataNumber != 0)
//			SHT_GenerateACK_SW(ACK);
//	}
//	SHT_GenerateACK_SW(NOACK);
//
//	SHT_GenerateSTOP_SW();
//	return Data;
//}
//
///*******************************************************************************
// *******************************************************************************/
//uint8_t SHT_WriteRegister_SW(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t RegisterData){
////	uint8_t i;
//	SHT_GenerateSTART_SW();
//	SHT_WriteByte_SW(DeviceAddress);
//	if(SHT_GetACK_SW())
//		return 1;//no ACKNOWLEDGE
//	SHT_WriteByte_SW(RegisterAddress);
//	if(SHT_GetACK_SW())
//		return 1;//no ACKNOWLEDGE
//	SHT_WriteByte_SW(RegisterData);
//	if(SHT_GetACK_SW())
//        return 1;//no ACKNOWLEDGE
//	SHT_GenerateSTOP_SW();
//	return 0;
//}
//
///*******************************************************************************
// *******************************************************************************/
//uint8_t SHT_ReadRegister_SW(uint8_t RegisterAddress){
//    uint8_t TemporaryData;
//    SHT_GenerateSTART_SW();
//    SHT_WriteByte_SW(MMA8452_WADDR);
//	if(SHT_GetACK_SW())
//		return 1;//no ACKNOWLEDGE
//    SHT_WriteByte_SW(RegisterAddress);
//   	if(SHT_GetACK_SW())
//		return 1;//no ACKNOWLEDGE
//    SHT_GenerateSTART_SW();
//    SHT_WriteByte_SW(MMA8452_RADDR);
//	if(SHT_GetACK_SW())
//		return 1;//no ACKNOWLEDGE
//    TemporaryData = SHT_ReadByte_SW();
//    SHT_GenerateACK_SW(NOACK);
//    SHT_GenerateSTOP_SW();
//return TemporaryData;
//}
