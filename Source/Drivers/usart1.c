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

INT16U rxcomd_i=0;//�Խ��յ�����Ч��������ַ������м���
INT8U RX_COMD[400];//���յ�����Ч������
INT8U RX_COMD_HByte=0;
INT8U RX_COMD_LByte=0;

INT16U rxJHdata_i=0;//�Խ��յ�����Ч�ľ������ݵ��ַ������м���
INT8U RX_JHdata[JHDATA_NUM];//���յ�����Ч�ľ�������
INT8U RX_JHdata_HByte=0;
INT8U RX_JHdata_LByte=0;
INT8U Send_JHdata_Size=0;//���ش�������Ҫ���͵ľ��ɵĴ�С

//���������������
//���������ݴ�����
#define FZCOMD_NUM 50

INT16U rxFZCOMD_i=0;//�Խ��յ��ĸ���������ַ������м���
INT8U RX_FZCOMD[FZCOMD_NUM];//���յ��ĸ�������


//ȫ�����ñ���
INT8U SX_RESET_flg=0;//0,������,1,��λоƬ
INT8U SX_CONFIG_flg=0;//0,������,1,����оƬ




INT8U STU_RX=0;
//0x00:��ʼ״̬
//0x01:���յ�$
//0x02:���յ�������=
//0x03:���յ�������",���ڽ�������״̬
//0x04:���յ�����������β��"
//0x05:���յ����������ݽ�����#

//0x11:���յ���1�����������!
//0x12:���յ���2�����������!
//0x13:���յ���3�����������!
//0x14:���յ���4�����������!

//0x21:���յ���1����λ�����*
//0x22:���յ���2����λ�����*
//0x23:���յ���3����λ�����*
//0x24:���յ���4����λ�����*

//0x31:���յ�&
//0x32:���յ�������=
//0x33:���յ�������",���ڽ�������״̬
//0x34:���յ�����������β��"
//0x35:���յ����������ݽ�����#

//���������
//0x41:���յ�[
//0x42:���յ�]
u8	CG_ERRandOK_Flag=0;//PC���µ������Ƿ�OK
u8	CG_BTEP_configFlag=0;//���±�־


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
  
  //*****************************����1�жϳ�ʼ��******************************

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
		 while((USART1->SR&0X40)==0);//�ȴ����ͽ���			
		}
}


