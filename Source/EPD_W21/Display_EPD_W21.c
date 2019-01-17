#include "Display_EPD_W21_Aux.h"
#include "Display_EPD_W21_Config.h"
#include "font_EPD.h"
#include "stm32f10x.h"
#include "Display_EPD_W21.h"


const unsigned char LUTDefault_full[]={
0x32,	// command

0x22,
0x55,
0xAA,
0x55,
0xAA,
0x55,
0xAA,
0x11,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x1E,
0x1E,
0x1E,
0x1E,
0x1E,
0x1E,
0x1E,
0x1E,
0x01,
0x00,
0x00,
0x00,
0x00,


};

const unsigned char LUTDefault_part[]={
0x32,	// command

0x18,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x0F,
0x01,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
};


static unsigned char ReadBusy(void)
{
  unsigned long i=0;

  
  for(i=0;i<400;i++)
  {
      if(isEPD_W21_BUSY==EPD_W21_BUSY_LEVEL) {
      return 1;
      }
	  driver_delay_xms(10);
  }
  return 0;
}

static void EPD_W21_WriteCMD(unsigned char command)
{
    EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}
//static void EPD_W21_WriteDATA(unsigned char command)
//{
//    EPD_W21_CS_0;                   
//	EPD_W21_DC_1;		// command write
//	SPI_Write(command);
//	EPD_W21_CS_1;
//}
	
static void EPD_W21_WriteCMD_p1(unsigned char command,unsigned char para)
{
	//while(isEPD_W21_BUSY == 1);	// wait
	ReadBusy();	

    EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_DC_1;		// command write
	SPI_Write(para);
	EPD_W21_CS_1;
}

//static void EPD_W21_WriteCMD_p2(unsigned char command,unsigned char para1,unsigned char para2)
//{
//	while(isEPD_W21_BUSY == 1);	// wait	
//
//    EPD_W21_CS_0;                   
//	EPD_W21_DC_0;		// command write
//	SPI_Write(command);
//	EPD_W21_DC_1;		// command write
//	SPI_Write(para1);
//	SPI_Write(para2);
//	EPD_W21_CS_1;
//} 
/*********************************************
功能：第一个字节写入的是命令值，剩余的是参数，
在配置阶段使用

*********************************************/
static void EPD_W21_Write(unsigned char *value, unsigned char datalen)
{
	unsigned char i = 0;
	unsigned char *ptemp;
	
	ptemp = value;
	

    EPD_W21_CS_0;                   	
	EPD_W21_DC_0;		// command write
	
	SPI_Write(*ptemp);
	ptemp++;

	EPD_W21_DC_1;		// data write
	
	for(i= 0;i<datalen-1;i++)	// sub the command
	{
		SPI_Write(*ptemp);
		ptemp++;
	}

	EPD_W21_CS_1;

}
/*********************************************
功能：写显示buffer
参数： 	XSize  x方向数量  128点方向，调整为8的整数倍
		YSize  y方向数量	 	   	
		Dispbuff 显示数据保存位置。 要求数据的排列方式必须正确
*********************************************/
static void EPD_W21_WriteDispRam(unsigned char XSize,unsigned int YSize,
							unsigned char *Dispbuff)
{
	int i = 0,j = 0;

	if(XSize%8 != 0)
	{
		XSize = XSize+(8-XSize%8);
	}
	XSize = XSize/8;

	//while(isEPD_W21_BUSY == 1);	//wait
	ReadBusy();		
	
    EPD_W21_CS_0;                   
	EPD_W21_DC_0;		//command write
	SPI_Write(0x24);
	
	EPD_W21_DC_1;		//data write
	for(i=0;i<YSize;i++)
	{
		for(j=0;j<XSize;j++)
		{
			SPI_Write(*Dispbuff);
			Dispbuff++;
		}
	}
	
	EPD_W21_CS_1;
}
/*********************************************
功能：  写显示buffer 用来将某一个区域写入相同的显示内容。
参数： 	XSize  x方向数量  128点方向，调整为8的整数倍
		YSize  y方向数量	 	   	
		dispdata 显示数据。
*********************************************/
static void EPD_W21_WriteDispRamMono(unsigned char XSize,unsigned int YSize,
							unsigned char dispdata)
{
	int i = 0,j = 0;

	if(XSize%8 != 0)
	{
		XSize = XSize+(8-XSize%8);
	}
	XSize = XSize/8;
	//while(isEPD_W21_BUSY == 1);	// wait	
	ReadBusy();	

    EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(0x24);
	
	EPD_W21_DC_1;		// data write
	for(i=0;i<YSize;i++)
	{
		for(j=0;j<XSize;j++)
		{
		 SPI_Write(dispdata);
		}
	}
	
	EPD_W21_CS_1;
}

