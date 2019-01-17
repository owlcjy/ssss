/*
================================================================================
Function : Operation for SI446x
Taobao website:http://zeyaotech.taobao.com/
Official website:http://www.ashining.com/
Written by:Lijian
TEL:028-61772770, e-mail: service@ashining.com
================================================================================
*/
#ifndef _BOARD_H_
#define _BOARD_H_

#include "stm32f10x.h"
#include "Delay.h"

//#include "mytypedef.h"

/*Exchange a byte via the SPI bus*/
INT8U SPI_ExchangeByte( INT8U input );

/*Initialize the SPI bus*/
void SPI_Initial( void );

void TIM2_Base_Config_owl(void);


void delay_us(u32 num);


/*turn on the LED*/
#define LED_On( )       GPIO_ResetBits( GPIOB, GPIO_Pin_0 )

/*turn off the LED*/
#define LED_Off( )      GPIO_SetBits( GPIOB, GPIO_Pin_0 )



#define LED3_On( )       GPIO_ResetBits( GPIOA, GPIO_Pin_11 )
#define LED3_Off( )      GPIO_SetBits( GPIOA, GPIO_Pin_11)

//PB0-LCDDC,PB1-NC,PB2-LCDRes,PB3-LCDCS

//--------------operations for OLED module------------------------
#define OLED_CSN_H( )   GPIO_SetBits( GPIOB, GPIO_Pin_11)
#define OLED_CSN_L( )   GPIO_ResetBits( GPIOB, GPIO_Pin_11)

#define OLED_CMD_H( )   GPIO_SetBits( GPIOB, GPIO_Pin_0)
#define OLED_CMD_L( )   GPIO_ResetBits( GPIOB, GPIO_Pin_0)

#define OLED_RST_H( )   GPIO_SetBits( GPIOB, GPIO_Pin_2)
#define OLED_RST_L( )   GPIO_ResetBits( GPIOB, GPIO_Pin_2)

#define POME_MAX    128//ƽ����������ֵ����Ĳ���������

typedef	struct
{
	u8	curMenuIndex;	//��ǰ�˵�״̬:
	u32 flash_OK;
}CG_Struct_MENU_STATUS;


//�洢�豸�Ĺؼ�״̬
typedef	struct
{

	u32	Freq;//Ƶ����Ϣ����λ1kHz
	
	s32	Power;//������Ϣ����λ1dbm,��ʾ��
	s32 Power_average;//���ʵ�ƽ��ֵ������ƽ��
	s32 Power_peak;//���ʵķ�ֵ
	s32 Power_realtime;//���ʵ�ʵʱֵ
	
	//��ͬģʽ���и����ӵļ��㷽ʽ
	s32 PoweMeasure[POME_MAX];
	
	u8 PoMe_i;//���ʲ�������
	u32	Key;//������Ϣ����λ0.1dbm

	u32	Span;//Ƶ����Ϣ����λ1kHz

	
	u8 Mode;//ģʽѡ��1:����ģʽ��2:ƽ��ģʽ3:��ֵģʽ

	u32 flash_OK;
}CG_Struct_TESTER_STATUS;

//�洢�豸�Ĺؼ�״̬
typedef	struct
{

	u32	ProdName;//Ʒ��	1��������2�㽶��3����
	s32	ProdAdd;//���� 1����2̩��3�㽭
	s32	ItemNum;//����
	s32	Price;//���ۣ���λ0.01Ԫ
	s32	MemPrice;//��Ա�ۣ���λ0.01Ԫ
	s32	ProdDate;//���ڸ�ʽ20180912
	s32	ChavgeUnit;//�Ƽ۵�λ1��500g��2����3ֻ
	u32 flash_OK;
}CG_Struct_BTEP_STATUS;
typedef enum {
	ProdNameUpdata=10,//Ʒ��	1��������2�㽶��3����
	ProdAddUpdata,//���� 1����2̩��3�㽭
	ItemNumUpdata,//����
	PriceUpdata,//���ۣ���λ0.01Ԫ
	MemPriceUpdata,//��Ա�ۣ���λ0.01Ԫ
	ProdDateUpdata,//���ڸ�ʽ20180912
	ChavgeUnitUpdata,//�Ƽ۵�λ1��500g��2����3ֻ
} CG_BTEP_configFlagEnum;


#endif //_BOARD_H_
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
