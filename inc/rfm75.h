#ifndef RFM75H
#define RFM75H

// Registers mapping

#include "stdint.h"
#include "stdint-gcc.h"
#include "wirelesstmp.h"

/************************************************************************************************************************
 ************************************************************************************************************************/
    #define RFM_CSN_LOW			  RFM_CTRL_GPIO->BRR |= RFM_CSN
    #define RFM_CSN_HI			  RFM_CTRL_GPIO->BSRR |= RFM_CSN
    #define RFM_CE_LOW			  RFM_CTRL_GPIO->BRR |= RFM_CE
    #define RFM_CE_HI			  RFM_CTRL_GPIO->BSRR |= RFM_CE
//    #define SHT_SCK_SDA_HI		SHT_GPIO->BSRR |= (SHT_SCK | SHT_SDA)
//    #define SHT_SCK_SDA_LOW		SHT_GPIO->BRR |= (SHT_SCK | SHT_SDA)

    #define MAX_PACKET_LEN        32



/************************************************************************************************************************
 ************************************************************************************************************************/
typedef volatile struct RFM75_MapRegisters {
    volatile uint8_t RegNumber;
    volatile uint8_t Config;//00
    volatile uint8_t En_aa;//01
    volatile uint8_t En_RXAddr;//02
    volatile uint8_t Setup_AW;//03
    volatile uint8_t Setup_Retr;//04 Automatic retransmisson
    volatile uint8_t RF_Ch;//05 RF Channel
    volatile uint8_t RF_Setup;//06 RF Setup Register
    volatile uint8_t Status;//07
    volatile uint8_t Observe_TX;//08
    volatile uint8_t Cd;//09 carrier detect
    volatile uint8_t RX_Addr_P0;//08
    volatile uint8_t RX_Addr_P1;//09 carrier detect
    volatile uint8_t RX_Addr_P2;//09 carrier detect
    volatile uint8_t RX_Addr_P3;//09 carrier detect
    volatile uint8_t RX_Addr_P4;//09 carrier detect
    volatile uint8_t RX_Addr_P5;//09 carrier detect
    volatile uint8_t TX_Addr;//10
    volatile uint8_t RX_Pw_P0;//11
    volatile uint8_t RX_Pw_P1;//12
    volatile uint8_t RX_Pw_P2;//13
    volatile uint8_t RX_Pw_P3;//14
    volatile uint8_t RX_Pw_P4;//15
    volatile uint8_t RX_Pw_P5;//16
    volatile uint8_t Fifo_Status;//17
    volatile uint8_t Ack_Pld;// Not writeable by special command
    volatile uint8_t TX_Pld;//Not writeable by special command
    volatile uint8_t RX_Pld;//Not writeable by special command
    volatile uint8_t DynPd;//0x1c
    volatile uint8_t Feature;//0x1d
} RFM75_MapRegisters;

RFM75_MapRegisters RFM_Registers;
RFM75_MapRegisters RFM_RegistersTMP;
//RFM75_MapRegisters* p_RFM75_MapRegisters;



/************************************************************************************************************************
 ************************************************************************************************************************/
typedef volatile struct {
    volatile uint8_t Preamble;
    volatile uint8_t TX_Address[5];
    volatile uint8_t RX_Address[5];
    volatile uint16_t PacketControl;
    volatile uint8_t RX_PayloadData[32];
    volatile uint8_t TX_PayloadData[32];
    uint16_t Crc;
} PacketStruct;

PacketStruct DataPacket;
PacketStruct *p_PacketStruct;

/************************************************************************************************************************
 ************************************************************************************************************************/
typedef enum {
  POWER_DOWN = 0,
  STBY_MODE1 = 1,
  STBY_MODE2 = 2,
  TX_MODE = 3,
  RX_MODE = 4,
} MRF_MODE;

/**************************************************
RFMx Registers Addresses
 *************************************************/
