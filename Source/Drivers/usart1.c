/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : Main program body
********************************************************************************
* History:
* 05/21/2007: V0.3
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_usart.h"
#include "usart1.h"
#include "board.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus1;


/* Private function prototypes -----------------------------------------------*/







INT8U RecNub=0;

INT8U rxdata=0;

INT16U rxcomd_i=0;//对接收到的有效的命令的字符数进行计数
INT8U RX_COMD[400];//接收到的有效的命令
INT8U RX_COMD_HByte=0;
INT8U RX_COMD_LByte=0;

INT16U rxJHdata_i=0;//对接收到的有效的净荷数据的字符数进行计数
INT8U RX_JHdata[JHDATA_NUM];//接收到的有效的净荷数据
INT8U RX_JHdata_HByte=0;
INT8U RX_JHdata_LByte=0;
INT8U Send_JHdata_Size=0;//主控传过来的要发送的净荷的大小

//串口新增命令处理方法
//辅助命令暂存数组
#define FZCOMD_NUM 50

INT16U rxFZCOMD_i=0;//对接收到的辅助命令的字符数进行计数
INT8U RX_FZCOMD[FZCOMD_NUM];//接收到的辅助命令


//全局配置变量
INT8U SX_RESET_flg=0;//0,不动作,1,复位芯片
INT8U SX_CONFIG_flg=0;//0,不动作,1,配置芯片




INT8U STU_RX=0;
//0x00:初始状态
//0x01:接收到$
//0x02:接收到命令组=
//0x03:接收到命令组",处于接收数据状态
//0x04:接收到命令组数据尾部"
//0x05:接收到命令组数据结束符#

//0x11:接收到第1个命令结束符!
//0x12:接收到第2个命令结束符!
//0x13:接收到第3个命令结束符!
//0x14:接收到第4个命令结束符!

//0x21:接收到第1个复位命令符*
//0x22:接收到第2个复位命令符*
//0x23:接收到第3个复位命令符*
//0x24:接收到第4个复位命令符*

//0x31:接收到&
//0x32:接收到命令组=
//0x33:接收到命令组",处于接收数据状态
//0x34:接收到命令组数据尾部"
//0x35:接收到命令组数据结束符#

//辅助命令处理
//0x41:接收到[
//0x42:接收到]
u8	CG_ERRandOK_Flag=0;//PC传下的命令是否OK
u8	CG_BTEP_configFlag=0;//更新标志