static void EPD_W21_POWERON(void)
{
	EPD_W21_WriteCMD_p1(0x22,0xc0);
	EPD_W21_WriteCMD(0x20);
	//EPD_W21_WriteCMD(0xff);
}
void EPD_W21_POWEROFF(void) //POWER OFF
{  	EPD_W21_WriteCMD_p1(0x22,0xc3);
	EPD_W21_WriteCMD(0x20);
//	EPD_W21_WriteCMD(0xff);
}
void EPD_W21_SLEEP(void) //SLEEP MODE
{
		EPD_W21_WriteCMD_p1(0x10,0x01);
}

static void EPD_W21_SetRamArea(unsigned char Xstart,unsigned char Xend,
						unsigned char Ystart,unsigned char Ystart1,unsigned char Yend,unsigned char Yend1)
{
    unsigned char RamAreaX[3];	// X start and end
	unsigned char RamAreaY[5]; 	// Y start and end
	RamAreaX[0] = 0x44;	// command
	RamAreaX[1] = Xstart;
	RamAreaX[2] = Xend;
	RamAreaY[0] = 0x45;	// command
	RamAreaY[1] = Ystart;
	RamAreaY[2] = Ystart1;
	RamAreaY[3] = Yend;
    RamAreaY[4] = Yend1;
	EPD_W21_Write(RamAreaX, sizeof(RamAreaX));
	EPD_W21_Write(RamAreaY, sizeof(RamAreaY));
}
static void EPD_W21_SetRamPointer(unsigned char addrX,unsigned char addrY,unsigned char addrY1)
{
    unsigned char RamPointerX[2];	// default (0,0)
	unsigned char RamPointerY[3]; 	
	RamPointerX[0] = 0x4e;
	RamPointerX[1] = addrX;
	RamPointerY[0] = 0x4f;
	RamPointerY[1] = addrY;
	RamPointerY[2] = addrY1;
	
	EPD_W21_Write(RamPointerX, sizeof(RamPointerX));
	EPD_W21_Write(RamPointerY, sizeof(RamPointerY));
}
static void part_display(unsigned char RAM_XST,unsigned char RAM_XEND,unsigned char RAM_YST,unsigned char RAM_YST1,unsigned char RAM_YEND,unsigned char RAM_YEND1)
{    
	EPD_W21_SetRamArea(RAM_XST,RAM_XEND,RAM_YST,RAM_YST1,RAM_YEND,RAM_YEND1);  	/*set w h*/
    EPD_W21_SetRamPointer (RAM_XST,RAM_YST,RAM_YST1);		    /*set orginal*/
}
//static void EPD_W21_UpdataDisplay(unsigned char *imgbuff,unsigned char xram,unsigned int yram)
//{
//	EPD_W21_WriteDispRam(xram, yram, imgbuff);
////	EPD_W21_Update();
//}

//=========================functions============================

