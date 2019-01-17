
#ifndef __USART1_H__
#define __USART1_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Uart1Init(void);
void RCC_Configuration1(void);
void NVIC_Configuration1(void);
void GPIO_Configuration1(void);
void USART_Configuration1(void);
 void uart1_senddata(u8 *p,u16 num);

 void USART1_IRQHandler(void);
 INT8U  ASCtoData(INT8U ASC_HByte,INT8U ASC_LByte);
 u8 CharStr_Compare(u8 *str1, u8 *str2,u8 size);

 u32 FC_BCDToIntProc(void);
 
void FZCOMD_deal(void);
void FC_ProdName_deal(void);
void FC_ProdAdd_deal(void);
void FC_ItemNum_deal(void);
void FC_Price_deal(void);
void FC_MemPrice_deal(void);
void FC_ProdDate_deal(void);
void FC_ChavgeUnit_deal(void);

void UART_FROM_PC_deal(u8 rxdata);




#define COMD_NUM 800
#define JHDATA_NUM 160
#define FZCOMD_NUM 50


#endif

