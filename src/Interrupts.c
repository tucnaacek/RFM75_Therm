//#include "buffer.h"
//#include "system.h"
//#include "main.h"

/************************************************************************************************************************
 ************************************************************************************************************************/
void USART1_IRQHandler(void)
{
  //TODO: faster way!!!
  char val;
//while(1)
    ;
 // if (USART1->ISR&(1<<(USART_IT_TXE&0xFF)))
  //if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
//  {
//    /* Write one byte to the transmit data register */
//    if (bgetc(&MessageBuffer,&val)) USART_SendData(USART1, val);
//    else USART1->CR1&=~(1<<(USART_IT_TXE&0xFF));
//  }

//  if (USART1->ISR&(1<<(3&0xFF)))
//  {
//	  USART1->ICR=(1<<(3&0xFF));
//  }
}

/************************************************************************************************************************
 ************************************************************************************************************************/
