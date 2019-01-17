

/*--------------------------- 头文件包含 --------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#include "usart1.h"
#include "gpio.h"
#include "Delay.h"
 

 
/* MAIN.C file
 *
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "board.h"
#include "si446x.h"
#include "Oled.h"
#include "matrixKey.h"
//#include "si_config.h"
#include "M433k1.h"
#include "M433k10.h"
#include "M433k100.h"
#include "M433k400.h"
#include "M470k1.h"
#include "M500k10.h"

#include "M297k10.h"
#include "M300k1.h"
#include "M333k10.h"
#include "M338k1.h"
#include "M350k1.h"
#include "M350k10.h"
#include "M350k100.h"
#include "M350k400.h"



#include "Display_EPD_W21.h"
#include "EPD_W21_font.h"





#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed long   /*   signed 32 bits. */
#define U32 unsigned long   /* unsigned 32 bits. */
#define I16P I16              /*   signed 16 bits OR MORE ! */
#define U16P U16              /* unsigned 16 bits OR MORE ! */

void hardware_scan_task(void );
void timerdeal(void);

void DatatoASC(u32 data ,char* buffer);
void DatatoASC4(u32 data ,char* buffer);

void KeyDispSenFreq(void);

void LCDDispSenFreq(void);
void LCDDispPower(void);
void LCDDispSpan(void);
void LCDDispMode(void);

void EPDDispAll(void);
void EPDDispPart(void);

void KeyDeal(u16 KeyValue);

void SSBDealFreaqNCO(u8 ch);
void SSBDealFreqSpan(u8 ch);

void SSB_ALL_CONFIG(u8 ch);
void KeyDeal(u16 KeyValue);
int DealFreqSpan420_525(void );
int DealFreqSpan284_350(void );
void var_init(void);
void BTEPvar_init(void);//变量初始化
void EPLCD_Init(void);

void PoweMeasureDeal(void);

extern INT8U config_table[];
u8 DispTimer=0;
const INT8U M433k1_config_table[] 	 	= RADIO_M433k1_CONFIGURATION_DATA_ARRAY;
const INT8U M433k10_config_table[] 	 	= RADIO_M433k10_CONFIGURATION_DATA_ARRAY;
const INT8U M433k100_config_table[] 	= RADIO_M433k100_CONFIGURATION_DATA_ARRAY;
const INT8U M433k400_config_table[] 	= RADIO_M433k400_CONFIGURATION_DATA_ARRAY;
const INT8U M470k1_config_table[] 	 	= RADIO_M470k1_CONFIGURATION_DATA_ARRAY;
const INT8U M500k10_config_table[] 	 	= RADIO_M500k10_CONFIGURATION_DATA_ARRAY;


const INT8U M297k10_config_table[] 	 	= RADIO_M297k10_CONFIGURATION_DATA_ARRAY;
const INT8U M300k1_config_table[] 	 	= RADIO_M300k1_CONFIGURATION_DATA_ARRAY;
const INT8U M333k10_config_table[] 	 	= RADIO_M333k10_CONFIGURATION_DATA_ARRAY;
const INT8U M338k1_config_table[] 	 	= RADIO_M338k1_CONFIGURATION_DATA_ARRAY;
const INT8U M350k1_config_table[] 	 	= RADIO_M350k1_CONFIGURATION_DATA_ARRAY;
const INT8U M350k10_config_table[] 	 	= RADIO_M350k10_CONFIGURATION_DATA_ARRAY;
const INT8U M350k100_config_table[] 	= RADIO_M350k100_CONFIGURATION_DATA_ARRAY;
const INT8U M350k400_config_table[] 	= RADIO_M350k400_CONFIGURATION_DATA_ARRAY;

extern const unsigned char EPD_GBFont32x32[];


ErrorStatus HSEStartUpStatus;
CG_Struct_MENU_STATUS	CG_gMenuStatus;
static CG_Struct_TESTER_STATUS CG_gTesterStatus;
CG_Struct_BTEP_STATUS BTEP_gTesterStatus;
extern u8  CheckKeyTimer;
extern const INT8U Our_Logo[];

INT8U  buffer[64] = {'0','0','0','0','0',0};






INT8U r_status=0;

extern INT16U rxcomd_i;//对接收到的有效的命令的字符数进行计数
extern INT8U RX_COMD[400];//接收到的有效的命令