//******************************************************************************
//  ����(ģ��)����: void USART1_IRQHandler(void)
//  ����: ��������USART1�жϴ�����   
//  �������: ��       		
//  �������: ��      		 		
//  ����˵��: ��   
//******************************************************************************
void USART1_IRQHandler(void)
{
  // static unsigned char iRxdData = 0;
   u16 i=0; 
    USART_TypeDef* UartRev;                   // ��ȡUSART1�Ĵ���ֵ
    UartRev = USART1;

    if(( UartRev->SR & USART_FLAG_RXNE ) == USART_FLAG_RXNE )
    {        
        rxdata = UartRev->DR;               // �������ݴ���
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
			else if(rxdata=='<')//�����������ʼ��־
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
				//�������ݴ�����
				if((rxcomd_i%2)==0)//�����ż��
					{
					RX_COMD_HByte=rxdata;//�Ȱ����ݴ�����
					}
				else
					{
					RX_COMD_LByte=rxdata;//�Ȱ����ݴ�����
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
			if(rxdata=='<')//�����������ʼ��־
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
				//�������ݴ�����
				if((rxJHdata_i%2)==0)//�����ż��
					{
					RX_JHdata_HByte=rxdata;//�Ȱ����ݴ�����
					}
				else
					{
					RX_JHdata_LByte=rxdata;//�Ȱ����ݴ�����
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
			if(rxdata=='>')//�յ���������Ľ�����
				{
				STU_RX=0x00;				
				FZCOMD_deal();
				rxFZCOMD_i=0;//���Ϻ����Ĵ��������˱��������Ա�����������������ɺ��������˱���
				}		
			else							
				{
				//������������ݴ�����		
				RX_FZCOMD[rxFZCOMD_i]=rxdata;		
				rxFZCOMD_i++;
				if(rxFZCOMD_i>=FZCOMD_NUM)//�����ݳ�������ĳ��Ⱥ�״̬����
					{
					rxFZCOMD_i=0;
					STU_RX=0;
					}			
				}
		break;
		}

//����Ĵ�����򣬱�֤һ�����Ը�λ
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
	for(i=0;i<COMD_NUM;i++)//��մ洢��
		{
		RX_COMD[i]=0;
		}
	rxcomd_i=0;
	
	for(i=0;i<JHDATA_NUM;i++)//��մ洢��
		{
		RX_JHdata[i]=0;
		}
	rxJHdata_i=0;
	
	STU_RX=0;
}		
//�����յ�!!!!�󣬽����ݴ洢�������͵Ļ�������������JHDATA
if(STU_RX==0x14)
{;
#if 0
	if(rxcomd_i>0)//���յ���Ч��������ݳ��Ȳ�Ϊ0
		{
		for(i=0;i<(rxcomd_i/2);i++)
			{
			config_table[i]=RX_COMD[i];
			}
		//�洢��SPI flash
		SPI_Flash_Write(RX_COMD,COMD_ADDR,(rxcomd_i/2));		//�����������鶼д��ȥ
		rx_comd_num_i=(rxcomd_i/2);
		SPI_Flash_Write((u8*)&rx_comd_num_i,COMD_NUM_ADDR,sizeof(rx_comd_num_i));			
		CG_RADIO_configFlag =1;//������������
		CG_ERRandOK_Flag=1;//��PC��ӡOK
		}
	
	if(rxJHdata_i>0)//���յ���Ч��������ݳ��Ȳ�Ϊ0
		{
		JHdata_sour1_num=rxJHdata_i/2;
		for(i=0;i<(rxJHdata_i/2);i++)
			{
			//�Ժ��һ�£����ݵ�һ���ֽڱ�ʾ��������Դ����2~5��ʾ�ж��ٸ���������
			JHdata_sour1[i]=RX_JHdata[i];
			}
		//�洢��SPI flash
		SPI_Flash_Write(RX_JHdata,JHDATA_ADDR,(rxJHdata_i/2));		//�Ѿ������ݸ��ݻ�ȡ����д
		rx_jhdata_num_i=(rxJHdata_i/2);
		SPI_Flash_Write((u8*)&rx_jhdata_num_i,JHDATA_NUM_ADDR,sizeof(rx_jhdata_num_i));			
		CG_RADIO_configFlag =1;//������������
		CG_ERRandOK_Flag=1;//��PC��ӡOK
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

//ASCII�뵽���ݵ�ת��
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

//	u32	ProdName;//Ʒ��	1��������2�㽶��3����
//	s32	ProdAdd;//���� 1����2̩��3�㽭
//	s32	ItemNum;//����
//	s32	Price;//���ۣ���λ0.01Ԫ
//	s32	MemPrice;//��Ա�ۣ���λ0.01Ԫ
//	s32	ProdDate;//���ڸ�ʽ20180912
//	s32	ChavgeUnit;//�Ƽ۵�λ1��500g��2����3ֻ
//	u32 flash_OK;


void FZCOMD_deal(void)
{
INT8U i;


//��һ�����ж�����������
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
//�������뷶Χ0~1000000
void FC_ProdName_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//����:��ȡ��Ч�����ݣ����µ���ʾ�ṹ�壬���¶�����־Ϊ��֪ͨ�Ӱ���ж���
//��ȡ���е���ֵ
abc_all=FC_BCDToIntProc();
//��Ч�Լ��
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//��PC��ӡERROR
return;
}
//���µ����ñ���
BTEP_gTesterStatus.ProdName=abc_all;

//�������ñ�־λ
CG_BTEP_configFlag =ProdNameUpdata;//������������

CG_ERRandOK_Flag=1;//��PC��ӡOK
}
//<ProdName="1000">
//�������뷶Χ0~1000000
void FC_ProdAdd_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//����:��ȡ��Ч�����ݣ����µ���ʾ�ṹ�壬���¶�����־Ϊ��֪ͨ�Ӱ���ж���
//��ȡ���е���ֵ
abc_all=FC_BCDToIntProc();
//��Ч�Լ��
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//��PC��ӡERROR
return;
}
//���µ����ñ���
BTEP_gTesterStatus.ProdAdd=abc_all;

//�������ñ�־λ
CG_BTEP_configFlag =ProdAddUpdata;//������������

CG_ERRandOK_Flag=1;//��PC��ӡOK
}

//<ItemNum="1000">
//�������뷶Χ0~1000000
void FC_ItemNum_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//����:��ȡ��Ч�����ݣ����µ���ʾ�ṹ�壬���¶�����־Ϊ��֪ͨ�Ӱ���ж���
//��ȡ���е���ֵ
abc_all=FC_BCDToIntProc();
//��Ч�Լ��
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//��PC��ӡERROR
return;
}
//���µ����ñ���
BTEP_gTesterStatus.ItemNum=abc_all;

