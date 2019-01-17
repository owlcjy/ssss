/*
================================================================================
Function : Operation for SI446x
Taobao website:http://zeyaotech.taobao.com/
Official website:http://www.ashining.com/
Written by:Lijian
TEL:028-61772770, e-mail: service@ashining.com
================================================================================
*/
#include "board.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/*
=================================================================================
SPI_ExchangeByte( );
Function : Exchange a byte via the SPI bus
INTPUT   : input, The input byte
OUTPUT   : The output byte from SPI bus
=================================================================================
*/

u8 SPI_ExchangeByte( u8 input )
{

	
	/* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, input);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);	
	
	
	
	
}



/*
=================================================================================
SPI_Initial( );
Function : Initialize the SPI bus
INTPUT   : None
OUTPUT   : None
=================================================================================
*/
void SPI_Initial( void )
{
	SPI2_Init();

}

void TIM2_Base_Config_owl(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ж�ʱ��ʹ��
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//�ж�������flash��

  //�жϿ�������ѡ��TIM2
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

  //��ʱ����������
  TIM_TimeBaseStructure.TIM_Period = 0x4000;//���ؼ���ֵ
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_PSCReloadMode_Update;//���ֻ�ǶԷ�Ƶ���Ӻ�ʱװ�ؽ�������
  //TIM_TimeBaseStructure.TIM_Prescaler = 0x0002;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����ģʽ

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//����

  TIM_PrescalerConfig(TIM2, 0x0000, TIM_PSCReloadMode_Immediate);//��ʱ��ʹ�õ�ʱ�ӵķ�Ƶ����
  TIM_ITConfig(TIM2,  TIM_IT_Update , ENABLE);//�Զ�ʱ�����ĸ��ж�Դ�������á�
  TIM_Cmd(TIM2, ENABLE);//��ʱ��2������
}




/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
