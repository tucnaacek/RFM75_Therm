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
#include "rfm75.h"
extern uint8_t TmpArray[20];

PacketStruct DataPacket = {
    0,//uint8_t Preamble;
    {0xaa, 0xaa, 0xaa, 0xaa, 0xaa},//{0xaa, 0xbb, 0xcc, 0xdd, 0xee},//uint8_t Address[5];
    {0xaa, 0xaa, 0xaa, 0xaa, 0xaa},//{0xaa, 0xbb, 0xcc, 0xdd, 0xee},//uint8_t Address[5];
    0,//uint16_t PacketControl;
    {ZERO32},//uint8_t RX_PayLoad[32];
    {DATA32},//uint8_t TX_PayLoad[32];
    0//uint16_t Crc;
};

RFM75_MapRegisters RFM_Registers = {
    30,//uint8_t RegNumber;
    0x3e,//  config
    0x3f,//  En_aa;//01
    0x3f,//  En_RXAddr;//02//enable all pipes
    0x01,//  Setup_AW;//03
    (RETR_TIME_1000us | RETR_OFF),//  Setup_Retr;//04 Automatic retransmisson
    0x02,//  RF_Ch;//05 RF Channel
    0x2f,//  RF_Setup;//2Mbps 06 RF Setup Register
    0x70,//  Status;// 07 write 1 clear written bit
    0x00,//  Observe_TX;//08 ReadOnly
    0x00,//  Cd;//09 carrier detect RadOnly
    0x11,//  RX_Addr_P0;//0a
    0xa1,//  RX_Addr_P1;//0b carrier detect
    0xa2,//  RX_Addr_P2;//0c carrier detect
    0xa3,//  RX_Addr_P3;//0d carrier detect
    0xa4,//  RX_Addr_P4;//0e carrier detect
    0xa5,//  RX_Addr_P5;//0f carrier detect
    0xaa,//  TX_Addr;//10
    0x04,//  RX_Pw_P0;//11 four bytes length
    0x04,//  RX_Pw_P1;//12
    0x04,//  RX_Pw_P2;//13
    0x04,//  RX_Pw_P3;//14
    0x04,//  RX_Pw_P4;//15
    0x04,//  RX_Pw_P5;//16
    0x00,//  Fifo_Status;//17 Reuse is set by SPI command
    0x00,//  Ack_Pld;// Not writeable by special command
    0x00,//  TX_Pld;//Not writeable by special command
    0x00,//  RX_Pld;//Not writeable by special command
    0x00,//  DynPd;//0x1c no dynamic payload
    0x00 // Feature;//0x1d no dynamic payload
};

RFM75_MapRegisters* p_RFM75_MapRegisters = &RFM_Registers;

RFM75_MapRegisters RFM_RegistersTMP = {
    0,//uint8_t RegNumber;
    0,//  config
    0,//  En_aa;//01
    0,//  En_RXAddr;//02
    0,//  Setup_AW;//03
    0,//  Setup_Retr;//04 Automatic retransmisson
    0,//  RF_Ch;//05 RF Channel
    0,//  RF_Setup;//06 RF Setup Register
    0,//  Status;// 07 write 1 clear written bit
    0,//  Observe_TX;//08 ReadOnly
    0,//  Cd;//09 carrier detect RadOnly
    0,//  RX_Addr_P0;//0a
    0,//  RX_Addr_P1;//0b carrier detect
    0,//  RX_Addr_P2;//0c carrier detect
    0,//  RX_Addr_P3;//0d carrier detect
    0,//  RX_Addr_P4;//0e carrier detect
    0,//  RX_Addr_P5;//0f carrier detect
    0,//  TX_Addr;//10
    0,//  RX_Pw_P0;//11 four bytes length
    0,//  RX_Pw_P1;//12
    0,//  RX_Pw_P2;//13
    0,//  RX_Pw_P3;//14
    0,//  RX_Pw_P4;//15
    0,//  RX_Pw_P5;//16
    0,//  Fifo_Status;//17 Reuse is set by SPI command
    0,//  Ack_Pld;// Not writeable by special command
    0,//  TX_Pld;//Not writeable by special command
    0,//  RX_Pld;//Not writeable by special command
    0,//  DynPd;//0x1c no dynamic payload
    0 // Feature;//0x1d no dynamic payload
};

