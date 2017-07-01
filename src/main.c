/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "math.h"
//#include "core_cmFunc.h"
//#include "core_cmInstr.h"
//#include "core_cm3.h"
#include "wirelesstmp.h"
//#include "i2c_SW.h"
#include "sht1x.h"
#include "loops.h"
#include "rfm75.h"
uint32_t SystemTimer;
uint16_t Temp;
uint8_t LED1_Flag = 0;
uint8_t LED2_Flag = 0;
uint8_t DataSend_Flag = 0;
volatile uint16_t DelayCnt, LED_Cnt, DataSend_Cnt;
uint8_t TmpArray[20];


/************************************************************************************************************************
 ************************************************************************************************************************/
void SetPinValue(GPIO_TypeDef *Portx, uint16_t Pin, State LowHigh){
    if(LowHigh == HI)
        Portx->BSRR = Pin;
    else
        Portx->BRR = Pin;
}

/************************************************************************************************************************
 ************************************************************************************************************************/
void LightOnLED(GPIO_TypeDef *Portx, uint16_t Pin, State LowHigh){
    if(LowHigh == HI)
        Portx->BSRR = Pin;
    else
        Portx->BRR = Pin;
}

/************************************************************************************************************************
 ************************************************************************************************************************/
void Delay_ms(uint16_t Time){
    DelayCnt = Time;
    while(DelayCnt)
        ;
}

/************************************************************************************************************************
 ************************************************************************************************************************/
void SysTick_Handler(void){
    DelayCnt -= 1;
    LED_Cnt -= 1;
    DataSend_Cnt -= 1;

    if(LED1_Flag){
        LightOnLED(SYSLED1, LED_OFF);
        LED1_Flag = 0;
    }
    else{
        LightOnLED(SYSLED1, LED_ON);
        LED1_Flag = 1;
    }
}

/************************************************************************************************************************
 ************************************************************************************************************************/
void LED_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LEDPORT, ENABLE);/* Enable GPIO clock */
// NRF output control pins
    GPIO_InitStructure.GPIO_Pin = SYS_LED1 | SYS_LED2;/* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LEDPORT, &GPIO_InitStructure);
//pin state config
}

/************************************************************************************************************************
 ************************************************************************************************************************/
int main(void){
    uint8_t r;
    Flags.flags = 0;
//    uint32_t TestPipe;
//    uint16_t TestReg;
//    double sinex = 0.97;
    SystemInit();
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_6);
    RCC_PLLCmd(ENABLE);				// enable PLL. mul by 6
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);//100us
    SystemTimer = 0;
    Delay_ms(15); //wait 15 ms because of SHT11

    SHT_InitInterface();
    SHT_ResetConnection();

/**********************************************************/
/**********************************************************/
    RFM_InitInterface();
    LED_Init();
    Delay_ms(11); //Wait 11ms because of SHT11
    RFM_RegistersTMP.Status = RFM_ReadRegister(STATUS);
    RFM_InitCommunication();
    Delay_ms(11);
    RFM_ReadConfig();

/**********************************************************/
/**********************************************************/
#ifdef TXMODE
    RFM_SetStandby_I();
    Delay_ms(1);
RFM_Registers.Config = RFM_ReadRegister(CONFIG);///TODO
    RFM_SetModeTX();// switch device to transmit mode
RFM_Registers.Config = RFM_ReadRegister(CONFIG);///TODO
    for(r = 0; r < 5; r++){
        Delay_ms(500);
        LightOnLED(SYSLED2,LED_ON);//while(1);
        Delay_ms(500);
        LightOnLED(SYSLED2,LED_OFF);
    }

    while(1){
        if((!LED_Cnt) || (LED_Cnt > 1000)){
            if(LED2_Flag){
                LED_Cnt = 1000;
                LightOnLED(SYSLED2, LED_OFF);
                LED2_Flag = 0;
            }
            else{
                LED_Cnt = 60;
                LightOnLED(SYSLED2, LED_ON);
                LED2_Flag = 1;
            }
        }

        if((!DataSend_Cnt) || (DataSend_Cnt > 5000)){
            DataSend_Cnt = 200;

     //       RFM_PayLoad_TX(4); //send data
           // RFM_SendPacket(&DataPacket);
           RFM_Send_NACK_Packet();
        }
        RFM_Registers.Config = RFM_ReadRegister(CONFIG);
    }
#endif
#ifdef RXMODE
    RFM_SetStandby_I();
    Delay_ms(1);
    RFM_SetModeRX();// switch device to transmit mode
    Delay_ms(2);

    for(r = 0; r < 3; r++){
        Delay_ms(500);
        LightOnLED(SYSLED2,LED_ON);//while(1);
        Delay_ms(500);
        LightOnLED(SYSLED2,LED_OFF);
    }

    while(1){
        ReceivePacket();

        if((!LED_Cnt) || (LED_Cnt > 2000)){
            if(LED2_Flag){
                LED_Cnt = 2000;
                LightOnLED(SYSLED2, LED_OFF);
                LED2_Flag = 0;
            }
            else{
                LED_Cnt = 120;
                LightOnLED(SYSLED2, LED_ON);
                LED2_Flag = 1;
            }
        }
        if(RFM_CarrierDetect())
            LightOnLED(SYSLED1,LED_ON);//while(1);
        else
            LightOnLED(SYSLED1,LED_OFF);

        RFM_Registers.Config = RFM_ReadRegister(CONFIG);
    }
#endif

//    while(1){
//       SHT_GenerateSTART();
//       SHT_ReadTemperature();
//    }
}