//-------------------------------------------------------
//Func	: void EPD_W21_DispInit(void)
//Desc	: display parameters initinal
//Input	: none
//Output: none
//Return: none
//Author: 
//Date	: 2011/12/24
//-------------------------------------------------------
static void EPD_W21_DispInit(void)
{

//	EPD_W21_Write(softstart, sizeof(softstart));	// X decrease, Y decrease
//
//	EPD_W21_Write(GDOControl, sizeof(GDOControl));	// Pannel configuration, Gate selection
//
//	EPD_W21_Write(Rambypass, sizeof(Rambypass));	// Rambypass
//
//	EPD_W21_Write(MAsequency, sizeof(MAsequency));	// MAsequency
//
//	EPD_W21_Write(GDVol, sizeof(GDVol));		    // GDVol setting
//
//	EPD_W21_Write(SDVol, sizeof(SDVol));		    // SDVol setting
//
//	EPD_W21_Write(VCOMVol, sizeof(VCOMVol));		// VCOM setting
//
//	EPD_W21_Write(DummyLine, sizeof(DummyLine));	// dummy line per gate
//	
//	EPD_W21_Write(Gatetime, sizeof(Gatetime));		// Gage time setting
//	
//	EPD_W21_Write(RamDataEntryMode, sizeof(RamDataEntryMode));	// X increase, Y decrease


	EPD_W21_Write(GDOControl, sizeof(GDOControl));	// Pannel configuration, Gate selection	
  EPD_W21_Write(softstart, sizeof(softstart));	// X decrease, Y decrease
	

	//EPD_W21_Write(Rambypass, sizeof(Rambypass));	// RAM bypass setting
  //EPD_W21_Write(MAsequency, sizeof(MAsequency));	// clock enable
	//EPD_W21_Write(GDVol, sizeof(GDVol));			// Gate voltage setting
	//EPD_W21_Write(SDVol, sizeof(SDVol));			// Source voltage setting
	EPD_W21_Write(VCOMVol, sizeof(VCOMVol));		// VCOM setting

	//EPD_W21_Write(BOOSTERFB, sizeof(BOOSTERFB));	// Hi-V feedback selection
	EPD_W21_Write(DummyLine, sizeof(DummyLine));	// dummy line per gate
	
	EPD_W21_Write(Gatetime, sizeof(Gatetime));		// Gage time setting

	EPD_W21_Write(RamDataEntryMode, sizeof(RamDataEntryMode));	// X increase, Y decrease
	
	EPD_W21_SetRamArea(0x00,(xDot-1)/8,(yDot-1)%256,(yDot-1)/256,0x00,0x00);	// X-source area,Y-gage area
  EPD_W21_SetRamPointer(0x00,(yDot-1)%256,(yDot-1)/256);	// set ram

}
static void EPD_W21_Init(void)
{
	EPD_W21_BS_0;		// 4 wire spi mode selected

	EPD_W21_RST_0;		// Module reset
	driver_delay_xms(100);
	EPD_W21_RST_1;
	driver_delay_xms(100); 

	EPD_W21_DispInit();		// pannel configure
}
//-------------------------------------------------------
//Func	: EPD_W21_EnableChargepump
//Desc	: 
//Input	:
//Output: 
//Return: 
//Author: 
//Date	: 2011/12/24
//-------------------------------------------------------
//static void EPD_W21_EnableChargepump(void)
//{
//	EPD_W21_WriteCMD_p1(0xf0,0x8f);
//	EPD_W21_WriteCMD_p1(0x22,0xc0);
//	EPD_W21_WriteCMD(0x20);
//	EPD_W21_WriteCMD(0xff);
//}

//-------------------------------------------------------
//Func	: EPD_W21_DisableChargepump
//Desc	: 
//Input	:
//Output: 
//Return: 
//Author: 
//Date	: 2011/12/24
//-------------------------------------------------------
//static void EPD_W21_DisableChargepump(void)
//{
//	EPD_W21_WriteCMD_p1(0x22,0xf0);
//	EPD_W21_WriteCMD(0x20);
//	EPD_W21_WriteCMD(0xff);
//}
//-------------------------------------------------------
//Func	: EPD_W21_Update
//Desc	: 
//Input	:
//Output: 
//Return: 
//Author: 
//Date	: 2011/12/24
//-------------------------------------------------------
static void EPD_W21_Update(void)
{
	EPD_W21_WriteCMD_p1(0x22,0xc4);
	EPD_W21_WriteCMD(0x20);
	EPD_W21_WriteCMD(0xff);
}