extern INT16U rxJHdata_i;//对接收到的有效的净荷数据的字符数进行计数
extern INT8U RX_JHdata[JHDATA_NUM];//接收到的有效的净荷数据

extern INT8U Send_JHdata_Size;//主控传过来的要发送的净荷的大小



//全局配置变量
extern INT8U SX_RESET_flg;//0,不动作,1,复位芯片
extern INT8U SX_CONFIG_flg;//0,不动作,1,配置芯片

extern u8	CG_ERRandOK_Flag;//PC传下的命令是否OK
extern u8	CG_BTEP_configFlag;//更新标志


extern u8	sendBuf[64];	//发送缓冲区
extern u8	revBuf[128];	//接收缓冲区






/**************************************************************
 * 函数名  : main()
 * 描述    : 主函数
 *************************************************************/
 //2019年1月17日16:40:17
int main( void )
{
u16 li=0;
volatile INT8U i, j, length, error;
GPIO_InitTypeDef GPIO_InitStructure;

/* --配置系统主频为72MHz */
	SystemInit();
	Delay_InitConfig( 72 );
	

	BTEPGPIOInit();//GPIOInit();	    //GPIO初始化
    POW_ON;//电源控制    
    
	TIM2_Base_Config_owl();
	Uart1Init();	//串口初始化用于输出调试信息 9600bps 8N1

    //SPI_Initial( );
  BTEPvar_init();//var_init();//变量初始化   
	EPLCD_Init();

   #if 0
    LCD_Init( );            //OLED初始化

    LCD_Dis_Str( 0,0,"    :   -104dBm" );
    LCD_Dis_Str( 2,0,"    :433.000MHz" );
	LCD_Dis_Str( 4,0,"    :     10kHz" );
	LCD_Dis_Str( 6,0,"    :          " );
 	LCD_Dis_Logo( );
    SI446X_RESET( );        //SI446X 模块复位
    delay_ms(10);
    SI446X_CONFIG_INIT( );  //SI446X 模块初始化配置函数
    delay_ms(10);
    SI446X_START_RX( 0, 0, PACKET_LENGTH,0,0,3 );
	delay_ms(10);
	#endif

		EPD_Dis_Logo();
	EPDDispAll();
	while( 1 )
		{
		
	
		hardware_scan_task();
		//timerdeal();
		//PoweMeasureDeal();
		delay_ms(10);//Delay（2ms）；
		if(DispTimer==0)
			{
			DispTimer=300;
			//EPDDispAll();
			}
		if(CG_BTEP_configFlag!=0)
			{			
			EPDDispPart();
			CG_BTEP_configFlag=0;
			}
		
			if(CG_ERRandOK_Flag !=0)
			{
			StatusInfoToPC(CG_ERRandOK_Flag);
			CG_ERRandOK_Flag=0;
			}	
		}

  
}

void var_init(void)//变量初始化
{
int i;
	CG_gTesterStatus.Freq=433000;
	CG_gTesterStatus.Power=-125;
	CG_gTesterStatus.Power_average=0;//功率的平均值，滑动平均
	CG_gTesterStatus.Power_peak=0;//功率的峰值
	CG_gTesterStatus.Power_realtime=0;//功率的实时值
for(i=0;i<200;i++)
{
	CG_gTesterStatus.PoweMeasure[i]=0;

}
	CG_gTesterStatus.PoMe_i=0;
	CG_gTesterStatus.Key=0xFF;
	CG_gTesterStatus.Span=10;
	CG_gTesterStatus.Mode=1;
	CG_gTesterStatus.flash_OK=0;
	


}