RFM75_MapRegisters* p_RFM_RegistersTMP = &RFM_RegistersTMP;


//Bank1 register initialization values

const uint8_t Bank1_R0[] = {0x40, 0x4b, 0x01, 0xe2};/*0xe2, 0x01, 0x4b, 0x40*/
const uint8_t Bank1_R1[] = {0xc0, 0x4b, 0x00, 0x00};
const uint8_t Bank1_R2[] = {0xd0, 0xfc, 0x8c, 0x02};
const uint8_t Bank1_R3[] = {0x99, 0x00, 0x39, 0x21};

const uint8_t Bank1_R4_1Msps[] = {0xf9, 0x96, 0x82, 0x1b};///1Msps
const uint8_t Bank1_R4_2Msps[] = {0xf9, 0x96, 0x82, 0xdb};///2Msps
const uint8_t Bank1_R4_250ksps[] = {0xf9, 0x96, 0x8a, 0xdb};///250ksps

const uint8_t Bank1_R5_1Msps[] = {0x24, 0x06, 0x0f, 0xa6};///1Msps
const uint8_t Bank1_R5_2Msps[] = {0x24, 0x06, 0x0f, 0xb6};///2Msps
const uint8_t Bank1_R5_250ksps[] = {0x24, 0x06, 0x0f, 0xb6};///250ksps


const uint8_t Bank1_R12[] = {0x00, 0x12, 0x73, 0x05};//{0x05, 0x73, 0x12, 0x00};//

const uint8_t Bank1_R12_120us[] = {0x00, 0x12, 0x73, 0x00};//{0x00, 0x73, 0x12, 0x00}; //
//is inverted
const uint8_t Bank1_R13[] = {0x36, 0xb4, 0x80, 0x00};//{0x00, 0x80, 0xb4, 0x36};//
//is inverted
const uint8_t Bank1_R14[] = {0x41, 0x20, 0x08, 0x04, 0x81, 0x20, 0xCF, 0xF7, 0xFE, 0xFF, 0xFF};//{0xff, 0xff, 0xfe, 0xf7, 0xcf, 0x20, 0x81, 0x04, 0x08, 0x20, 0x41};




//*******************************************************************************
// *******************************************************************************/
uint8_t RFM_ReadRegister(uint8_t Register){
    uint8_t Reg = (Register & 0x1f);//mask register address
    uint16_t Tmp;
    RFM_CSN_LOW;
    RFM_WriteByte(R_REGISTER | Reg); //connect command character and register addres
    Reg = RFM_ReadByte();

    RFM_CSN_HI;
    return Reg;
}

//*******************************************************************************
// *******************************************************************************/
void RFM_WriteRegister(uint8_t Register, uint8_t Value){
    RFM_CSN_LOW;//mrf active

    RFM_WriteByte(W_REGISTER | Register);
    RFM_WriteByte(Value);

    RFM_CSN_HI;
}

//*******************************************************************************
// *******************************************************************************/
void RFM_Activate(uint8_t Value){
    RFM_CSN_LOW;//mrf active

    RFM_WriteByte(ACTIVATE);
    RFM_WriteByte(Value);

    RFM_CSN_HI;
}

//*******************************************************************************
// *******************************************************************************/
void RFM_WriteRegisterBuf(uint8_t Register, uint8_t* Buffer, uint8_t NmbBytes){
    uint8_t i;
    RFM_CSN_LOW;//mrf active

    RFM_WriteByte(W_REGISTER | Register);
    for (i = 0; i<NmbBytes; i++){
        RFM_WriteByte(*(Buffer+i));//RFM_WriteByte(Buffer[i]);
    }

    RFM_CSN_HI;
}

//*******************************************************************************
// *******************************************************************************/
void RFM_WriteRegister_Bnk1(uint8_t Register, uint8_t** Buffer, uint8_t NmbBytes){
    uint8_t i;
    RFM_CSN_LOW;//mrf active

    RFM_WriteByte(W_REGISTER | Register);
    for (i = 0; i<NmbBytes; i++){
        RFM_WriteByte(*(Buffer+i));//RFM_WriteByte(Buffer[i]);
    }

    RFM_CSN_HI;
}

