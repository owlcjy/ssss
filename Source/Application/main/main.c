

/*--------------------------- ͷ�ļ����� --------------------------------------*/
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
void BTEPvar_init(void);//������ʼ��
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

extern INT16U rxcomd_i;//�Խ��յ�����Ч��������ַ������м���
extern INT8U RX_COMD[400];//���յ�����Ч������


extern INT16U rxJHdata_i;//�Խ��յ�����Ч�ľ������ݵ��ַ������м���
extern INT8U RX_JHdata[JHDATA_NUM];//���յ�����Ч�ľ�������

extern INT8U Send_JHdata_Size;//���ش�������Ҫ���͵ľ��ɵĴ�С



//ȫ�����ñ���
extern INT8U SX_RESET_flg;//0,������,1,��λоƬ
extern INT8U SX_CONFIG_flg;//0,������,1,����оƬ

extern u8	CG_ERRandOK_Flag;//PC���µ������Ƿ�OK
extern u8	CG_BTEP_configFlag;//���±�־


extern u8	sendBuf[64];	//���ͻ�����
extern u8	revBuf[128];	//���ջ�����






/**************************************************************
 * ������  : main()
 * ����    : ������
 *************************************************************/
 //2019��1��17��16:40:17
int main( void )
{
u16 li=0;
volatile INT8U i, j, length, error;
GPIO_InitTypeDef GPIO_InitStructure;

/* --����ϵͳ��ƵΪ72MHz */
	SystemInit();
	Delay_InitConfig( 72 );
	

	BTEPGPIOInit();//GPIOInit();	    //GPIO��ʼ��
    POW_ON;//��Դ����    
    
	TIM2_Base_Config_owl();
	Uart1Init();	//���ڳ�ʼ���������������Ϣ 9600bps 8N1

    //SPI_Initial( );
  BTEPvar_init();//var_init();//������ʼ��   
	EPLCD_Init();

   #if 0
    LCD_Init( );            //OLED��ʼ��

    LCD_Dis_Str( 0,0,"    :   -104dBm" );
    LCD_Dis_Str( 2,0,"    :433.000MHz" );
	LCD_Dis_Str( 4,0,"    :     10kHz" );
	LCD_Dis_Str( 6,0,"    :          " );
 	LCD_Dis_Logo( );
    SI446X_RESET( );        //SI446X ģ�鸴λ
    delay_ms(10);
    SI446X_CONFIG_INIT( );  //SI446X ģ���ʼ�����ú���
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
		delay_ms(10);//Delay��2ms����
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

void var_init(void)//������ʼ��
{
int i;
	CG_gTesterStatus.Freq=433000;
	CG_gTesterStatus.Power=-125;
	CG_gTesterStatus.Power_average=0;//���ʵ�ƽ��ֵ������ƽ��
	CG_gTesterStatus.Power_peak=0;//���ʵķ�ֵ
	CG_gTesterStatus.Power_realtime=0;//���ʵ�ʵʱֵ
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

void BTEPvar_init(void)//������ʼ��
{
	
		BTEP_gTesterStatus.ProdName=1;//Ʒ�� 1��������2�㽶��3����
		BTEP_gTesterStatus.ProdAdd=1;//���� 1����2̩��3�㽭
		BTEP_gTesterStatus.ItemNum=620034;//����
		BTEP_gTesterStatus.Price=600;//���ۣ���λ0.01Ԫ
		BTEP_gTesterStatus.MemPrice=569;//��Ա�ۣ���λ0.01Ԫ
		BTEP_gTesterStatus.ProdDate=20190101;//���ڸ�ʽ20180912
		BTEP_gTesterStatus.ChavgeUnit=1;//�Ƽ۵�λ1��500g��2����3ֻ
	
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
	driver_delay_xms(DELAYTIME*1.2); //��ʱʱ��ӳ���������ʾʱ��̫�̣����治ȫ

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

//���ʵĲ�������ʾ
void PoweMeasureDeal(void)
{
int i;
s32 Power_buf=0;
	CG_gTesterStatus.PoweMeasure[CG_gTesterStatus.PoMe_i]=GetRSSI();//��ȡ��ǿ
	//�ó�ʵʱ��ǿ
	CG_gTesterStatus.Power_realtime=CG_gTesterStatus.PoweMeasure[CG_gTesterStatus.PoMe_i];
	//����ƽ��ֵ
	for(i=0;i<POME_MAX;i++)
		{
		Power_buf=Power_buf+CG_gTesterStatus.PoweMeasure[i];
		}
	CG_gTesterStatus.Power_average=Power_buf/POME_MAX;
	//�����ֵ
	for(i=0;i<POME_MAX;i++)
		{
		if(Power_buf<CG_gTesterStatus.PoweMeasure[i])
			{
			Power_buf=CG_gTesterStatus.PoweMeasure[i];

			}		
		}
	CG_gTesterStatus.Power_peak=Power_buf;	
	//���ݵ�ǰģʽ����ʾ����ʾ��ֵ
	switch(CG_gTesterStatus.Mode)
		{
			case 1:
			//����
			CG_gTesterStatus.Power=CG_gTesterStatus.Power_realtime;
			break;
			case 2:
			//ƽ��
			CG_gTesterStatus.Power=CG_gTesterStatus.Power_average;
			break;
			case 3:
			//��ֵ
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
* ������   : StatusInfoToPC
* �������� :��ָ��״̬��Ϣ�ϴ���PC
*
* ����     :
* ����ֵ   :
*           ��
* �޸ļ�¼:
* 		    jycheng2018��2��26��18:51:11
***********************************************************************/

void StatusInfoToPC(u8 Flag)
{	u8 PBufOK[] = {"OK\x0d"};
	u8 PBufERROR[] = {"ERROR\x0d"};

	switch(Flag)
	{
		case 0://����ӡ
		break;
		
		case 1://��ӡOK
		uart1_senddata(PBufOK,(sizeof(PBufOK)-1));
		break;
		
		case 0xFF://��ӡERROR
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




        /*����ɨ��*/
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
* ������   : LCDDispSenFreq
* �������� :���������õ�Ƶ����ʾ����
*
* ����     :
* ����ֵ   :
*           ��
* �޸ļ�¼:
* 		    jycheng2017��9��21��09:51:49
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
	memcpy(&PBufFreq[1], buf, 3);//��Ƶ�ʵ�λֵ���Ƶ�buf��
	DatatoASC(SenFreq_L,buf);
	memcpy(&PBufFreq[5], buf, 3);//��Ƶ�ʵ�λֵ���Ƶ�buf��
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
			//����
		    LCD_Dis_16x16( 6, 3*16, &Our_Logo[32*8] );
		    LCD_Dis_16x16( 6, 4*16, &Our_Logo[32*9] );	
			LCD_Dis_16x16( 6, 5*16, &Our_Logo[32*14] );
		    LCD_Dis_16x16( 6, 6*16, &Our_Logo[32*15] );	
			break;
			case 2:
				//ƽ��
		    LCD_Dis_16x16( 6, 3*16, &Our_Logo[32*10] );
		    LCD_Dis_16x16( 6, 4*16, &Our_Logo[32*11] );	
			LCD_Dis_16x16( 6, 5*16, &Our_Logo[32*14] );
		    LCD_Dis_16x16( 6, 6*16, &Our_Logo[32*15] );	
			break;
			case 3:
				//��ֵ
		    LCD_Dis_16x16( 6, 3*16, &Our_Logo[32*12] );
		    LCD_Dis_16x16( 6, 4*16, &Our_Logo[32*13] );	
			LCD_Dis_16x16( 6, 5*16, &Our_Logo[32*14] );
		    LCD_Dis_16x16( 6, 6*16, &Our_Logo[32*15] );	
			break;
			
		}	


}




//��ʾ������Ϣ
void EPDDispAll(void)
{	
u8 i;
	u32	SenFreq_H,SenFreq_L;
	char PBufFreq[] = {0,0,0,0,0,0,0,0,0,0};
	char buf[30];

		
		EPD_Dis_u32data8x16(8,5,BTEP_gTesterStatus.ItemNum);//����
		EPD_Dis_u32data8x16(13,20,BTEP_gTesterStatus.ItemNum);//����
		
		EPD_Dis_Price8x16(10,7,BTEP_gTesterStatus.MemPrice);//��Ա��
		EPD_Dis_u32data8x16(12,5,BTEP_gTesterStatus.ProdDate);//����

	EPD_Dis_Price16x32(3,9,BTEP_gTesterStatus.Price);//����


	//EPD_Dis_u32data8x16(0,0,BTEP_gTesterStatus.ProdDate);
	//EPD_Dis_u32data8x16(4,0,1622);


}

//��ʾ������Ϣ
void EPDDispPart(void)
{
	switch(CG_BTEP_configFlag)
			{
				case ProdNameUpdata://Ʒ��	1��������2�㽶��3����
			
				break;	

				case ProdAddUpdata://���� 1����2̩��3�㽭
			
				break;
				case ItemNumUpdata://����
				//������ռ�
				EPD_Dis_Str8x16(8,5,"        ");
				EPD_Dis_u32data8x16(8,5,BTEP_gTesterStatus.ItemNum);//����
				EPD_Dis_Str8x16(13,20,"        ");
				EPD_Dis_u32data8x16(13,20,BTEP_gTesterStatus.ItemNum);//����
				break;
				case PriceUpdata://���ۣ���λ0.01Ԫ
				EPD_Dis_Str16x32(3,9,"      ");
				EPD_Dis_Price16x32(3,9,BTEP_gTesterStatus.Price);//����
				break;
				case MemPriceUpdata://��Ա�ۣ���λ0.01Ԫ
				EPD_Dis_Str8x16(10,7,"      ");
				EPD_Dis_Price8x16(10,7,BTEP_gTesterStatus.MemPrice);//��Ա��
				break;
				case ProdDateUpdata://���ڸ�ʽ20180912
				EPD_Dis_Str8x16(12,5,"        ");
				EPD_Dis_u32data8x16(12,5,BTEP_gTesterStatus.ProdDate);//����
				break;
				case ChavgeUnitUpdata://�Ƽ۵�λ1��500g��2����3ֻ
			
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
					SSB_ALL_CONFIG(1);//�˴����������������
					break;	
					case KEY_RESTE://��λ�豸����					
					var_init();
					SSB_ALL_CONFIG(1);
					break;	
					
					case KEY_PWR_OFF:
					 POW_OFF;//��Դ����(1);//�˴����������������
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
	memcpy(&PBufFreq[1], buf, 3);//��Ƶ�ʵ�λֵ���Ƶ�buf��
	DatatoASC(SenFreq_L,buf);
	memcpy(&PBufFreq[5], buf, 3);//��Ƶ�ʵ�λֵ���Ƶ�buf��
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

	//�ж�Ƶ�����ĸ���
	//���õĶ�
	//142~175
	//264~350
	//420~525
	//850~1050
	//������ķֶη���
	//0~200,200~400,400~700,700~1050
	
	flo_freq=(float)(CG_gTesterStatus.Freq);//Ƶ��ֵת��Ϊ����������λΪkHz
	
	if(CG_gTesterStatus.Freq <=200000)//ʵ�ʿ���142~175
		{
		//��Ƶ��2/24
		flo_freq=flo_freq/2166.66667;//����kHz��λ����ȥ�����1/12
		config_table[185]=0x0D;
		}
	else if(CG_gTesterStatus.Freq <=400000)//ʵ�ʿ���264~350
		{
		//��Ƶ��2/12
		flo_freq=flo_freq/4333.33333;//����kHz��λ����ȥ�����1/6
		config_table[185]=0x0B;

		}
	else if(CG_gTesterStatus.Freq <=700000)//ʵ�ʿ���420~525
		{
		//��Ƶ��2/8
		flo_freq=flo_freq/6500;//����kHz��λ����ȥ�����1/4
		config_table[185]=0x0A;
		}
	else if(CG_gTesterStatus.Freq <=1050000)//ʵ�ʿ���850~1050
		{
		//��Ƶ��2/4
		flo_freq=flo_freq/13000;//����kHz��λ����ȥ�����1/2
		config_table[185]=0x08;
		}	
	else 
		{
		return;
		}
	
	//������������
	u_fcinte=(unsigned char)(flo_freq);//��NCO����ȡ��
	u_fcinte -= 1;//��ȥ1�õ�NCO��������
	//����С������
	flo_fcfrac=flo_freq-u_fcinte;//С�����ֵĸ����ʾ
	flo_fcfrac=flo_fcfrac*524288;//�õ�20λ��
	u_fcfrac=(unsigned long)(flo_fcfrac);//ת��Ϊ������ʽ
	u_fcfrac=u_fcfrac & 0x000FFFFF;//ȷ��������Ч

	//��NCO��ֵ�ŵ�buf��
	buf[0]=u_fcinte;
	buf[1]=(unsigned char)((u_fcfrac>>16)&0x0000000F);
	buf[2]=(unsigned char)((u_fcfrac>>8)&0x000000FF);
	buf[3]=(unsigned char)((u_fcfrac)&0x000000FF);

	//���µ���ǰ������
	config_table[260]=buf[0];
	config_table[261]=buf[1];
	config_table[262]=buf[2];
	config_table[263]=buf[3];



}



void SSBDealFreqSpan(u8 ch)
{

	if(CG_gTesterStatus.Freq <=200000)//ʵ�ʿ���142~175
			{
			;
			}
		else if(CG_gTesterStatus.Freq <=400000)//ʵ�ʿ���284~350
			{	
			DealFreqSpan284_350();
			}
		else if(CG_gTesterStatus.Freq <=700000)//ʵ�ʿ���420~525
			{
			DealFreqSpan420_525();
			}
		else if(CG_gTesterStatus.Freq <=1050000)//ʵ�ʿ���850~1050
			{
			;
			}	
		else 
			{
			return;
			}


}

/***********************************************************************
* ������   : SSB_ALL_CONFIG
* �������� : ��������������������Ҷ�ģ��������ã����õ�Ƭ����λ�������˺���ִ����Ҫ2����
*
* ����     :u8 ch Ҫ���õ�SSBģ��
			
* ����ֵ   :
*           ��
* �޸ļ�¼:
*         
***********************************************************************/

void SSB_ALL_CONFIG(u8 ch)
{        	
			
			
			SSBDealFreqSpan(ch);
			SSBDealFreaqNCO(ch);//����Ƶ�ʲ���
			
			SI446X_RESET( );		//SI446X ģ�鸴λ
			   delay_ms(10);
			SI446X_CONFIG_INIT( );	//SI446X ģ���ʼ�����ú���
			   delay_ms(10);
			SI446X_START_RX( 0, 0, PACKET_LENGTH,0,0,3 );
			   delay_ms(10);

}

//��Ƶ��Ϊ420M~525M֮���Span���в�����
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
//����������������


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

//��Ƶ��Ϊ284M~350M֮���Span���в�����
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
//����������������

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