//�������ñ�־λ
CG_BTEP_configFlag =ItemNumUpdata;//������������

CG_ERRandOK_Flag=1;//��PC��ӡOK
}

//<Price="1000">
//�������뷶Χ0~1000000
void FC_Price_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//����:��ȡ��Ч�����ݣ����µ���ʾ�ṹ�壬���¶�����־Ϊ��֪ͨ�Ӱ���ж���
//��ȡ���е���ֵ
abc_all=FC_BCDToIntProc();
//��Ч�Լ��
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//��PC��ӡERROR
return;
}
//���µ����ñ���
BTEP_gTesterStatus.Price=abc_all;

//�������ñ�־λ
CG_BTEP_configFlag =PriceUpdata;//������������

CG_ERRandOK_Flag=1;//��PC��ӡOK
}

//<MemPrice="1000">
//�������뷶Χ0~1000000
void FC_MemPrice_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//����:��ȡ��Ч�����ݣ����µ���ʾ�ṹ�壬���¶�����־Ϊ��֪ͨ�Ӱ���ж���
//��ȡ���е���ֵ
abc_all=FC_BCDToIntProc();
//��Ч�Լ��
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//��PC��ӡERROR
return;
}
//���µ����ñ���
BTEP_gTesterStatus.MemPrice=abc_all;

//�������ñ�־λ
CG_BTEP_configFlag =MemPriceUpdata;//������������

CG_ERRandOK_Flag=1;//��PC��ӡOK
}

//<ProdDate="1000">
//�������뷶Χ0~1000000
void FC_ProdDate_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//����:��ȡ��Ч�����ݣ����µ���ʾ�ṹ�壬���¶�����־Ϊ��֪ͨ�Ӱ���ж���
//��ȡ���е���ֵ
abc_all=FC_BCDToIntProc();
//��Ч�Լ��
if(abc_all>20991231)
{
CG_ERRandOK_Flag=0xFF;//��PC��ӡERROR
return;
}
//���µ����ñ���
BTEP_gTesterStatus.ProdDate=abc_all;

//�������ñ�־λ
CG_BTEP_configFlag =ProdDateUpdata;//������������

CG_ERRandOK_Flag=1;//��PC��ӡOK
}

//<ChavgeUnit="1000">
//�������뷶Χ0~1000000
void FC_ChavgeUnit_deal(void)
{
	u32 abc_all;
	u8 i,len;
	u8 begin,end;
	
//����:��ȡ��Ч�����ݣ����µ���ʾ�ṹ�壬���¶�����־Ϊ��֪ͨ�Ӱ���ж���
//��ȡ���е���ֵ
abc_all=FC_BCDToIntProc();
//��Ч�Լ��
if(abc_all>1000000)
{
CG_ERRandOK_Flag=0xFF;//��PC��ӡERROR
return;
}
//���µ����ñ���
BTEP_gTesterStatus.ChavgeUnit=abc_all;

//�������ñ�־λ
CG_BTEP_configFlag =ChavgeUnitUpdata;//������������

CG_ERRandOK_Flag=1;//��PC��ӡOK
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
	memcpy(buf,&RX_FZCOMD[begin+1],len);//��BCDֵ���Ƶ�buf
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