//*******************************************************************************
// *******************************************************************************/
void RFM_ReadRegisterBuf(uint8_t Register, uint8_t* Buffer, uint8_t NmbBytes){
    uint8_t i;
    RFM_CSN_LOW;//mrf active

    RFM_WriteByte(R_REGISTER | Register);
    for (i = 0; i<NmbBytes; i++){
        *(Buffer+i) = RFM_ReadByte();//Buffer[i] = RFM_ReadByte();
    }

    RFM_CSN_HI;
}

//*******************************************************************************
// *******************************************************************************/
uint8_t RFM_CountLostPackets(void){ //return number of lost packets
    uint8_t Tmp;
///    Tmp = RFM_ReadRegister(OBSERVE_TX);
return (Tmp & 0xf0);
}

//*******************************************************************************
// *******************************************************************************/
uint8_t RFM_CountRetransPackets(void){
    uint8_t Tmp;
///    Tmp = RFM_ReadRegister(OBSERVE_TX);//read observe register
return (Tmp & 0x0f);
}

//*******************************************************************************
// *******************************************************************************/
void RFM_TX_ReuseSet(){

}
//*******************************************************************************
// *******************************************************************************/

void RFM_TX_ReuseReset(){

}


/************************************************************************************************************************
 ************************************************************************************************************************/
void RFM_InitInterface(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RFM_RCC_SPI_GPIO | RCC_APB2Periph_AFIO, ENABLE);/* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RFM_RCC_SPI, ENABLE);/* Enable SPI clock */

// NRF SPI pins
    GPIO_InitStructure.GPIO_Pin = RFM_SCK | RFM_MOSI;// | RFM_MCO;/* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = RFM_SPI_GPIOOType;//switch to alternate function
    GPIO_InitStructure.GPIO_Speed = RFM_SPI_GPIOSpeed;//GPIO_Speed_50MHz;
    GPIO_Init(RFM_SPI_GPIO, &GPIO_InitStructure);
// NRF SPI pins
    GPIO_InitStructure.GPIO_Pin = RFM_MISO;// | RFM_MCO;/* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = RFM_SPI_GPIOOType;//GPIO_Mode_IN_FLOATING;//switch to alternate function
    GPIO_InitStructure.GPIO_Speed = RFM_SPI_GPIOSpeed;//GPIO_Speed_50MHz;
    GPIO_Init(RFM_SPI_GPIO, &GPIO_InitStructure);
// NRF output control pins
    GPIO_InitStructure.GPIO_Pin = RFM_CE | RFM_CSN;/* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RFM_CTRL_GPIO, &GPIO_InitStructure);
// NRF SPI configuration
    SPI_InitStructure.SPI_Direction = RFM_SPI_Direction;
	SPI_InitStructure.SPI_Mode = RFM_SPI_Mode;
	SPI_InitStructure.SPI_DataSize = RFM_SPI_DataSize;
	SPI_InitStructure.SPI_CPOL = RFM_SPI_Cpol;
	SPI_InitStructure.SPI_CPHA = RFM_SPI_Cpha;
	SPI_InitStructure.SPI_NSS = RFM_SPI_Nss;
	SPI_InitStructure.SPI_BaudRatePrescaler = RFM_SPI_BaudRate;//SPI_BaudRatePrescaler_256; // 72000kHz/256=281kHz < 400kHz
	SPI_InitStructure.SPI_FirstBit = RFM_SPI_FirstBit;
    SPI_Init(RFM_SPI, &SPI_InitStructure);/* USART configuration */
    SPI_Cmd(RFM_SPI, ENABLE);
