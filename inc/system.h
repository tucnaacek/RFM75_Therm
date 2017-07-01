#ifndef _H_SYSTEM_H_
#define _H_SYSTEM_H_

/* ====================== Knihovny STM32F427 =================================*/

//#include "misc.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cm3.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_syscfg.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
//#include "stm32f10xx_spi.h"
//#include "stm32f10xx_usart.h"

/* ====================== Dalsi headery knihovny =============================*/

#include <string.h>
#include "amit_types.h"
#include "delay.h"
#include "cycle_counter.h"

/* =========================== Povoleni periferii ============================*/

#define   AHB1_PERIF     (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | \
                          RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG | \
                          RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_CRC)
#define   AHB2_PERIF     (0)
#define   APB1_PERIF     (RCC_APB1Periph_PWR)
#define   APB2_PERIF     (RCC_APB2Periph_SYSCFG)

/* ==================== specialni sekce =================================== */

#define  __MEMORY_DMA_            __attribute__ ((section (".ramdma")))             // pamet pro DMA prenosy
#define  __RAM_FUNC_              __attribute__ ((long_call, section (".fastrun"))) // funkce v RAM pameti

/* ======================= Preruseni =========================================*/

// Pouzivame grouping 2, tedy 2 bity na prioritu a 2 bity na subprioritu
#define  INT_GROUP                  NVIC_PriorityGroup_2          // 2 bity na prioritu a 2 bity na subprioritu
#define  INT_PRIO_WIDTH             2
#define  INT_SUBPRIO_WIDTH          2
#define  INT_PRIO(prio,sub)         ((prio<<INT_SUBPRIO_WIDTH)+sub)

#define  ENABLE_PURE_INT()          __enable_irq()
#define  DISABLE_PURE_INT()         __disable_irq()

#define ENABLE_INT();      {  if (IntEnabledVar) IntEnabledVar--;         \
                              if (IntEnabledVar==0) {                     \
                                 __enable_irq();                          \
                              }                                           \
                           }
#define DISABLE_INT();     {  __disable_irq();                            \
                              IntEnabledVar++;                            \
                           }

#define IS_INT_ENABLED()   (IntEnabledVar==0)

extern VU32 IntEnabledVar;

// Seznam preruseni
// #define   INT_ICANF_PRIORITY           1        // Interni CAN radice - fast preruseni (cteni ramce)
// #define   INT_ICANF_SUBPRIORITY        0

/* =========================== Bitova adresace ============================= */

#define  BITBAND_SRAM_REF   0x20000000
#define  BITBAND_SRAM_BASE  0x22000000
#define  BITBAND_SRAM(a,b) ((BITBAND_SRAM_BASE+((a)-BITBAND_SRAM_REF)*32+((b)*4)))  // Convert SRAM address

#define  BITBAND_PERI_REF   0x40000000
#define  BITBAND_PERI_BASE  0x42000000
#define  BITBAND_PERI(a,b) ((BITBAND_PERI_BASE+((a)-BITBAND_PERI_REF)*32+((b)*4)))  // Convert PERI address

/* ============================ Dalsi define =============================== */

// Kalibrace spozdovaci smycky
#define CPU_DELAY_CALIB       11              // kalibrace delay, viz delay.h
#define HCLK                  84000000

// Pamet CPU
#define RTC_MEMORY_SIZE       80
#define RAM_BOTTOM            SRAM_BASE       // Adresa, od ktere zacina RAM
#define RAM_SIZE              0x10000         // Velikost RAM
#define STACK_DUMMY_MEMORY    0xA5A5A5A5      // Defaultni hodnota RAM
#define RAM_STACK_SIZE        0x2000          // 8kb stacku

/* ==================== dalsi funkce ====================================== */

#define CPU_RESET()           NVIC_SystemReset()      // Reset CPU

// Pouzivame nepresny ale spolehlivejsi asynchronni watchdog IWDG
#ifndef _DEBUG_
   #define WATCH_TICK();      IWDG_ReloadCounter();   // Refresh
#else
   #define WATCH_TICK();
#endif

/* ====================== headery modulu ================================== */

#include "systimer.h"
//#include "hid.h"
//#include "hal_io_support.h"
//#include "lib_aci.h"
//#include "aci_setup.h"
//#include "uart_over_ble.h"
//#include "services.h"

// Externi funkce
/* ====================== Vyuziti periferii =============================== */

unsigned long _etext;
unsigned long _data;
unsigned long _edata;
unsigned long _bss;
unsigned long _ebss;



/*
   DMA:
   Casovace:
   SPI:
   I2C:
   UART:
   EXTI:
*/


#endif
