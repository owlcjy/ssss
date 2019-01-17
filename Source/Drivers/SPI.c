/******************** (C) COPYRIGHT tongxinmao.com ***************************
* �ļ���		: SPI.C
* ����   		: tcm123@126.com
* ��������  	: 2016.3.8
* ����			: SPI�����ļ� 
* �汾			: V1.0
* ��ʷ��¼  	: ����
********************************************************************************/
#include "stm32f10x_spi.h"

#include "stm32f10x.h"

#define RF_SI4463


/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_Init(void)
{
  
  
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    SPI_I2S_DeInit(SPI2);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    //SPI2??APB1?,????    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
       
    /* Configure SPI2 pins: SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;      //????SPI2_CLK?SPI3_MISO?SPI4_MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //??SPI???????????:SPI??????????
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;        //??SPI????:????SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;        //??SPI?????:SPI????8????

	#ifdef RF_SI4463
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//;        //??????????:?????  SI4463:SPI_CPOL_High	1278SPI_CPOL_Low

	#endif
   #if (defined RF_SI4432) || (defined RF_SX1278) 
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	////����ģʽ�� SCK Ϊ��
   	#endif

    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//SPI_CPHA_2Edge;    //???????????
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //NSS?????(NSS??)????(??SSI?)??:??NSS???SSI???
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;        //??????????:????????256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;    //???????MSB???LSB???:?????MSB???
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC???????
    SPI_Init(SPI2, &SPI_InitStructure);  //??SPI_InitStruct???????????SPIx???
    
    /* Enable SPI2  */
    SPI_Cmd(SPI2, ENABLE); //??SPI??
  
  
  
     
}
//Mode=0,LCD,Mode=1,Si
void SPI2_Init_CPOL(u8 Mode)
{
//����Ƭѡ״̬������ʱ����� 
if(Mode==0)
	{
	SPI2->CR1&=0xFFFD; 	//����ģʽ��SCKΪ0 CPOL=0
	}
else
	{
	SPI2->CR1|=1<<1; 	//����ģʽ��SCKΪ1 CPOL=1
	}

}