//pin state config
    RFM_CE_LOW;
    RFM_CSN_HI;
}
//*******************************************************************************
// *******************************************************************************/
void RFM_InitCommunication(void){
    //RegNumber;
    uint8_t i;
    Delay_ms(100);
    RFM_SetRegBank(0);//switch to bank0 registers

    RFM_WriteRegister(CONFIG, RFM_Registers.Config);//Config;//00
    RFM_WriteRegister(EN_AA, RFM_Registers.En_aa);//En_aa;//01
    RFM_WriteRegister(EN_RXADDR, RFM_Registers.En_RXAddr);//En_RXAddr;//02
    RFM_WriteRegister(SETUP_AW, RFM_Registers.Setup_AW);//Setup_AW;//03
    RFM_WriteRegister(SETUP_RETR, RFM_Registers.Setup_Retr);//Setup_Retr;//04 Automatic retransmisson
    RFM_WriteRegister(RF_CH, RFM_Registers.RF_Ch);//RF_Ch;//05 RF Channel
    RFM_WriteRegister(RF_SETUP, RFM_Registers.RF_Setup);//RF_Setup;//06 RF Setup Register
    RFM_WriteRegister(STATUS, RFM_Registers.Status);//Status;//07
    RFM_WriteRegister(OBSERVE_TX, RFM_Registers.Observe_TX);//Observe_TX;//08
    RFM_WriteRegister(CD, RFM_Registers.Cd);//Cd;//09 carrier detect
    RFM_WriteRegister(RX_ADDR_P0, RFM_Registers.RX_Addr_P0);//RX_Addr_P0;//08
    RFM_WriteRegister(RX_ADDR_P1, RFM_Registers.RX_Addr_P1);//RX_Addr_P1;//09 carrier detect
    RFM_WriteRegister(RX_ADDR_P2, RFM_Registers.RX_Addr_P2);//RX_Addr_P2;//09 carrier detect
    RFM_WriteRegister(RX_ADDR_P3, RFM_Registers.RX_Addr_P3);//RX_Addr_P3;//09 carrier detect
    RFM_WriteRegister(RX_ADDR_P4, RFM_Registers.RX_Addr_P4);//RX_Addr_P4;//09 carrier detect
    RFM_WriteRegister(RX_ADDR_P5, RFM_Registers.RX_Addr_P5);//RX_Addr_P5;//09 carrier detect
    RFM_WriteRegister(TX_ADDR, RFM_Registers.TX_Addr);//TX_Addr;///10
    RFM_WriteRegister(RX_PW_P0, RFM_Registers.RX_Pw_P0);//RX_Pw_P0;//11
    RFM_WriteRegister(RX_PW_P1, RFM_Registers.RX_Pw_P1);//RX_Pw_P1;//12
    RFM_WriteRegister(RX_PW_P2, RFM_Registers.RX_Pw_P2);//RX_Pw_P2;//13
    RFM_WriteRegister(RX_PW_P3, RFM_Registers.RX_Pw_P3);//RX_Pw_P3;//14
    RFM_WriteRegister(RX_PW_P4, RFM_Registers.RX_Pw_P4);//RX_Pw_P4;//15
    RFM_WriteRegister(RX_PW_P5, RFM_Registers.RX_Pw_P5);//RX_Pw_P5;//16
    RFM_WriteRegister(FIFO_STATUS, RFM_Registers.Fifo_Status);//Fifo_Status;//17
//    RFM_WriteRegister(ACK_PLD, RFM_Registers.Ack_Pld);//Ack_Pld;// Not writeable by special command


    RFM_WriteRegister(DYNPD, RFM_Registers.DynPd);//DynPd;//0x1c
    RFM_WriteRegister(FEATURE, RFM_Registers.Feature);//Feature;//0x1d
    RFM_SetChannel(CHANNEL0);
    RFM_SetRFGain(RF_GAIN_HI);
    RFM_SetAddressWidth(WADDR5);


    RFM_SetChannel(0);
    RFM_SetTXPower(RF_PWR3);
    RFM_SetRXAddress(DataPacket.RX_Address, 5, 0);//(uint8_t* Address, uint8_t AddrWidth, uint8_t PipeNumber);
    RFM_SetTXAddress(DataPacket.TX_Address, 5, 0);//(uint8_t* Address, uint8_t AddrWidth, uint8_t PipeNumber);
///    RFM_ReadRegisterBuf(TX_ADDR, TmpArray, 5);///TODO
///    RFM_ReadRegisterBuf(RX_ADDR_P0, TmpArray, 5);///TODO
///    RFM_ReadRegisterBuf(RX_ADDR_P1, TmpArray, 5);///TODO

    RFM_Registers.Feature = RFM_ReadRegister(FEATURE);
    if(RFM_Registers.Feature == 0)
        RFM_WriteRegister(ACTIVATE, 0x73);/// if chip is not activated, activate it

    RFM_SetRegBank(1); //switch to bank 1

    RFM_WriteRegisterBuf(0, Bank1_R0, 4);/// this register has to be written in reverse mode
    RFM_WriteRegisterBuf(1, Bank1_R1, 4);/// this register has to be written in reverse mode
    RFM_WriteRegisterBuf(2, Bank1_R2, 4);/// this register has to be written in reverse mode
    RFM_WriteRegisterBuf(3, Bank1_R3, 4);/// this register has to be written in reverse mode
    RFM_WriteRegisterBuf(0x04, Bank1_R4_2Msps, 4);/// this register has to be written in reverse mode
    RFM_WriteRegisterBuf(0x05, Bank1_R5_2Msps, 4);/// this register has to be written in reverse mode
    RFM_WriteRegisterBuf(0x0c, Bank1_R12, 4);
    RFM_WriteRegisterBuf(0x0d, Bank1_R13, 4);
    RFM_WriteRegisterBuf(0x0e, Bank1_R14, 11);
//check written data
    RFM_Registers.Status = RFM_ReadRegister(STATUS);
    RFM_ReadRegisterBuf(BNK1_08, TmpArray, 4);///TODO, read chip ID

    Delay_ms(10);
    RFM_SetRegBank(0);
}