void BTEPvar_init(void)//变量初始化
{
	
		BTEP_gTesterStatus.ProdName=1;//品名 1火龙果，2香蕉，3西瓜
		BTEP_gTesterStatus.ProdAdd=1;//产地 1海南2泰国3浙江
		BTEP_gTesterStatus.ItemNum=620034;//货号
		BTEP_gTesterStatus.Price=600;//单价，单位0.01元
		BTEP_gTesterStatus.MemPrice=569;//会员价，单位0.01元
		BTEP_gTesterStatus.ProdDate=20190101;//日期格式20180912
		BTEP_gTesterStatus.ChavgeUnit=1;//计价单位1，500g，2个，3只
	
}
void EPLCD_Init(void)
{
	u8 m;
	u8 num=2;

	EPD_init_Full();
	driver_delay_xms(200);


		m=0xff;
	EPD_Dis_Full((unsigned char *)&m,0);  //all white
		driver_delay_xms(DELAYTIME);
		
		EPD_init_Part();
		driver_delay_xms(200);
#if 0		
		for(m=0;m<9;m++) 
				EPD_Dis_Part(32*3,32*3+31,32*4,32*4+31,(unsigned char *)&EPD_GBFont32x32[m*128],1);
	


	EPD_Dis_Full((unsigned char *)first,1);  //	pic
	driver_delay_xms(DELAYTIME*1.2); //延时时间加长，否则显示时间太短，画面不全

 	EPD_Dis_Full((unsigned char *)second,1);  //	pic
	driver_delay_xms(DELAYTIME*1.2);

	EPD_Dis_Full((unsigned char *)third,1);  //	pic
	driver_delay_xms(DELAYTIME*1.2);
	
	EPD_init_Part();
	driver_delay_xms(200);

	EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)third,1); //pic

	for(m=0;m<9;m++) 
		EPD_Dis_Part(0,31,0,63,(unsigned char *)&Num[m],1);

   driver_delay_xms(DELAYTIME);

	EPD_init_Full();
	driver_delay_xms(200);
	m=0xff;
	EPD_Dis_Full((unsigned char *)&m,0);  //all white
	driver_delay_xms(DELAYTIME);
	
	EPD_init_Part();
	driver_delay_xms(200);

	while(num--)
	{	  
	  EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)first,1); //pic
	  driver_delay_xms(1000); 

	  EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)second,1); //pic
	  driver_delay_xms(1000);
		  
	  EPD_Dis_Part(0,xDot-1,0,yDot-1,(unsigned char *)third,1); //pic
	  driver_delay_xms(900);
		  
	  };
	#endif
	  //ENTER SLEEP MODE
	  #if 0
	  EPD_W21_POWEROFF(); //POWER OFF
	  EPD_W21_SLEEP(); //SLEEP MODE
	  #endif
}

//功率的测量和显示
void PoweMeasureDeal(void)
{
int i;
s32 Power_buf=0;
	CG_gTesterStatus.PoweMeasure[CG_gTesterStatus.PoMe_i]=GetRSSI();//获取场强
	//得出实时场强
	CG_gTesterStatus.Power_realtime=CG_gTesterStatus.PoweMeasure[CG_gTesterStatus.PoMe_i];
	//计算平均值
	for(i=0;i<POME_MAX;i++)
		{
		Power_buf=Power_buf+CG_gTesterStatus.PoweMeasure[i];
		}
	CG_gTesterStatus.Power_average=Power_buf/POME_MAX;
	//计算峰值
	for(i=0;i<POME_MAX;i++)
		{
		if(Power_buf<CG_gTesterStatus.PoweMeasure[i])
			{
			Power_buf=CG_gTesterStatus.PoweMeasure[i];

			}		
		}
	CG_gTesterStatus.Power_peak=Power_buf;	
	//根据当前模式，显示该显示的值
	switch(CG_gTesterStatus.Mode)
		{
			case 1:
			//正常
			CG_gTesterStatus.Power=CG_gTesterStatus.Power_realtime;
			break;
			case 2:
			//平均
			CG_gTesterStatus.Power=CG_gTesterStatus.Power_average;
			break;
			case 3:
			//峰值
			CG_gTesterStatus.Power=CG_gTesterStatus.Power_peak;
			break;			
		}	
	
	CG_gTesterStatus.PoMe_i++;
	if(CG_gTesterStatus.PoMe_i>=POME_MAX)
		{
		CG_gTesterStatus.PoMe_i=0;
		}

}

/***********************************************************************
* 函数名   : StatusInfoToPC
* 函数描述 :把指令状态信息上传给PC
*
* 参数     :
* 返回值   :
*           无
* 修改记录:
* 		    jycheng2018年2月26日18:51:11
***********************************************************************/

void StatusInfoToPC(u8 Flag)
{	u8 PBufOK[] = {"OK\x0d"};
	u8 PBufERROR[] = {"ERROR\x0d"};

	switch(Flag)
	{
		case 0://不打印
		break;
		
		case 1://打印OK
		uart1_senddata(PBufOK,(sizeof(PBufOK)-1));
		break;
		
		case 0xFF://打印ERROR
		uart1_senddata(PBufERROR,(sizeof(PBufERROR)-1));
		break;
		default:
		break;		
	}
}