extern CG_Struct_BTEP_STATUS BTEP_gTesterStatus;

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart1Init(void)
{

   
#ifdef DEBUG
  debug();
#endif

  /* Configure the system clocks */
  RCC_Configuration1();
    
  /* NVIC Configuration */
 NVIC_Configuration1();

  /* Configure the GPIOs */
  GPIO_Configuration1();
  
  /* Configure the USART1 */
  USART_Configuration1();

 // printf("\r\n  -------------------------- \r\n");

}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number */

  printf("\n\r Wrong parameter value detected on\r\n");
  printf("       file  %s\r\n", file);
  printf("       line  %d\r\n", line);
    
  /* Infinite loop */
  /* while (1)
  {
  } */
}
#endif

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration1(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus1 = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus1 == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
   
  /* Enable USART1 and GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration1(void)
{
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Configuration1(void)
{
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef  USART_ClockInitStruct;
  NVIC_InitTypeDef NVIC_InitStructure;

/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


  
  USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;
  USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;
  USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;

  USART_ClockInit(USART1,&USART_ClockInitStruct);
  USART_Init(USART1, &USART_InitStructure);
  
  //*****************************串口1中断初始化******************************

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
  
  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
}

void uart1_senddata(u8 *p,u16 num)
{

	 u16 i;
	 u8  *pdata;
	 pdata=p;
		for(i=0;i<num;i++)
		{
		 USART1->DR=*pdata++;
		 while((USART1->SR&0X40)==0);//等待发送结束			
		}
}


//******************************************************************************
//  函数(模块)名称: void USART1_IRQHandler(void)
//  功能: 本函数是USART1中断处理函数   
//  输入参数: 无       		
//  输出参数: 无      		 		
//  其它说明: 无   
//******************************************************************************
void USART1_IRQHandler(void)
{
  // static unsigned char iRxdData = 0;
   u16 i=0; 
    USART_TypeDef* UartRev;                   // 获取USART1寄存器值
    UartRev = USART1;

    if(( UartRev->SR & USART_FLAG_RXNE ) == USART_FLAG_RXNE )
    {        
        rxdata = UartRev->DR;               // 接收数据传递
        UART_FROM_PC_deal(rxdata);
    }
	
}
void UART_FROM_PC_deal(u8 rxdata)
{

//$="C0FF0E0345"#
//!!!!
//****
u16 i;
u16 rx_comd_num_i=0;
u16 rx_jhdata_num_i=0;


	switch(STU_RX)
	{
		case 0x00:
		case 0x05:
		case 0x35:
			if(rxdata=='$')
				{
				STU_RX=0x01;
				}
			else if(rxdata=='!')
				{
				STU_RX=0x11;
				}
			else if(rxdata=='*')
				{
				STU_RX=0x21;
				}
			else if(rxdata=='&')
				{
				STU_RX=0x31;
				}
			else if(rxdata=='<')//辅助命令的起始标志
				{
				STU_RX=0x41;
				}
			else							
				{
				STU_RX=0x00;
				}
			break;

		case 0x01:
			if(rxdata=='=')
				{
				STU_RX=0x02;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x02:
			if(rxdata=='"')
				{
				STU_RX=0x03;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x03:
			if((rxdata>=0x30)&&(rxdata<=0x39) || (rxdata>=0x41)&&(rxdata<=0x46))
				{
				//主的数据处理方法
				if((rxcomd_i%2)==0)//如果是偶数
					{
					RX_COMD_HByte=rxdata;//先把数暂存起来
					}
				else
					{
					RX_COMD_LByte=rxdata;//先把数暂存起来
					RX_COMD[rxcomd_i/2]=ASCtoData(RX_COMD_HByte,RX_COMD_LByte);
					}
				rxcomd_i++;
				
				}		
			else if(rxdata=='"')							
				{
				STU_RX=0x04;
				}
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x04:
			if(rxdata=='#')
				{
				STU_RX=0x05;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x11:
			if(rxdata=='!')
				{
				STU_RX=0x12;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x12:
			if(rxdata=='!')
				{
				STU_RX=0x13;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x13:
			if(rxdata=='!')
				{
				STU_RX=0x14;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x14:
			if(rxdata=='*')
				{
				STU_RX=0x21;
				}		
			else							
				{
				STU_RX=0x14;
				}
			if(rxdata=='<')//辅助命令的起始标志
				{
				STU_RX=0x41;
				}
		break;
		case 0x21:
			if(rxdata=='*')
				{
				STU_RX=0x22;
				}		
			else							
				{
				STU_RX=0x14;
				}
		break;
		case 0x22:
			if(rxdata=='*')
				{
				STU_RX=0x23;
				}		
			else							
				{
				STU_RX=0x14;
				}
		break;
		case 0x23:
			if(rxdata=='*')
				{
				STU_RX=0x24;
				}		
			else							
				{
				STU_RX=0x14;
				}
		break;
		case 0x31:
			if(rxdata=='=')
				{
				STU_RX=0x32;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x32:
			if(rxdata=='"')
				{
				STU_RX=0x33;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x33:
			if((rxdata>=0x30)&&(rxdata<=0x39) || (rxdata>=0x41)&&(rxdata<=0x46))
				{				
				//主的数据处理方法
				if((rxJHdata_i%2)==0)//如果是偶数
					{
					RX_JHdata_HByte=rxdata;//先把数暂存起来
					}
				else
					{
					RX_JHdata_LByte=rxdata;//先把数暂存起来
					RX_JHdata[rxJHdata_i/2]=ASCtoData(RX_JHdata_HByte,RX_JHdata_LByte);
					}
				rxJHdata_i++;
				if(rxJHdata_i>=JHDATA_NUM)
					{
					rxJHdata_i=0;
					}				
				}		
			else if(rxdata=='"')							
				{
				STU_RX=0x34;
				}
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x34:
			if(rxdata=='#')
				{
				STU_RX=0x35;
				}		
			else							
				{
				STU_RX=0x00;
				}
		break;
		case 0x41:
			if(rxdata=='>')//收到辅助命令的结束符
				{
				STU_RX=0x00;				
				FZCOMD_deal();
				rxFZCOMD_i=0;//以上函数的处理依赖此变量，所以必须上述函数处理完成后才能清除此变量
				}		
			else							
				{
				//辅助命令的数据处理方法		
				RX_FZCOMD[rxFZCOMD_i]=rxdata;		
				rxFZCOMD_i++;
				if(rxFZCOMD_i>=FZCOMD_NUM)//当数据超过合理的长度后，状态归零
					{
					rxFZCOMD_i=0;
					STU_RX=0;
					}			
				}
		break;
		}

//额外的处理程序，保证一定可以复位
if((STU_RX!=0x21)&&(STU_RX!=0x22)&&(STU_RX!=0x23)&&(STU_RX!=0x24)&&(STU_RX!=0x00)&&(STU_RX!=0x05))
{
	if(rxdata=='*')
		{
		STU_RX=0x00;
		}
}

if(STU_RX==0x24)
{
	//IWDG_Init(0,0x00ff);
	for(i=0;i<COMD_NUM;i++)//清空存储区
		{
		RX_COMD[i]=0;
		}
	rxcomd_i=0;
	
	for(i=0;i<JHDATA_NUM;i++)//清空存储区
		{
		RX_JHdata[i]=0;
		}
	rxJHdata_i=0;
	
	STU_RX=0;
}		
//当接收到!!!!后，将数据存储到待发送的缓存里，包括命令和JHDATA
if(STU_RX==0x14)
{;
#if 0
	if(rxcomd_i>0)//接收到有效的命令，数据长度不为0
		{
		for(i=0;i<(rxcomd_i/2);i++)
			{
			config_table[i]=RX_COMD[i];
			}
		//存储到SPI flash
		SPI_Flash_Write(RX_COMD,COMD_ADDR,(rxcomd_i/2));		//把命令定义的数组都写进去
		rx_comd_num_i=(rxcomd_i/2);
		SPI_Flash_Write((u8*)&rx_comd_num_i,COMD_NUM_ADDR,sizeof(rx_comd_num_i));			
		CG_RADIO_configFlag =1;//更新无线配置
		CG_ERRandOK_Flag=1;//向PC打印OK
		}
	
	if(rxJHdata_i>0)//接收到有效的命令，数据长度不为0
		{
		JHdata_sour1_num=rxJHdata_i/2;
		for(i=0;i<(rxJHdata_i/2);i++)
			{
			//以后改一下，数据第一个字节表示几号数据源，第2~5表示有多少个二进制数
			JHdata_sour1[i]=RX_JHdata[i];
			}
		//存储到SPI flash
		SPI_Flash_Write(RX_JHdata,JHDATA_ADDR,(rxJHdata_i/2));		//把净荷数据根据获取的来写
		rx_jhdata_num_i=(rxJHdata_i/2);
		SPI_Flash_Write((u8*)&rx_jhdata_num_i,JHDATA_NUM_ADDR,sizeof(rx_jhdata_num_i));			
		CG_RADIO_configFlag =1;//更新无线配置
		CG_ERRandOK_Flag=1;//向PC打印OK
		}	
#endif
	
	//STU_RX=0;
}		

}

u8 CharStr_Compare(u8 *str1, u8 *str2,u8 size)
{
	u8 i;
	if(size>50)
		{
		return 0xFF;
		}
	for(i=0;i<size;i++)
	{
		if((*(str1+i))!=(*(str2+i)))
		{
			break;
		}
	}

	if(i==size)
		{
		return 0x01;
		}
	else
		{
		return 0xFF;
		}
}

//ASCII码到数据的转换
u8 ASCtoData(u8 ASC_HByte,u8 ASC_LByte)
{
	u8 out_data_h,out_data_l;
	
	if((ASC_HByte>=0x30)&&(ASC_HByte<=0x39))
		{
		out_data_h=(ASC_HByte-0x30)*16;
		}
	else if((ASC_HByte>=0x41)&&(ASC_HByte<=0x46))
		{
		out_data_h=(ASC_HByte-0x37)*16;
		}
	else
		{
		out_data_h=0;
		}
	
	if((ASC_LByte>=0x30)&&(ASC_LByte<=0x39))
		{
		out_data_l=(ASC_LByte-0x30);
		}
	else if((ASC_LByte>=0x41)&&(ASC_LByte<=0x46))
		{
		out_data_l=(ASC_LByte-0x37);
		}
	else
		{
		out_data_l=0;
		}	
		
return (out_data_h+out_data_l);

}

//	u32	ProdName;//品名	1火龙果，2香蕉，3西瓜
//	s32	ProdAdd;//产地 1海南2泰国3浙江
//	s32	ItemNum;//货号
//	s32	Price;//单价，单位0.01元
//	s32	MemPrice;//会员价，单位0.01元
//	s32	ProdDate;//日期格式20180912
//	s32	ChavgeUnit;//计价单位1，500g，2个，3只
//	u32 flash_OK;


void FZCOMD_deal(void)
{
INT8U i;


//第一步，判断是哪条命令
if(1==CharStr_Compare(RX_FZCOMD,"ProdName=",sizeof("ProdName=")-1))
{
	FC_ProdName_deal();
}
else if(1==CharStr_Compare(RX_FZCOMD,"ProdAdd=",sizeof("ProdAdd=")-1))
{
	FC_ProdAdd_deal();
}
else if(1==CharStr_Compare(RX_FZCOMD,"ItemNum=",sizeof("ItemNum=")-1))
{
	FC_ItemNum_deal();
}
else if(1==CharStr_Compare(RX_FZCOMD,"Price=",sizeof("Price=")-1))
{
	FC_Price_deal();
}
else if(1==CharStr_Compare(RX_FZCOMD,"MemPrice=",sizeof("MemPrice=")-1))
{
	FC_MemPrice_deal();
}
else if(1==CharStr_Compare(RX_FZCOMD,"ProdDate=",sizeof("ProdDate=")-1))
{
	FC_ProdDate_deal();
}
else if(1==CharStr_Compare(RX_FZCOMD,"ChavgeUnit=",sizeof("ChavgeUnit=")-1))
{
	FC_ChavgeUnit_deal();
}
else
{
;
}
}


//<ProdName="1000">
//允许输入范围0~1000000
void FC_ProdName_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//功能:提取有效的数据，更新到显示结构体，更新动作标志为，通知子板进行动作
//提取其中的数值
abc_all=FC_BCDToIntProc();
//有效性检查
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//向PC打印ERROR
return;
}
//更新到设置变量
BTEP_gTesterStatus.ProdName=abc_all;

//更新设置标志位
CG_BTEP_configFlag =ProdNameUpdata;//更新无线配置

CG_ERRandOK_Flag=1;//向PC打印OK
}
//<ProdName="1000">
//允许输入范围0~1000000
void FC_ProdAdd_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//功能:提取有效的数据，更新到显示结构体，更新动作标志为，通知子板进行动作
//提取其中的数值
abc_all=FC_BCDToIntProc();
//有效性检查
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//向PC打印ERROR
return;
}
//更新到设置变量
BTEP_gTesterStatus.ProdAdd=abc_all;

//更新设置标志位
CG_BTEP_configFlag =ProdAddUpdata;//更新无线配置

CG_ERRandOK_Flag=1;//向PC打印OK
}

//<ItemNum="1000">
//允许输入范围0~1000000
void FC_ItemNum_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//功能:提取有效的数据，更新到显示结构体，更新动作标志为，通知子板进行动作
//提取其中的数值
abc_all=FC_BCDToIntProc();
//有效性检查
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//向PC打印ERROR
return;
}
//更新到设置变量
BTEP_gTesterStatus.ItemNum=abc_all;

//更新设置标志位
CG_BTEP_configFlag =ItemNumUpdata;//更新无线配置

CG_ERRandOK_Flag=1;//向PC打印OK
}

//<Price="1000">
//允许输入范围0~1000000
void FC_Price_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//功能:提取有效的数据，更新到显示结构体，更新动作标志为，通知子板进行动作
//提取其中的数值
abc_all=FC_BCDToIntProc();
//有效性检查
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//向PC打印ERROR
return;
}
//更新到设置变量
BTEP_gTesterStatus.Price=abc_all;

//更新设置标志位
CG_BTEP_configFlag =PriceUpdata;//更新无线配置

CG_ERRandOK_Flag=1;//向PC打印OK
}

//<MemPrice="1000">
//允许输入范围0~1000000
void FC_MemPrice_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//功能:提取有效的数据，更新到显示结构体，更新动作标志为，通知子板进行动作
//提取其中的数值
abc_all=FC_BCDToIntProc();
//有效性检查
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//向PC打印ERROR
return;
}
//更新到设置变量
BTEP_gTesterStatus.MemPrice=abc_all;

//更新设置标志位
CG_BTEP_configFlag =MemPriceUpdata;//更新无线配置

CG_ERRandOK_Flag=1;//向PC打印OK
}

//<ProdDate="1000">
//允许输入范围0~1000000
void FC_ProdDate_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//功能:提取有效的数据，更新到显示结构体，更新动作标志为，通知子板进行动作
//提取其中的数值
abc_all=FC_BCDToIntProc();
//有效性检查
if(abc_all>20991231)
{
CG_ERRandOK_Flag=0xFF;//向PC打印ERROR
return;
}
//更新到设置变量
BTEP_gTesterStatus.ProdDate=abc_all;

//更新设置标志位
CG_BTEP_configFlag =ProdDateUpdata;//更新无线配置

CG_ERRandOK_Flag=1;//向PC打印OK
}

//<ChavgeUnit="1000">
//允许输入范围0~1000000
void FC_ChavgeUnit_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//功能:提取有效的数据，更新到显示结构体，更新动作标志为，通知子板进行动作
//提取其中的数值
abc_all=FC_BCDToIntProc();
//有效性检查
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//向PC打印ERROR
return;
}
//更新到设置变量
BTEP_gTesterStatus.ChavgeUnit=abc_all;

//更新设置标志位
CG_BTEP_configFlag =ChavgeUnitUpdata;//更新无线配置

CG_ERRandOK_Flag=1;//向PC打印OK
}


u32 FC_BCDToIntProc(void)
{
    u8 i,len;
    u32 data;
	u8 begin,end;
    u8 buf[30];
	for(i=0;i<rxFZCOMD_i;i++)
		{
		if(RX_FZCOMD[i]=='"')
			{
			begin=i;
			break;
			}
		}
	for(i=begin+1;i<rxFZCOMD_i;i++)
		{
		if(RX_FZCOMD[i]=='"')
			{
			end=i;
			break;
			}
		}
	
	len=end-begin-1;
	memcpy(buf,&RX_FZCOMD[begin+1],len);//将BCD值复制到buf
    if(len >0 && len <= 1)
    {
        data = buf[0] - 0x30;
    }
    else
    {
        data = buf[0] - 0x30;
        for(i=1;i<len;i++)
        {
            data = data*10 + buf[i] - 0x30;
        }
    }
    return data;

}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