//*******************************************************************************
//*******************************************************************************
//*******************************************************************************
// *******************************************************************************/
void RFM_ReadConfig(void){
    //RegNumber;
    RFM_RegistersTMP.Config = RFM_ReadRegister(CONFIG);//Config;//00
    RFM_RegistersTMP.En_aa = RFM_ReadRegister(EN_AA);//En_aa;//01
    RFM_RegistersTMP.En_RXAddr = RFM_ReadRegister(EN_RXADDR);//En_RXAddr;//02
    p_RFM_RegistersTMP->Setup_AW = RFM_ReadRegister(SETUP_AW);//Setup_AW;//03
    p_RFM_RegistersTMP->Setup_Retr = RFM_ReadRegister(SETUP_RETR);//Setup_Retr;//04 Automatic retransmisson
    p_RFM_RegistersTMP->RF_Ch = RFM_ReadRegister(RF_CH);//RF_Ch;//05 RF Channel
    p_RFM_RegistersTMP->RF_Setup = RFM_ReadRegister(RF_SETUP);//RF_Setup;//06 RF Setup Register
    p_RFM_RegistersTMP->Status = RFM_ReadRegister(STATUS);//Status;//07
    RFM_RegistersTMP.Observe_TX = RFM_ReadRegister(OBSERVE_TX);//Observe_TX;//08
    RFM_RegistersTMP.Cd = RFM_ReadRegister(CD);//Cd;//09 carrier detect
    RFM_RegistersTMP.RX_Addr_P0 = RFM_ReadRegister(RX_ADDR_P0);//RX_Addr_P0;//08
    RFM_RegistersTMP.RX_Addr_P1 = RFM_ReadRegister(RX_ADDR_P1);//RX_Addr_P1;//09 carrier detect
    RFM_RegistersTMP.RX_Addr_P2 = RFM_ReadRegister(RX_ADDR_P2);//RX_Addr_P2;//09 carrier detect
    RFM_RegistersTMP.RX_Addr_P3 = RFM_ReadRegister(RX_ADDR_P3);//RX_Addr_P3;//09 carrier detect
    RFM_RegistersTMP.RX_Addr_P4 = RFM_ReadRegister(RX_ADDR_P4);//RX_Addr_P4;//09 carrier detect
    RFM_RegistersTMP.RX_Addr_P5 = RFM_ReadRegister(RX_ADDR_P5);//RX_Addr_P5;//09 carrier detect
    RFM_RegistersTMP.TX_Addr = RFM_ReadRegister(TX_ADDR);//TX_Addr;///10
    RFM_RegistersTMP.RX_Pw_P0 = RFM_ReadRegister(RX_PW_P0);//RX_Pw_P0;//11
    RFM_RegistersTMP.RX_Pw_P1 = RFM_ReadRegister(RX_PW_P1);//RX_Pw_P1;//12
    RFM_RegistersTMP.RX_Pw_P2 = RFM_ReadRegister(RX_PW_P2);//RX_Pw_P2;//13
    RFM_RegistersTMP.RX_Pw_P3 = RFM_ReadRegister(RX_PW_P3);//RX_Pw_P3;//14
    RFM_RegistersTMP.RX_Pw_P4 = RFM_ReadRegister(RX_PW_P4);//RX_Pw_P4;//15
    RFM_RegistersTMP.RX_Pw_P5 = RFM_ReadRegister(RX_PW_P5);//RX_Pw_P5;//16
    RFM_RegistersTMP.Fifo_Status = RFM_ReadRegister(FIFO_STATUS);//Fifo_Status;//17
//    RFM_WriteRegister(ACK_PLD, RFM_Registers.Ack_Pld);//Ack_Pld;// Not writeable by special command
//    RFM_WriteRegister(TX_PLD, RFM_Registers.TX_Pld);//TX_Pld;//Not writeable by special command
//    RFM_WriteRegister(RX_PLD, RFM_Registers.RX_Pld);//RX_Pld;//Not writeable by special command
    RFM_Registers.DynPd = RFM_ReadRegister(DYNPD);//DynPd;//0x1c
    RFM_Registers.Feature = RFM_ReadRegister(FEATURE);//Feature;//0x1d
}