#define  CONFIG         0x00//00
#define  EN_AA          0x01
#define  EN_RXADDR      0x02
#define  SETUP_AW       0x03
#define  SETUP_RETR     0x04 //Automatic retransmisson
#define  RF_CH          0x05 //RF Channel
#define  RF_SETUP       0x06 //RF Setup Register
#define  STATUS         0x07
#define  OBSERVE_TX     0x08
#define  CD             0x09 //CArrier detect
#define  RX_ADDR_P0     0x0a
#define  RX_ADDR_P1     0x0b //09 carrier detect
#define  RX_ADDR_P2     0x0c //09 carrier detect
#define  RX_ADDR_P3     0x0d //09 carrier detect
#define  RX_ADDR_P4     0x0e //09 carrier detect
#define  RX_ADDR_P5     0x0f //09 carrier detect
#define  TX_ADDR        0x10
#define  RX_PW_P0       0x11
#define  RX_PW_P1       0x12
#define  RX_PW_P2       0x13
#define  RX_PW_P3       0x14
#define  RX_PW_P4       0x15
#define  RX_PW_P5       0x16
#define  FIFO_STATUS    0x17
#define  ACK_PLD        0// Not writeable by special command
#define  TX_PLD         0// Not writeable by special command
#define  RX_PLD         0// Not writeable by special command
#define  DYNPD          0X1c
#define  FEATURE        0x1d
#define  BNK1_00        0x00
#define  BNK1_01        0x01
#define  BNK1_02        0x02
#define  BNK1_03        0x03
#define  BNK1_04        0x04
#define  BNK1_05        0x05
//#define  BNK1_06        0x06
#define  BNK1_07        0x07
#define  BNK1_08        0x08
//#define  BNK1_09        0x09
//#define  BNK1_0a        0x0a
//#define  BNK1_0b        0x0b
#define  BNK1_0c        0x0c
#define  BNK1_0d        0x0d
#define  BNK1_0e        0x0e


/**************************************************
RFMx Commands definition
 *************************************************/
#define     R_REGISTER              0x00
#define     W_REGISTER              0x20
#define     R_RX_PAYLOAD            0x61
#define     W_TX_PAYLOAD            0xa0
#define     FLUSH_TX                0xe1
#define     FLUSH_RX                0xe2
#define     REUSE_TX_PL             0xe3
#define     ACTIVATE                0x50
#define     R_RX_PL_WID             0x60
#define     W_ACK_PAYLOAD           0xa8
#define     W_TX_PAYLOAD_NO_ACK     0xb0
#define     NOP                     0xff

/**************************************************
RFMx Constants definition
 *************************************************/
//output power
#define     RF_PWR0             0x00
#define     RF_PWR1             (0x01 << 1)
#define     RF_PWR2             (0x10 << 1)
#define     RF_PWR3             (0x11 << 1)
//gain
#define     RF_GAIN_LO          0x00
#define     RF_GAIN_HI          0x02
//retransmit time
#define     RETR_TIME_250us     0x00
#define     RETR_TIME_500us     0x10
#define     RETR_TIME_750us     0x20
#define     RETR_TIME_1000us    0x30
#define     RETR_TIME_1250us    0x40
#define     RETR_TIME_1500us    0x50
#define     RETR_TIME_1750us    0x60
#define     RETR_TIME_2000us    0x70
#define     RETR_TIME_2250us    0x80
#define     RETR_TIME_2500us    0x90
#define     RETR_TIME_2750us    0xa0
#define     RETR_TIME_3000us    0xb0
#define     RETR_TIME_3250us    0xc0
#define     RETR_TIME_3500us    0xd0
#define     RETR_TIME_3750us    0xe0
#define     RETR_TIME_4000us    0xf0
//retransmit count
#define     RETR_OFF            0x00
#define     RETR_COUNT_1        0x01
#define     RETR_COUNT_2        0x02
#define     RETR_COUNT_3        0x03
#define     RETR_COUNT_4        0x04
#define     RETR_COUNT_5        0x05
#define     RETR_COUNT_6        0x06
#define     RETR_COUNT_7        0x07
#define     RETR_COUNT_8        0x08
#define     RETR_COUNT_9        0x09
#define     RETR_COUNT_10       0x0a
#define     RETR_COUNT_11       0x0b
#define     RETR_COUNT_12       0x0c
#define     RETR_COUNT_13       0x0d
#define     RETR_COUNT_14       0x0e
#define     RETR_COUNT_15       0x0f
//air data rate
#define     RF_DR_1Mbps         0x00
#define     RF_DR_250Kbps       0x20
#define     RF_DR_2Mbps         0x08
#define     RF_DR_2MbpsDef      0x28
//channel numbers
#define     CHANNEL0            0x00 //2400 MHz
#define     CHANNEL1            0x1e
#define     CHANNEL2            0x3c
#define     CHANNEL3            0x53
// Address width
#define     WADDR3              0x01
#define     WADDR4              0x02
#define     WADDR5              0x03