void hardware_scan_task(void )
{
    u16 KeyValue;
    static u16 keyval_saved = 0xffff;




        /*按键扫描*/
        KeyValue = getComKeyVal();
        if(KeyValue < 0xff)
        {
        CG_gTesterStatus.Key=KeyValue;
		//KeyDispSenFreq();
            if(keyval_saved != KeyValue)
            {
            	
			    KeyDeal(KeyValue);

				LCDDispSenFreq();
				//LCDDispPower();
				LCDDispSpan();	
				LCDDispMode();
	
                keyval_saved = KeyValue;
            }
        }

       
}
void timerdeal(void)
{
	if((CheckKeyTimer != 0)&&(CheckKeyTimer != 0xff)) CheckKeyTimer--;
	if((DispTimer != 0)&&(DispTimer != 0xff)) DispTimer--;
	delay_ms(1);

}




/***********************************************************************
* 函数名   : LCDDispSenFreq
* 函数描述 :把正在设置的频率显示出来
*
* 参数     :
* 返回值   :
*           无
* 修改记录:
* 		    jycheng2017年9月21日09:51:49
***********************************************************************/

void LCDDispSenFreq(void)
{	
u8 i;
	u32	SenFreq_H,SenFreq_L;
	char PBufFreq[] = {":435.000"};
	char buf[5];

	SenFreq_H=CG_gTesterStatus.Freq/1000%1000;
	SenFreq_L=CG_gTesterStatus.Freq%1000;

	DatatoASC(SenFreq_H,buf);
	for(i=0;i<3;i++)
		{
		if(buf[i]!='0')
			{
				break;
			}
		else
			{
			buf[i]=' ';
			}
		}
	if(i==3)
		{
		PBufFreq[4]=' ';

		}
	memcpy(&PBufFreq[1], buf, 3);//将频率低位值复制到buf中
	DatatoASC(SenFreq_L,buf);
	memcpy(&PBufFreq[5], buf, 3);//将频率低位值复制到buf中
	LCD_Dis_Str( 2,2*16,PBufFreq );
	
}

void LCDDispPower(void)
{	
u8 i;
	u32	Power_H;
	char PBufPower[] = {":       "};
	char buf[5];
if(CG_gTesterStatus.Power>=0)
{
	Power_H=CG_gTesterStatus.Power%1000;
}
else
{
	Power_H=(0-CG_gTesterStatus.Power)%1000;
}
		

	DatatoASC(Power_H,buf);
	for(i=0;i<3;i++)
		{
		if(buf[i]!='0')
			{
				break;
			}
		else
			{

				buf[i]=' ';
			}
		}

	if(CG_gTesterStatus.Power>=0)
	{
	PBufPower[4+i]=' ';			
	}
	else
	{
	PBufPower[4+i]='-';	
	}
		if(i==3)
		{
		PBufPower[7]='0'; 
		}
	memcpy(&PBufPower[5+i], &buf[i], 3-i);
	LCD_Dis_Str( 0,2*16,PBufPower );

	
}

void LCDDispSpan(void)
{	
u8 i;
	u32	Span;
	char PBufSpan[] = {":   1000"};
	char buf[5];

	Span=CG_gTesterStatus.Span%10000;
	
	DatatoASC4(Span,buf);
	for(i=0;i<4;i++)
		{
		if(buf[i]!='0')
			{
				break;
			}
		else
			{
			buf[i]=' ';
			}
		}

	memcpy(&PBufSpan[4], buf, 4);
	LCD_Dis_Str( 4,2*16,PBufSpan );
	
}


void LCDDispMode(void)
{	
	switch(CG_gTesterStatus.Mode)
		{
			case 1:
			//正常
		    LCD_Dis_16x16( 6, 3*16, &Our_Logo[32*8] );
		    LCD_Dis_16x16( 6, 4*16, &Our_Logo[32*9] );	
			LCD_Dis_16x16( 6, 5*16, &Our_Logo[32*14] );
		    LCD_Dis_16x16( 6, 6*16, &Our_Logo[32*15] );	
			break;
			case 2:
				//平均
		    LCD_Dis_16x16( 6, 3*16, &Our_Logo[32*10] );
		    LCD_Dis_16x16( 6, 4*16, &Our_Logo[32*11] );	
			LCD_Dis_16x16( 6, 5*16, &Our_Logo[32*14] );
		    LCD_Dis_16x16( 6, 6*16, &Our_Logo[32*15] );	
			break;
			case 3:
				//峰值
		    LCD_Dis_16x16( 6, 3*16, &Our_Logo[32*12] );
		    LCD_Dis_16x16( 6, 4*16, &Our_Logo[32*13] );	
			LCD_Dis_16x16( 6, 5*16, &Our_Logo[32*14] );
		    LCD_Dis_16x16( 6, 6*16, &Our_Logo[32*15] );	
			break;
			
		}	


}