//*******************************************************************************
// *******************************************************************************/
void RFM_SetModeTX(void){
    uint8_t Tmp;

    RFM_WriteRegister(FLUSH_TX, 0);//flush Tx FIFO
///    Tmp = RFM_ReadRegister(STATUS);
///    RFM_WriteRegister(STATUS, Tmp);// Clear flags. The flags are cleared if 1 is written

    RFM_CE_LOW;
    Delay_ms(1);
    RFM_Registers.Config = RFM_ReadRegister(CONFIG);//Status;//07
    RFM_Registers.Config = (RFM_Registers.Config & (~PRIM_RX));//set tx mode  - clear PRIM_RX bit
    RFM_WriteRegister(CONFIG, RFM_Registers.Config);//write data back to RFM chip
    Tmp = RFM_ReadRegister(CONFIG);///TODO test register value
    RFM_CE_HI;
}
//*******************************************************************************
// *******************************************************************************/
void RFM_SetStandby_I(void){
    RFM_CE_LOW;//clear CE pin
    Delay_ms(1);
    RFM_Registers.Config = RFM_ReadRegister(CONFIG);//read backup
    RFM_Registers.Config = (RFM_Registers.Config & 0x7f) | PWR_UP;//set power_up on
    RFM_WriteRegister(CONFIG, RFM_Registers.Config);//write data back to RFM chip
}
//*******************************************************************************
// *******************************************************************************/
void RFM_SetModeRX(void){
    uint8_t Tmp;

    RFM_WriteRegister(FLUSH_RX, 0);//flush Rx FIFO
    Tmp = RFM_ReadRegister(STATUS);
    RFM_WriteRegister(STATUS, Tmp);// Clear flags. The flags are cleared if 1 is written

    RFM_CE_LOW;
    Delay_ms(1);
    RFM_Registers.Config = RFM_ReadRegister(CONFIG);//Status;//07
    RFM_Registers.Config = (RFM_Registers.Config | PRIM_RX);//set tx mode  - clear PRIM_RX bit
    RFM_WriteRegister(CONFIG, RFM_Registers.Config);//write data back to RFM chip
    RFM_CE_HI; //Enter to RX-mode
}
//*******************************************************************************
// *******************************************************************************/
void RFM_SetModeSleep(void){
    RFM_Registers.Config = RFM_ReadRegister(CONFIG);//Status;//07
}

//*******************************************************************************
// *******************************************************************************/
void RFM_SetChannel(uint8_t Channel){
    RFM_WriteRegister(RF_CH, Channel);//Status;//07
}

//*******************************************************************************
//*******************************************************************************/
uint8_t RFM_ReadPipeNumber(void){
    RFM_Registers.Status |= (RFM_ReadRegister(STATUS) & 0x0e);//mask other bits than pipe number
    return ((RFM_Registers.Status & 0x0e) >> 1);
return 0;
}

//*******************************************************************************
//*******************************************************************************/
uint8_t RFM_CarrierDetect(void){
    RFM_Registers.Cd = RFM_ReadRegister(CD);//CD);
    return (RFM_Registers.Cd & 0x01);
}