static void EPD_W21_Update_Part(void)
{
	EPD_W21_WriteCMD_p1(0x22,0x04);
	//EPD_W21_WriteCMD_p1(0x22,0x08);
	EPD_W21_WriteCMD(0x20);
	EPD_W21_WriteCMD(0xff);
}
//-------------------------------------------------------
//Func	: EPD_W21_WirteLUT(unsigned char *LUTvalue)
//Desc	: write the waveform to the dirver's ram 
//Input	: *LUTvalue, the wavefrom tabe address
//Output: none
//Return: none
//Author: 
//Date	: 2011/12/24
//-------------------------------------------------------
static void EPD_W21_WirteLUT(unsigned char *LUTvalue,unsigned char Size)
{	
	EPD_W21_Write(LUTvalue, Size);
}


void EPD_init_Full(void)
{		
        //driver_delay_xms(4900);
        //driver_delay_xms(100000);
	EPD_W21_Init();			// display
    EPD_W21_WirteLUT((unsigned char *)LUTDefault_full,sizeof(LUTDefault_full));
		
	EPD_W21_POWERON();
        //driver_delay_xms(100000); 		
}

void EPD_init_Part(void)
{		
	EPD_W21_Init();			// display
	
	EPD_W21_WirteLUT((unsigned char *)LUTDefault_part,sizeof(LUTDefault_part));
	EPD_W21_POWERON();
//	//清除底色
//	//EPD_W21_SetRamArea(0x00,0x0f,0x27,0x01,0x00,0x00);	  
//	//EPD_W21_SetRamPointer(0x00,0x27,0x01);	// set ram
//	part_display(0,xDot/8,yDot%256,yDot/256,0,0);
//	EPD_W21_WriteDispRamMono(xDot, yDot, 0xff);	// white
//	EPD_W21_Update_Part();
//	driver_delay_xms(500);
//	
//	
//	part_display(0,xDot/8,yDot%256,yDot/256,0,0);
//	EPD_W21_WriteDispRamMono(xDot, yDot, 0xff);	// white	
	
	//driver_delay_xms(2500);
        		
}
/**********************************************************
参数： Label  
       =1 显示 DisBuffer中的内容
	   =0 显示 DisBuffer中第一个字节的内容
***********************************************************/

void EPD_Dis_Full(unsigned char *DisBuffer,unsigned char Label)
{
    EPD_W21_SetRamPointer(0x00,(yDot-1)%256,(yDot-1)/256);	// set ram
	if(Label==0)
	{
		EPD_W21_WriteDispRamMono(xDot, yDot, 0xff);	// white	
	}
	else
	{
		EPD_W21_WriteDispRam(xDot, yDot, (unsigned char *)DisBuffer);	// white
	}	
	EPD_W21_Update();	
	
}

/**********************************************************
参数： 
		xStart
			x方向开始坐标
		xEnd
			x方向结束坐标
		yStart
			y方向开始坐标
		yEnd
			y方向结束坐标
		DisBuffer
			显示buffer
		Label  
       		=1 显示 DisBuffer中的内容
	   		=0 显示 DisBuffer中第一个字节的内容
	   
***********************************************************/

void EPD_Dis_Part(unsigned char xStart,unsigned char xEnd,unsigned long yStart,unsigned long yEnd,unsigned char *DisBuffer,unsigned char Label)
{
	if(Label==0)
	{
		part_display(xStart/8,xEnd/8,yEnd%256,yEnd/256,yStart%256,yStart/256);
		EPD_W21_WriteDispRamMono(xEnd-xStart, yEnd-yStart+1, DisBuffer[0]);	// black
 		EPD_W21_Update_Part();
		driver_delay_xms(200);
		part_display(xStart/8,xEnd/8,yEnd%256,yEnd/256,yStart%256,yStart/256);	
		EPD_W21_WriteDispRamMono(xEnd-xStart, yEnd-yStart+1, DisBuffer[0]);	// black
		//driver_delay_xms(200);	
	}
	else
	if(Label==1)
	{
		part_display(xStart/8,xEnd/8,yEnd%256,yEnd/256,yStart%256,yStart/256);	// set ram	
		EPD_W21_WriteDispRam(xEnd-xStart, yEnd-yStart+1,DisBuffer);
		EPD_W21_Update_Part();
		driver_delay_xms(300);
		part_display(xStart/8,xEnd/8,yEnd%256,yEnd/256,yStart%256,yStart/256);
		EPD_W21_WriteDispRam(xEnd-xStart, yEnd-yStart+1,DisBuffer);
		//driver_delay_xms(300);
	}
	else
	{
		part_display(xStart/8,xEnd/8,yEnd%256,yEnd/256,yStart%256,yStart/256);	// set ram	
		EPD_W21_WriteDispRam(xEnd-xStart, yEnd-yStart+1,DisBuffer);
		EPD_W21_Update_Part();
		driver_delay_xms(200);
		part_display(xStart/8,xEnd/8,yEnd%256,yEnd/256,yStart%256,yStart/256);
		EPD_W21_WriteDispRam(xEnd-xStart, yEnd-yStart+1,DisBuffer);
		//driver_delay_xms(300);
	}
}


