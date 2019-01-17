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

#define POME_MAX    128//平均计数及峰值计算的采样点数量

typedef	struct
{
	u8	curMenuIndex;	//当前菜单状态:
	u32 flash_OK;
}CG_Struct_MENU_STATUS;


//存储设备的关键状态
typedef	struct
{

	u32	Freq;//频率信息，单位1kHz
	
	s32	Power;//功率信息，单位1dbm,显示用
	s32 Power_average;//功率的平均值，滑动平均
	s32 Power_peak;//功率的峰值
	s32 Power_realtime;//功率的实时值
	
	//不同模式，有更复杂的计算方式
	s32 PoweMeasure[POME_MAX];
	
	u8 PoMe_i;//功率测量计数
	u32	Key;//功率信息，单位0.1dbm

	u32	Span;//频率信息，单位1kHz

	
	u8 Mode;//模式选择1:正常模式，2:平均模式3:峰值模式

	u32 flash_OK;
}CG_Struct_TESTER_STATUS;

//存储设备的关键状态
typedef	struct
{

	u32	ProdName;//品名	1火龙果，2香蕉，3西瓜
	s32	ProdAdd;//产地 1海南2泰国3浙江
	s32	ItemNum;//货号
	s32	Price;//单价，单位0.01元
	s32	MemPrice;//会员价，单位0.01元
	s32	ProdDate;//日期格式20180912
	s32	ChavgeUnit;//计价单位1，500g，2个，3只
	u32 flash_OK;
}CG_Struct_BTEP_STATUS;
typedef enum {
	ProdNameUpdata=10,//品名	1火龙果，2香蕉，3西瓜
	ProdAddUpdata,//产地 1海南2泰国3浙江
	ItemNumUpdata,//货号
	PriceUpdata,//单价，单位0.01元
	MemPriceUpdata,//会员价，单位0.01元
	ProdDateUpdata,//日期格式20180912
	ChavgeUnitUpdata,//计价单位1，500g，2个，3只
} CG_BTEP_configFlagEnum;


#endif //_BOARD_H_
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