//显示所有信息
void EPDDispAll(void)
{	
u8 i;
	u32	SenFreq_H,SenFreq_L;
	char PBufFreq[] = {0,0,0,0,0,0,0,0,0,0};
	char buf[30];

		
		EPD_Dis_u32data8x16(8,5,BTEP_gTesterStatus.ItemNum);//货号
		EPD_Dis_u32data8x16(13,20,BTEP_gTesterStatus.ItemNum);//货号
		
		EPD_Dis_Price8x16(10,7,BTEP_gTesterStatus.MemPrice);//会员价
		EPD_Dis_u32data8x16(12,5,BTEP_gTesterStatus.ProdDate);//日期

	EPD_Dis_Price16x32(3,9,BTEP_gTesterStatus.Price);//单价


	//EPD_Dis_u32data8x16(0,0,BTEP_gTesterStatus.ProdDate);
	//EPD_Dis_u32data8x16(4,0,1622);


}

//显示部分信息
void EPDDispPart(void)
{
	switch(CG_BTEP_configFlag)
			{
				case ProdNameUpdata://品名	1火龙果，2香蕉，3西瓜
			
				break;	

				case ProdAddUpdata://产地 1海南2泰国3浙江
			
				break;
				case ItemNumUpdata://货号
				//先清除空间
				EPD_Dis_Str8x16(8,5,"        ");
				EPD_Dis_u32data8x16(8,5,BTEP_gTesterStatus.ItemNum);//货号
				EPD_Dis_Str8x16(13,20,"        ");
				EPD_Dis_u32data8x16(13,20,BTEP_gTesterStatus.ItemNum);//货号
				break;
				case PriceUpdata://单价，单位0.01元
				EPD_Dis_Str16x32(3,9,"      ");
				EPD_Dis_Price16x32(3,9,BTEP_gTesterStatus.Price);//单价
				break;
				case MemPriceUpdata://会员价，单位0.01元
				EPD_Dis_Str8x16(10,7,"      ");
				EPD_Dis_Price8x16(10,7,BTEP_gTesterStatus.MemPrice);//会员价
				break;
				case ProdDateUpdata://日期格式20180912
				EPD_Dis_Str8x16(12,5,"        ");
				EPD_Dis_u32data8x16(12,5,BTEP_gTesterStatus.ProdDate);//日期
				break;
				case ChavgeUnitUpdata://计价单位1，500g，2个，3只
			
				break;				
			}	

}


