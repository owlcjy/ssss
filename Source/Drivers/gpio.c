/******************** (C) COPYRIGHT tongxinmao.com ***************************
* 文件名		: gpio.C
* 作者   		: tcm123@126.com
* 创建日期  	: 2016.3.8
* 描述			: GPIO驱动头文件 
* 版本			: V1.0
* 历史记录  	: 暂无
********************************************************************************/

/*--------------------------- 头文件包含 --------------------------------------*/
#include "gpio.h"
/*-----------------------------------------------------------------------------*/


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------         宏定义	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define SW2_V1_H( )     GPIO_SetBits( GPIOA, GPIO_Pin_11)
#define SW2_V1_L( )     GPIO_ResetBits( GPIOA, GPIO_Pin_11)
#define SW2_V2_H( )     GPIO_SetBits( GPIOB, GPIO_Pin_1)
#define SW2_V2_L( )     GPIO_ResetBits( GPIOB, GPIO_Pin_1)




//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------       变量定义	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------       函数声明	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------     私有函数定义	    ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------





/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );



//键盘
//PA0~3-KEYOUT1~4
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 //PB5-KEYIN1,PB6-KEYIN2,PB7-KEYIN3,
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 //PB11-KEY_K
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 //GPIO_Init(GPIOB, &GPIO_InitStructure); 
 //LCD
 GPIO_SetBits( GPIOB, GPIO_Pin_11);
 //PB0-LCDDC,PB1-NC,PB2-LCDRes,PB3-LCDCS代替//PB11-KEY_K
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 //Si4463
 //PA8-Si-SDN
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 //PB10-Si-IRQ,
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 //PB12-Si-SEL,
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 
 //PB13-Si-SCK-,PB14-Si-SDO,PB15-Si-SDI 别处单独初始化

 //LED及其他
 //PA11-LED3
 //PB4-RUNLED,JTAG不能用,PB8-POWER-KEY,PB9-POW_ON/OFF,
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 
 //USART 别处单独初始化
 //PA9-USART-TX,
 //PA10-USART-RX,


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
  
}

void BTEPGPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );



//键盘
//PA0~3-KEYOUT1~4
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 //PB5-KEYIN1,PB6-KEYIN2,PB7-KEYIN3,
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 //PB11-KEY_K
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 //GPIO_Init(GPIOB, &GPIO_InitStructure); 
 //LCD
 GPIO_SetBits( GPIOB, GPIO_Pin_11);
 //PB0-LCDDC,PB1-NC,PB2-LCDRes,PB3-LCDCS代替//PB11-KEY_K
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
  //PA11-LCD_BUSY
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 //PB13-LCDCLK,PB15-LCDMOSI
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_15;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 
#if 0
 //Si4463
 //PA8-Si-SDN
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 //PB10-Si-IRQ,
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 //PB12-Si-SEL,
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 
 //PB13-Si-SCK-,PB14-Si-SDO,PB15-Si-SDI 别处单独初始化
#endif
 //LED及其他

 //PB4-RUNLED,JTAG不能用,PB8-POWER-KEY,PB9-POW_ON/OFF,

 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 
 //USART 别处单独初始化
 //PA9-USART-TX,
 //PA10-USART-RX,


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
  
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------     公有函数定义	    ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------





/* end of Led_Driver.c -------------------------------------------------------------*/