void EPD_Dis_8x16( INT8U page, INT8U column, INT8U *p_data )
{
	EPD_Dis_Part(page*8,page*8+15,250-8-column*8,250-8-column*8+7,(unsigned char *)p_data,1);

}
void EPD_Dis_16x16( INT8U page, INT8U column,INT8U *p_data )
{
	EPD_Dis_Part(page*8,page*8+15,250-16-column*16,250-16-column*16+15,(unsigned char *)p_data,1);

}

void EPD_Dis_16x32( INT8U page, INT8U column, INT8U *p_data )
{
	EPD_Dis_Part(page*8,page*8+31,250-16-column*16,250-16-column*16+15,(unsigned char *)p_data,1);

}
void EPD_Dis_32x32( INT8U page, INT8U column, INT8U *p_data )
{
	EPD_Dis_Part(page*8,page*8+31,250-32-column*32,250-32-column*32+31,(unsigned char *)p_data,1);

}

void EPD_Dis_Str8x16( INT8U page, INT8U column, char *str )
{
    while( *str )
    {
		EPD_Dis_8x16(page,column,(unsigned char *)&EPD_Font8x16[*str*16]);
        column++;
		str++;
    }
}
void EPD_Dis_Str16x32( INT8U page, INT8U column, char *str )
{
    while( *str )
    {
		EPD_Dis_16x32(page,column,(unsigned char *)&EPD_Font16x32[*str*64]);
        column++;
		str++;
    }
}
//buf 大小30，buf[29]存放有效位数，返回值也是有效位数 
u8 DatatoASC_n(u32 data ,char* buffer)
{
	u8 i;
	for(i=0;i<10;i++)
	{
		buffer[i]=0;
	}
	buffer[29]=0;

	buffer[0]=data/1000000000%10+'0';	
	buffer[1]=data/100000000%10+'0';	
	buffer[2]=data/10000000%10+'0';	
	buffer[3]=data/1000000%10+'0';	
	buffer[4]=data/100000%10+'0';	
	buffer[5]=data/10000%10+'0';	
	buffer[6]=data/1000%10+'0';	
	buffer[7]=data/100%10+'0';	
	buffer[8]=data/10%10+'0';	
	buffer[9]=data/1%10+'0';


	for(i=0;i<10;i++)
			{
			if(buffer[i]!='0')
				{
					break;
				}
			else
				{
				buffer[i]=0;
				}
			}
	buffer[29]=10-i;

	return (10-i);
}

//显示价格
void EPD_Dis_Price8x16( INT8U page, INT8U column, u32 data )
{
	u8 i;
		char PBufFreq[] = {0,0,0,0,0,0,0,0,0,0};
		char buf[30];

	EPD_Dis_u32data8x16(page,column,data/100);
	EPD_Dis_Str8x16(page,column+1,".");

		
		i= DatatoASC_n(data ,buf);
		i=2;
		memcpy(PBufFreq, &buf[10-i], i);//将频率低位值复制到buf中
		EPD_Dis_Str8x16(page,column+2,PBufFreq);

}
//显示价格
void EPD_Dis_Price16x32( INT8U page, INT8U column, u32 data )
{
	u8 i;
		char PBufFreq[] = {0,0,0,0,0,0,0,0,0,0};
		char buf[30];

	EPD_Dis_u32data16x32(page,column,data/100);
	EPD_Dis_Str16x32(page,column+1,".");

		
		i= DatatoASC_n(data ,buf);
		i=2;
		memcpy(PBufFreq, &buf[10-i], i);//将频率低位值复制到buf中
		EPD_Dis_Str16x32(page,column+2,PBufFreq);


}