void KeyDeal(u16 KeyValue)
{	


	switch(KeyValue)
            		{	

					case KEY_FREQ_UP:
					CG_gTesterStatus.Freq+=CG_gTesterStatus.Span;
					if(CG_gTesterStatus.Freq>500000)
						{
						CG_gTesterStatus.Freq=300000;
						}
					break;
					case KEY_FREQ_DOWN:
					CG_gTesterStatus.Freq-=CG_gTesterStatus.Span;
					if(CG_gTesterStatus.Freq<300000)
						{
						CG_gTesterStatus.Freq=500000;
						}
					break;
					case KEY_SPAN_UP:
					CG_gTesterStatus.Span=CG_gTesterStatus.Span*10;
					if(CG_gTesterStatus.Span>1000)
						{
						CG_gTesterStatus.Span=1;
						}
					break;
					case KEY_SPAN_DOWN:
					CG_gTesterStatus.Span=CG_gTesterStatus.Span/10;
					if(CG_gTesterStatus.Span<1)
						{
						CG_gTesterStatus.Span=1000;
						}
					break;					
					case KEY_MODE_UP:
					CG_gTesterStatus.Mode+=1;
					if(CG_gTesterStatus.Mode>3)
						{
						CG_gTesterStatus.Mode=1;
						}
					break;	

					case KEY_MODE_DOWN:
					CG_gTesterStatus.Mode-=1;
					if(CG_gTesterStatus.Mode<1)
						{
						CG_gTesterStatus.Mode=3;
						}
					break;	

					case KEY_ENTER:
					SSB_ALL_CONFIG(1);//此处用来保存设置情况
					break;	
					case KEY_RESTE://复位设备参数					
					var_init();
					SSB_ALL_CONFIG(1);
					break;	
					
					case KEY_PWR_OFF:
					 POW_OFF;//电源控制(1);//此处用来保存设置情况
					break;	

					

            		}
	

}
void KeyDispSenFreq(void)
{	
u8 i;
	u32	SenFreq_H,SenFreq_L;
	char PBufFreq[] = {":435.000"};
	char buf[5];

	SenFreq_H=CG_gTesterStatus.Key/1000%1000;
	SenFreq_L=CG_gTesterStatus.Key%1000;

	DatatoASC(SenFreq_H,buf);
	for(i=0;i<3;i++)
		{
		if(buf[i]!='0')
			{
				break;
			}
		else
			{
			buf[i]=' ';
			}
		}
	if(i==3)
		{
		PBufFreq[4]=' ';

		}
	memcpy(&PBufFreq[1], buf, 3);//将频率低位值复制到buf中
	DatatoASC(SenFreq_L,buf);
	memcpy(&PBufFreq[5], buf, 3);//将频率低位值复制到buf中
	LCD_Dis_Str( 6,2*16,PBufFreq );

	
}

void DatatoASC(u32 data ,char* buffer)
{

buffer[0]=data/100%10+'0';	
buffer[1]=data/10%10+'0';	
buffer[2]=data/1%10+'0';	


}
void DatatoASC4(u32 data ,char* buffer)
{

buffer[0]=data/1000%10+'0';	
buffer[1]=data/100%10+'0';	
buffer[2]=data/10%10+'0';
buffer[3]=data/1%10+'0';



}

void SSBDealFreaqNCO(u8 ch)
{


	float flo_freq,flo_fcfrac;	
	u8  u_fcinte;
	u32 u_fcfrac;
	u8 buf[4];

	//判断频率在哪个段
	//可用的段
	//142~175
	//264~350
	//420~525
	//850~1050
	//程序里的分段方案
	//0~200,200~400,400~700,700~1050
	
	flo_freq=(float)(CG_gTesterStatus.Freq);//频率值转化为浮点数，单位为kHz
	
	if(CG_gTesterStatus.Freq <=200000)//实际可用142~175
		{
		//分频比2/24
		flo_freq=flo_freq/2166.66667;//按照kHz单位，除去晶振的1/12
		config_table[185]=0x0D;
		}
	else if(CG_gTesterStatus.Freq <=400000)//实际可用264~350
		{
		//分频比2/12
		flo_freq=flo_freq/4333.33333;//按照kHz单位，除去晶振的1/6
		config_table[185]=0x0B;

		}
	else if(CG_gTesterStatus.Freq <=700000)//实际可用420~525
		{
		//分频比2/8
		flo_freq=flo_freq/6500;//按照kHz单位，除去晶振的1/4
		config_table[185]=0x0A;
		}
	else if(CG_gTesterStatus.Freq <=1050000)//实际可用850~1050
		{
		//分频比2/4
		flo_freq=flo_freq/13000;//按照kHz单位，除去晶振的1/2
		config_table[185]=0x08;
		}	
	else 
		{
		return;
		}
	
	//计算整数部分
	u_fcinte=(unsigned char)(flo_freq);//对NCO进行取整
	u_fcinte -= 1;//减去1得到NCO整数部分
	//计算小数部分
	flo_fcfrac=flo_freq-u_fcinte;//小数部分的浮点表示
	flo_fcfrac=flo_fcfrac*524288;//得到20位数
	u_fcfrac=(unsigned long)(flo_fcfrac);//转换为整数格式
	u_fcfrac=u_fcfrac & 0x000FFFFF;//确保数据有效

	//将NCO的值放到buf中
	buf[0]=u_fcinte;
	buf[1]=(unsigned char)((u_fcfrac>>16)&0x0000000F);
	buf[2]=(unsigned char)((u_fcfrac>>8)&0x000000FF);
	buf[3]=(unsigned char)((u_fcfrac)&0x000000FF);

	//更新到当前数据中
	config_table[260]=buf[0];
	config_table[261]=buf[1];
	config_table[262]=buf[2];
	config_table[263]=buf[3];



}