//*******************************************************************************
//*******************************************************************************/
uint8_t RFM_TX_Full(void){
    RFM_Registers.Status |= (RFM_ReadRegister(STATUS) & 0x01); //save state to registers map
    return ((RFM_Registers.Status) & 0x01);
return 0;
}

//*******************************************************************************
//*******************************************************************************/
uint8_t RFM_RX_Empty(void){
return 0;
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetTXPower(uint8_t Pwr){
   RFM_Registers.RF_Setup = RFM_ReadRegister(RF_SETUP);
   RFM_WriteRegister(RF_SETUP, ((RFM_Registers.RF_Setup & 0xe9) | Pwr)); //mask by 01111001
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetRFGain(uint8_t Gain){
    RFM_Registers.RF_Setup = RFM_ReadRegister(RF_SETUP) & 0xfd;//mask by 11111101
    RFM_WriteRegister(RF_SETUP, (RFM_Registers.RF_Setup | Gain)); //mask by 01111110
}

//*******************************************************************************
//*******************************************************************************/
void RFM_WriteByte(uint8_t Byte){
    while (SPI_I2S_GetFlagStatus(RFM_SPI, SPI_I2S_FLAG_TXE) == RESET)
         ;
    SPI_I2S_SendData(RFM_SPI, Byte);//SendByte(Cmd.CommandLength);
    while(SPI_I2S_GetFlagStatus(RFM_SPI, SPI_I2S_FLAG_BSY) == 1)
        ;//SPI_I2S_FLAG_BSY
}

//*******************************************************************************
//*******************************************************************************/
uint8_t RFM_ReadByte(void){
	uint16_t Data;
    Data = (SPI_I2S_ReceiveData(RFM_SPI));// setup register contain. Throw out
    //wait until transmit complete
    while (SPI_I2S_GetFlagStatus(RFM_SPI, SPI_I2S_FLAG_TXE) == RESET);
    //Send dummy byte 0x00
    SPI_I2S_SendData(RFM_SPI, 0);
    //wait until receive complete
    while (SPI_I2S_GetFlagStatus(RFM_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    //wait until SPI will be free
   // while(SPI_I2S_GetFlagStatus(RFM_SPI, SPI_I2S_FLAG_BSY) == 1);
    //Read new data from rx register
    Data = (SPI_I2S_ReceiveData(RFM_SPI));
    while(SPI_I2S_GetFlagStatus(RFM_SPI, SPI_I2S_FLAG_BSY) == 1);

return (uint8_t)Data;
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetTXAddress(volatile uint8_t Address[], uint8_t AddrWidth, uint8_t PipeNumber){
    RFM_WriteRegisterBuf(TX_ADDR, Address, AddrWidth);
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetAddressWidth(uint8_t Width){
    RFM_WriteRegister(SETUP_AW, Width);
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetRegBank(uint8_t Bank){
    RFM_Registers.Status = RFM_ReadRegister(STATUS); //save state to registers map

    if((((RFM_Registers.Status & RBANK) == RBANK) && (Bank == 0)) || (((RFM_Registers.Status & RBANK) == 0) && Bank))
        RFM_Activate(0x53);
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetRXAddress(volatile uint8_t Address[], uint8_t AddrWidth, uint8_t PipeNumber){
   if(PipeNumber > 5) //out of address range
       return;
   if(PipeNumber < 2){//pipes 0 or 1, write full address
        RFM_WriteRegisterBuf(RX_ADDR_P0 + PipeNumber, Address, AddrWidth);
   }
   else{
        RFM_WriteRegister((RX_ADDR_P0 + PipeNumber), DataPacket.TX_Address[0]); //LSB is shared byte
   }
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetRetransTime(uint8_t Time){
    RFM_Registers.Setup_Retr = RFM_ReadRegister(SETUP_RETR);//read actual value from setup register
    RFM_Registers.Setup_Retr = ((RFM_Registers.Setup_Retr & 0xf0) | (Time & 0xf0)); //write new value to setup register
    RFM_WriteRegister(SETUP_RETR, RFM_Registers.Setup_Retr);
}

//*******************************************************************************
//*******************************************************************************/
void RFM_SetRetransCount(uint8_t Count){
    RFM_Registers.Setup_Retr = RFM_ReadRegister(SETUP_RETR);//read actual value from setup register
    RFM_Registers.Setup_Retr = ((RFM_Registers.Setup_Retr & 0x0f) | (Count & 0x0f)); //write new value to setup register
    RFM_WriteRegister(SETUP_RETR, RFM_Registers.Setup_Retr);
}

//*******************************************************************************
//*******************************************************************************/
void RFM_PayLoad_TX(uint8_t DataLenght){
    uint8_t Tmp, i = 0;

    RFM_Registers.Fifo_Status = RFM_ReadRegister(FIFO_STATUS);
    if(!(RFM_Registers.Fifo_Status & TX_FULL)){ //not full send
        RFM_CSN_LOW;    //enable communication
        RFM_WriteByte(W_TX_PAYLOAD);//command payload

        for(i = 0; i < DataLenght; i++){
            RFM_WriteByte(DataPacket.TX_PayloadData[i]);
        }
    }
    RFM_CSN_HI;
//    RFM_CE_HI;
//    Delay_ms(1);//Loop(5);
//    RFM_CE_LOW;
}

//*******************************************************************************
//*******************************************************************************/
void RFM_PayLoad_RX(uint8_t DataLenght){
    uint8_t i = 0;
    RFM_CSN_LOW;    //enable communication
    RFM_WriteByte(R_RX_PAYLOAD);//command payload

    for(i = 0; i < DataLenght; i++){
        DataPacket.RX_PayloadData[i] = RFM_ReadByte();
    }

    RFM_CSN_HI;
    RFM_CE_HI;
    Delay_ms(1);//Loop(5);
    RFM_CE_LOW;
}

/*************************************************************************************
*************************************************************************************/

uint8_t RFM_ReadPayloadWidth(void){
    uint8_t Lenght;//mask register address

    RFM_CSN_LOW;
    RFM_WriteByte(R_RX_PL_WID); //connect command character and register addres
    Lenght = RFM_ReadByte();
    RFM_CSN_HI;

    return Lenght;
}

/*************************************************************************************
*************************************************************************************/
void ReceivePacket(void){
  //  uint8_t Len, i, Status, Fifo_sta, Value, Checksum;

    RFM_Registers.Status = RFM_ReadRegister(STATUS); // read register STATUS's value
    RFM_Registers.Config = RFM_ReadRegister(CONFIG); /// TODO test jen pro zkouseni zarizeni

    if((RX_DR & RFM_Registers.Status) == 0x40){        // if receive data ready (RX_DR) interrupt
        do {
            RFM_Registers.RX_Pw_P0 = RFM_ReadPayloadWidth();  // read len

            if(RFM_Registers.RX_Pw_P0 <= MAX_PACKET_LEN){
                RFM_PayLoad_RX(RFM_Registers.RX_Pw_P0);// read receive payload from RX_FIFO buffer
            }
            else {
                RFM_WriteByte(FLUSH_RX);				//flush Rx
            }

            RFM_Registers.Fifo_Status = RFM_ReadRegister(FIFO_STATUS); // read register FIFO_STATUS's value

        }while((RFM_Registers.Fifo_Status & RX_EMPTY) == 0); //while not empty
    }
}
/*******************************************************************************
*******************************************************************************/
void RFM_SendCommand(uint8_t Command){
}

/*******************************************************************************
*******************************************************************************/
void RFM_SendPacket(PacketStruct* Packet){
    uint8_t Lenght = 4;///TODO load from packet structure

    RFM_SetModeTX();
    RFM_PayLoad_TX(Lenght);

    RFM_CE_HI;
    Delay_ms(1);//Loop(5);
    RFM_CE_LOW;
}

/*******************************************************************************
*******************************************************************************/
void RFM_Send_NACK_Packet(void){
    uint8_t i, Lenght = 4;///TODO load from packet structure

    RFM_SetModeTX();

    RFM_Registers.Fifo_Status = RFM_ReadRegister(FIFO_STATUS);

    if((RFM_Registers.Fifo_Status & TX_FULL) == 0){ //not full send
        RFM_CE_LOW;    //enable communication

        for(i = 0; i < 10; i++){
            RFM_WriteRegister(W_TX_PAYLOAD_NO_ACK, DataPacket.TX_PayloadData[i]);
        }

        RFM_CE_HI;
        Delay_ms(1);//Loop(5);
        RFM_CE_LOW;
    }
    Delay_ms(100);
}