/**************************************************
RFMx bits definition
 *************************************************/
//config reg
#define     PRIM_RX             0x01
#define     PWR_UP              0x02
#define     CRCO                0x04
#define     EN_CRC              0x08
#define     MASK_MAX_RT         0x10
#define     MASK_TX_DS          0x20
#define     MASK_RX_DR          0x40
//#define     RESERVED                 0x80

/**************************************************
 *************************************************/
//fifo_status reg
#define     RX_EMPTY            0x01
#define     RX_FULL             0x02
//#define     RESERVED            0x04
//#define     RESERVED            0x08
#define     TX_EMPTY            0x10
#define     TX_FULL             0x20
#define     TX_REUSE            0x40
//#define     RESERVED                 0x80

/**************************************************
 *************************************************/
//status reg
#define     RBANK               0x80
#define     RX_DR               0x40
#define     TX_DS               0x20
#define     MAX_RT              0x80
#define     RX_P_NO2            0x08
#define     RX_P_NO1            0x04
#define     RX_P_NO0            0x02
#define     STA_TX_FULL         0x01

#define     MSPS1               1
#define     RSSI_MSPS1          1

uint8_t RFM_ReadRegister(uint8_t Register);
void RFM_WriteRegister(uint8_t Register, uint8_t Value);
void RFM_WriteRegisterBuf(uint8_t Register, uint8_t* Buffer, uint8_t NmbBytes);
void RFM_WriteRegisterBuf1(uint8_t Register, uint8_t** Buffer, uint8_t NmbBytes);
void RFM_ReadRegisterBuf(uint8_t Register, uint8_t* Buffer, uint8_t NmbBytes);
uint8_t RFM_CountLostPackets(void);
uint8_t RFM_CountRetransPackets(void);
void RFM_TX_ReuseSet(void);
void RFM_TX_ReuseReset(void);
void RFM_InitInterface(void);
void SetCommunication(void);
void RFM_SetModeTX(void);
void RFM_SetModeRX(void);
void RFM_SetModeSleep(void);
void RFM_SetChannel(uint8_t Channel);
uint8_t RFM_ReadPipeNumber(void);
uint8_t RFM_CarrierDetect(void);
uint8_t RFM_TX_Full(void);
uint8_t RFM_RX_Empty(void);
uint8_t RFM_ReadByte(void);
uint8_t RFM_ReadPayloadWidth(void);
void RFM_SetTXPower(uint8_t Pwr);
void RFM_SetRFGain(uint8_t Gain);
void RFM_WriteByte(uint8_t Byte);
void RFM_SetAddressWidth(uint8_t Width);
void RFM_SetRXAddress(volatile uint8_t Address[], uint8_t AddrWidth, uint8_t PipeNumber);
void RFM_SetTXAddress(volatile uint8_t Address[], uint8_t AddrWidth, uint8_t PipeNumber);
void RFM_PayLoad_TX(uint8_t DataLenght);
void RFM_PayLoad_RX(uint8_t DataLenght);
void RFM_SendPacket(PacketStruct* Packet);
void ReceivePacket(void);
void RFM_SendCommand(uint8_t Command);
void RFM_InitCommunication(void);
void RFM_ReadConfig(void);
void RFM_SetStandby_I(void);
void RFM_SetRegBank(uint8_t Bank);
void RFM_Activate(uint8_t);
void RFM_Send_NACK_Packet(void);
#endif