void SSBDealFreqSpan(u8 ch)
{

	if(CG_gTesterStatus.Freq <=200000)//实际可用142~175
			{
			;
			}
		else if(CG_gTesterStatus.Freq <=400000)//实际可用284~350
			{	
			DealFreqSpan284_350();
			}
		else if(CG_gTesterStatus.Freq <=700000)//实际可用420~525
			{
			DealFreqSpan420_525();
			}
		else if(CG_gTesterStatus.Freq <=1050000)//实际可用850~1050
			{
			;
			}	
		else 
			{
			return;
			}


}

/***********************************************************************
* 函数名   : SSB_ALL_CONFIG
* 函数描述 : 计算所有所需参数，并且对模块进行配置，采用单片机复位操作，此函数执行需要2秒钟
*
* 参数     :u8 ch 要设置的SSB模块
			
* 返回值   :
*           无
* 修改记录:
*         
***********************************************************************/

void SSB_ALL_CONFIG(u8 ch)
{        	
			
			
			SSBDealFreqSpan(ch);
			SSBDealFreaqNCO(ch);//计算频率参数
			
			SI446X_RESET( );		//SI446X 模块复位
			   delay_ms(10);
			SI446X_CONFIG_INIT( );	//SI446X 模块初始化配置函数
			   delay_ms(10);
			SI446X_START_RX( 0, 0, PACKET_LENGTH,0,0,3 );
			   delay_ms(10);

}

//对频率为420M~525M之间的Span进行查表计算
int DealFreqSpan420_525(void )
{
if(CG_gTesterStatus.Freq <=400000)
{
	return -1;
}
if(CG_gTesterStatus.Freq >700000)
{
	return -1;
}
//后面是正常处理部分


switch(CG_gTesterStatus.Span)
	{
	case 1:
		if(CG_gTesterStatus.Freq <=468000)
			{
			memcpy(config_table, M433k1_config_table, sizeof(M433k1_config_table)); 
			}
		else
			{
			memcpy(config_table, M470k1_config_table, sizeof(M470k1_config_table)); 
			}
		break;
	
	case 10:
		if(CG_gTesterStatus.Freq <=500000)
			{
			memcpy(config_table, M433k10_config_table, sizeof(M433k10_config_table)); 
			}
		else
			{
			memcpy(config_table, M500k10_config_table, sizeof(M500k10_config_table)); 
			}
		break;

					
	case 100:
		memcpy(config_table, M433k100_config_table, sizeof(M433k100_config_table)); 
	break;	
	case 1000:
		memcpy(config_table, M433k400_config_table, sizeof(M433k400_config_table)); 
	break;	
	}
return 0;


}

//对频率为284M~350M之间的Span进行查表计算
int DealFreqSpan284_350(void )
{
if(CG_gTesterStatus.Freq <=400000)
{
	return -1;
}
if(CG_gTesterStatus.Freq >700000)
{
	return -1;
}
//后面是正常处理部分

switch(CG_gTesterStatus.Span)
	{
	case 1:
		if(CG_gTesterStatus.Freq <=300000)
			{
			memcpy(config_table, M300k1_config_table, sizeof(M300k1_config_table)); 
			}
		else if(CG_gTesterStatus.Freq <=338000)
			{
			memcpy(config_table, M338k1_config_table, sizeof(M338k1_config_table)); 
			}
		else
			{
			memcpy(config_table, M350k1_config_table, sizeof(M350k1_config_table)); 
			}
		break;
	
	case 10:
		if(CG_gTesterStatus.Freq <=297000)
			{
			memcpy(config_table, M297k10_config_table, sizeof(M297k10_config_table)); 
			}
		else if(CG_gTesterStatus.Freq <=331000)
			{
			memcpy(config_table, M333k10_config_table, sizeof(M333k10_config_table)); 
			}
		else
			{
			memcpy(config_table, M350k10_config_table, sizeof(M350k10_config_table)); 
			}
		break;
					
	case 100:
		memcpy(config_table, M350k100_config_table, sizeof(M350k100_config_table)); 
	break;	
	case 1000:
		memcpy(config_table, M350k400_config_table, sizeof(M350k400_config_table)); 
	break;	
	}
return 0;


}

/* end of main.c -------------------------------------------------------------*/