//显示一个32位数
void EPD_Dis_u32data8x16( INT8U page, INT8U column, u32 data )
{
	u8 i;
		char PBufFreq[] = {0,0,0,0,0,0,0,0,0,0};
		char buf[30];
		
		i= DatatoASC_n(data ,buf);
		memcpy(PBufFreq, &buf[10-i], i);//将频率低位值复制到buf中
		EPD_Dis_Str8x16(page,column,PBufFreq);
}
//显示一个32位数

void EPD_Dis_u32data16x32( INT8U page, INT8U column, u32 data )
{
	u8 i;
		char PBufFreq[] = {0,0,0,0,0,0,0,0,0,0};
		char buf[30];
		
		i= DatatoASC_n(data ,buf);
		memcpy(PBufFreq, &buf[10-i], i);//将频率低位值复制到buf中
		EPD_Dis_Str16x32(page,column,PBufFreq);

}


void EPD_Dis_Logo( void )
{
	//垂直方向0-14
	//水平方向0-14*16
	//布局
	//--0-1-2-3-4-5-6-7-8-9-1011121314
	//-0火	龙	果
	//-2				￥6.49
	//-4
	//-6产地：海南
	//-8货号：316601
	//10会员价：6.00
	//12日期：2019.01.01	 条形码
	//14					 316601
//	 品(0) 名(1) 火(2) 龙(3) 果(4) 香(5) 蕉(6) 西(7)
//    瓜(8) 产(9) 地(10) 海(11) 南(12) 泰(13) 国(14) 浙(15)
//	 江(16) 货(17) 号(18) 单(19) 价(20) 会(21) 员(22) 价(23) 
//    日(24) 期(25) 计(26) 价(27) 单(28) 位(29)

//火龙果
	EPD_Dis_32x32(0,0,(unsigned char *)&EPD_GBFont32x32[2*128]);
	EPD_Dis_32x32(0,1,(unsigned char *)&EPD_GBFont32x32[3*128]);
	EPD_Dis_32x32(0,2,(unsigned char *)&EPD_GBFont32x32[4*128]);
//产地:海南
	EPD_Dis_16x16(6,0,(unsigned char *)&EPD_GBFont16x16[9*32]);
	EPD_Dis_16x16(6,1,(unsigned char *)&EPD_GBFont16x16[10*32]);
	EPD_Dis_Str8x16(6,4,":");
	
	EPD_Dis_16x16(6,3,(unsigned char *)&EPD_GBFont16x16[11*32]);
	EPD_Dis_16x16(6,4,(unsigned char *)&EPD_GBFont16x16[12*32]);
//货号
	EPD_Dis_16x16(8,0,(unsigned char *)&EPD_GBFont16x16[17*32]);
	EPD_Dis_16x16(8,1,(unsigned char *)&EPD_GBFont16x16[18*32]);
	EPD_Dis_Str8x16(8,4,":");

//会员价
	EPD_Dis_16x16(10,0,(unsigned char *)&EPD_GBFont16x16[21*32]);
	EPD_Dis_16x16(10,1,(unsigned char *)&EPD_GBFont16x16[22*32]);
	EPD_Dis_16x16(10,2,(unsigned char *)&EPD_GBFont16x16[23*32]);
	EPD_Dis_Str8x16(10,6,":");

//日期
	EPD_Dis_16x16(12,0,(unsigned char *)&EPD_GBFont16x16[24*32]);
	EPD_Dis_16x16(12,1,(unsigned char *)&EPD_GBFont16x16[25*32]);
	EPD_Dis_Str8x16(12,4,":");

//$
EPD_Dis_32x32(3,3,(unsigned char *)&EPD_GBFont32x32[30*128]);

//
EPD_Dis_Str8x16(0,22,"Copr.");
EPD_Dis_16x16(0,29,(unsigned char *)&EPD_GBFont16x16[30*32]);
EPD_Dis_16x16(0,30,(unsigned char *)&EPD_GBFont16x16[31*32]);

}

/***********************************************************
						end file
***********************************************************/

