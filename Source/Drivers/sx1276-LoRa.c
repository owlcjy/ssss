/******************** (C) COPYRIGHT tongxinmao.com ***************************
* 文件名		: sx1276-LoRa.C
* 作者   		: tcm123@126.com
* 创建日期  	: 2016.3.8
* 描述			: SX1276/78驱动文件 
* 版本			: V1.0
* 历史记录  	: 暂无
********************************************************************************/

#include "delay.h"
 
#include "stm32f10x_spi.h"
 
#include "sx1276-LoRa.h"
 
#include <stdio.h>
#include "usart1.h"




#define gb_SF  5
//#define gb_BW  6
static u8 gb_BW=6;
//#define CR    0x04 
static u8 CR=0x04;
//新增
static u8 Header=0x01;////报头模式,0，显示，1隐式

//#define CRC_EN   0x00  //CRC Enable
 static u8 CRC_EN=0x00;//CRC是否开启，0不开启，1开启
 //前导码长度高位和低位
 static u8 Preamble_len_H=0;
 static u8 Preamble_len_L=12;
 //使用PA还是RFO
 static u8 RFO=0x87;

 u8 gtmp;
 
 
/**********************************************************
**Parameter table define
**********************************************************/
u16 RFM96FreqTbl[3] = {0x066C, 0x0780, 0x0800}; //434MHz  32M晶振0x066C, 0x0780, 0x0800 	   30M:0x0673, 0x07BB, 0x08BB   {0x85,0x89,0xd9},//26M-TCXO 434M

u16 RFM96PowerTbl[4] =
{ 
  0x09FF,                   //20dbm  
  0x09FC,                   //17dbm
  0x09F9,                   //14dbm
  0x09F6,                   //11dbm 
};

 /*
 const u8 RFM96SpreadFactorTbl[7] =
{
  6,7,8,9,10,11,12
   
}; */

u8 RFM96SpreadFactorTbl[6] =
{
  7,8,9,10,11,12
};//由于6扩频因子档位只能是固定数据包长所以去掉6扩频因子档位

u8 RFM96LoRaBwTbl[10] =
{// 0        1     2       3      4       5         6      7       8      9
//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
  0,1,2,3,4,5,6,7,8,9
};

u8  RFM96Data[] = {"1234567890ABCDEFGHIJK"};

extern SX_Struct_TESTER_STATUS SX_gTesterStatus;


void SX_RADIO_deal(void)
{
	INT8U i;
	INT16U j = 0;
	float f_RF,f_reg;	
	u8  sf;
	u32 u_f_reg;
	u8 buf[4];
	
	//根据频率设置开关
	if(SX_gTesterStatus.SX_Freq <700000)
		{
		LORA_L_OUT();
		}
	else
		{
		LORA_H_OUT();
		}
	//灵敏度测试模式，频率信息，单位1kHz
	
	f_RF=(float)(SX_gTesterStatus.SX_Freq);//频率值转化为浮点数，单位为kHz
	f_reg=f_RF*16.384;
	u_f_reg=(unsigned long)(f_reg);//转换为整数格式
	u_f_reg=u_f_reg & 0x00FFFFFF;//确保数据有效


	//将值放到buf中
	buf[0]=(unsigned char)((u_f_reg>>16)&0x000000FF);
	buf[1]=(unsigned char)((u_f_reg>>8)&0x000000FF);
	buf[2]=(unsigned char)((u_f_reg)&0x000000FF);
	RFM96FreqTbl[0]=0x0600+buf[0];
	RFM96FreqTbl[1]=0x0700+buf[1];
	RFM96FreqTbl[2]=0x0800+buf[2];

	//灵敏度测试模式，功率信息，单位0.1dbm

	//目前第一版使用的是外购模块，所以只能顺着这个来，使用PA引脚
	if((SX_gTesterStatus.SX_Power>=0)&&(SX_gTesterStatus.SX_Power<=9))
	{
		if(SX_gTesterStatus.SX_Power==9)//9表示使用PA引脚
			{
				RFM96PowerTbl[0]=0x0981;//输出2dbm
				RFO=0x87;
			}
		if(SX_gTesterStatus.SX_Power==0)//0表示使用RFO,此时输出-4.2dbm
			{
				RFM96PowerTbl[0]=0x0900;//输出-4.2dbm
				//RFM96PowerTbl[0]=0x097F;//输出-4.2dbm
				RFO=0x84;
			}

	}
	//设置模式下设置的调制速率
	//速率是倒推出来的，故没有设置意义

	//扩频因子6~12
	if((SX_gTesterStatus.SX_SF>=6)&&(SX_gTesterStatus.SX_SF<=12))
	{
		RFM96SpreadFactorTbl[gb_SF]=SX_gTesterStatus.SX_SF;

	}
	//带宽0:7.8;	1:10.4;	2:15.6;	3:20.8;	4:31.25;	5:41.7;	6:62.5;	7125;	8:250;	9:500.
	gb_BW=SX_gTesterStatus.SX_BW;

	//报头模式,0，显示，1隐式
	if((SX_gTesterStatus.SX_Header>=0)&&(SX_gTesterStatus.SX_Header<=1))
	{
		Header=SX_gTesterStatus.SX_Header;
	}

	//CRC是否开启，0不开启，1开启
	if((SX_gTesterStatus.SX_CRC>=0)&&(SX_gTesterStatus.SX_CRC<=1))
	{
		CRC_EN=SX_gTesterStatus.SX_CRC;
	}

	//前导码长度
	if((SX_gTesterStatus.SX_Preamble>=10)&&(SX_gTesterStatus.SX_Preamble<=65539))
	{
		Preamble_len_H=(u8)((SX_gTesterStatus.SX_Preamble-4) >> 8);
		Preamble_len_L=(u8)((SX_gTesterStatus.SX_Preamble-4) & 0x00FF);
	}

	//纠错编码率,1:4/5;		2:4/6;	3:4/7;	4:4/8;
	if((SX_gTesterStatus.SX_Code>=1)&&(SX_gTesterStatus.SX_Code<=4))
	{
		CR=SX_gTesterStatus.SX_Code;
	}



}



void RF_GpioInt()
{
  /*
  //RF_rst   Pc5
  PC_DDR_DDR5=1; //OUTPUT
  PC_CR1_C15=1; //SET PD

 //  RF_SCK        PB_ODR_ODR5
  PB_DDR_DDR5=1; //SET PD
  PB_CR1_C15=1; //SET PD
  
  
  // RF_MISO       PB_IDR_IDR7 //INPUT
  PB_DDR_DDR7=0; //INPUT
  PB_CR1_C17=1; //PULL-UP-INPUT

 // RF_MOSI       PB_ODR_ODR6 
  PB_DDR_DDR6=1;//OUTPUT
  PB_CR1_C16=1; //SET PD
 
  //RF_NSEL_PIN   PB_ODR_ODR4
  PB_DDR_DDR4=1; //OUTPUT
  PB_CR1_C14=1; //SET PD
  

  //RF_MISO       RF_IRQ       PC_IDR_IDR4
  PC_DDR_DDR4=0; //INPUT
  PC_CR1_C14=1; //PULL-UP-INPUT

   RF_NSEL_PIN=1;
   */
}

/**********************************************************
**Name:     SPICmd8bit
**Function: SPI Write one byte
**Input:    WrPara
**Output:   none
**note:     use for burst mode
**********************************************************/
u8 SPICmd8bit(u8 WrPara)
{
   	/* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, WrPara);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);	
	

}

/**********************************************************
**Name:     SPIRead8bit
**Function: SPI Read one byte
**Input:    None
**Output:   result byte
**Note:     use for burst mode
**********************************************************/

u8 SPIRead8bit(void)
{
   return SPICmd8bit(0XFF);
}

/**********************************************************
**Name:     SPIRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/

u8 SPIRead(u8 adr)
{
  u8 tmp; 

   

  GPIO_ResetBits(RF_GPIO, RF_SEL_PIN);//NSS_Low;
  //Delay_ms(1); 	 

   SPICmd8bit(adr&0x7f);                                        //Send address first
  tmp = SPIRead8bit();  
 //  printf("读addr:%02x  val:%02x\n",adr,tmp);

 //  Delay_ms(1); 
   GPIO_SetBits(RF_GPIO, RF_SEL_PIN);//NSS_High; 
  return(tmp);
}


/**********************************************************
**Name:     SPIWrite
**Function: SPI Write CMD
**Input:    WrPara -> address & data
**Output:   None
**********************************************************/

void SPIWrite(u16 WrPara)                
{                                                       
  
  
  WrPara |= 0x8000;                                        //MSB must be "1" for write 
//  printf("写addr:%02x  val:%02x\n",WrPara>>8,(u8)WrPara);
   GPIO_ResetBits(RF_GPIO, RF_SEL_PIN);//NSS_Low;
 

 SPICmd8bit(WrPara>>8);//	15->0
 
  SPICmd8bit((u8)WrPara);	
 
 GPIO_SetBits(RF_GPIO, RF_SEL_PIN);//NSS_High; 
 
}


/**********************************************************
**Name:     SPIBurstRead
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void SPIBurstRead(u8 adr, u8 *ptr, u8 length)
{
  u8 i;
  if(length<=1)                                            //length must more than one
    return;
  else
  {
    GPIO_ResetBits(RF_GPIO, RF_SEL_PIN);//NSS_Low;
	 	 

    SPICmd8bit(adr&0x7f); 

    for(i=0;i<length;i++)
    ptr[i] = SPICmd8bit(0xff);

    
    GPIO_SetBits(RF_GPIO, RF_SEL_PIN);//NSS_High;
  }
}

/**********************************************************
**Name:     SPIBurstWrite
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void BurstWrite(u8 adr, u8 *ptr, u8 length)
{ 
  u8 i;

 
    GPIO_ResetBits(RF_GPIO, RF_SEL_PIN);//NSS_Low; 
    
    SPICmd8bit(adr|0x80);
    for(i=0;i<length;i++)
    	SPICmd8bit(ptr[i]);

	    
    GPIO_SetBits(RF_GPIO, RF_SEL_PIN);//NSS_High;
  
}


/**********************************************************
**Name:     RFM96_Standby
**Function: Entry standby mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_Standby(void)
{
  SPIWrite(LR_RegOpMode+0x01+0x08);                              //Standby
}

/**********************************************************
**Name:     RFM96_Sleep
**Function: Entry sleep mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_Sleep(void)
{
  SPIWrite(LR_RegOpMode+0x00+0x08);                              //Sleep
}

/*********************************************************/
//LoRa mode
/*********************************************************/
/**********************************************************
**Name:     RFM96_EntryLoRa
**Function: Set RFM69 entry LoRa(LongRange) mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_EntryLoRa(void)
{
  SPIWrite(LR_RegOpMode+0x80+0x08);
}

/**********************************************************
**Name:     RFM96_LoRaClearIrq
**Function: Clear all irq
**Input:    None
**Output:   None
**********************************************************/
void RFM96_LoRaClearIrq(void)
{
  SPIWrite(LR_RegIrqFlags+0xFF);
}

/**********************************************************
**Name:     RFM96_Config
**Function: RFM96 base config
**Input:    mode
**Output:   None
**********************************************************/
//配置基本参数
void RFM96_Config(u8 mode)
{
  u8 i; 
    
   GPIO_ResetBits(GPIOA, RF_RST);//REST_Low XL1278-D01;
	 GPIO_ResetBits(GPIOB, RF_RST_1);//REST_Low  XL1278-SMT;
  for(i=100;i!=0;i--)                                      //Delay
    delay_us(10); 
  
  GPIO_SetBits(GPIOA, RF_RST);//REST_High XL1278-D01;
	GPIO_SetBits(GPIOB, RF_RST_1);//REST_High XL1278-SMT;
  
  for(i=250;i!=0;i--)                                      //Delay
    delay_us(10);  
    
  RFM96_Sleep();                                           //Change modem mode Must in Sleep mode 
  for(i=250;i!=0;i--)                                      //Delay
    delay_us(1);  

	 SPIWrite(REG_LR_TCXO+0x08);//使用TCXO  

	  
    RFM96_EntryLoRa();  
   // SPIWrite(0x5904);   //?? Change digital regulator form 1.6V to 1.47V: see errata note


     {
		u8	Sx1276VerNO = SPIRead((u8)(REG_LR_VERSION>>8)); //获取SX1276 版本号 是0X11(V1A版本 工程版） 或者是 0X12（V1B 正式版）
	//	 printf("sx1278(V12) v:%02X",Sx1276VerNO);
			  printf("sx1278(V12) v:%02X",Sx1276VerNO   );
	//	 SPIWrite(RFM96FreqTbl[0]); 
	//	 Sx1276VerNO=SPIRead((u8)(RFM96FreqTbl[0]>>8));
	 //   printf("RFM96FreqTbl[0](0X06_6C) R:0x%02X",Sx1276VerNO);

       	}
   //频率配置
    for(i=0;i<3;i++)                                       //setting frequency parameter
    {
      SPIWrite(RFM96FreqTbl[i]);  
    }

    //setting base parameter 
    //设置发射功率
	SPIWrite(0x4D00+RFO); 								//发射功率 for 20dBm
	SPIWrite(RFM96PowerTbl[0]);							  //Setting output power parameter	 0=20dBm   

	//SPIWrite(REG_LR_PADAC+0x87); 
	//SPIWrite(REG_LR_PADAC+0x84); 
	//SPIWrite(0x0900); 

	
  //  SPIWrite(LR_RegOcp+0x0B);                              //RegOcp,Close Ocp  
//OCP电流微调
  SPIWrite(LR_RegOcp+0x3B);                                //0X3B : ON Ocp  240mA   *
//LNA设置
	SPIWrite(LR_RegLna+0x23);                              //RegLNA,High & LNA Enable
  //根据SF设置带宽和纠错码率  
    if(RFM96SpreadFactorTbl[gb_SF]==6)           //SFactor=6
    {
      u8 tmp;				
	  //此时只支持隐式报头
      SPIWrite(LR_RegModemConfig1+(RFM96LoRaBwTbl[gb_BW]<<4)+(CR<<1)+(0x01));//带宽设置，包结构，Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
      SPIWrite(LR_RegModemConfig2+(RFM96SpreadFactorTbl[gb_SF]<<4)+(CRC_EN<<2)+0x03);      
      tmp = SPIRead(0x31);
      tmp &= 0xF8;
      tmp |= 0x05;
      SPIWrite(0x3100+tmp);
      SPIWrite(0x3700+0x0C);
    } 
    else
    {
    //带宽、编码率、报头格式
      SPIWrite(LR_RegModemConfig1+(RFM96LoRaBwTbl[gb_BW]<<4)+(CR<<1)+(Header&0x01));//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
      SPIWrite(LR_RegModemConfig2+(RFM96SpreadFactorTbl[gb_SF]<<4)+(CRC_EN<<2)+0x03);  //SFactor &  LNA gain set by the internal AGC loop 
    }


//	SPIWrite(LR_RegModemConfig1+0x36); //BW=3 20.8KHz , CR=3 4/7, explit
//	SPIWrite(LR_RegModemConfig2+0x77); //SF=6, CRC on

//RX超时寄存器
    SPIWrite(LR_RegSymbTimeoutLsb+0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max) 
 //前导码长度   
    SPIWrite(LR_RegPreambleMsb + Preamble_len_H);                       //RegPreambleMsb 
    SPIWrite(LR_RegPreambleLsb + Preamble_len_L);                      //RegPreambleLsb 8+4=12byte Preamble
  //IO口设置，
    SPIWrite(REG_LR_DIOMAPPING2+0x01);                     //RegDioMapping2 DIO5=00, DIO4=01
//符号长度超过16ms时必须开启
	  SPIWrite(LR_RegModemConfig3+0x08);                    //OPEN  SX1276LoRaSetLowDatarateOptimize  *
		SPIWrite(LR_RegSyncWord+0x12);                        //Lora Sync Word  *
	
    RFM96_Standby();                                         //Entry standby mode
}

/**********************************************************
**Name:     RFM96_LoRaEntryRx
**Function: Entry Rx mode
**Input:    None
**Output:   None
**********************************************************/
void RFM96_LoRaEntryRx(void)
{
  u8 addr; 
        
  RFM96_Config(0);                                         //setting base parameter
  
 // SPIWrite(0x4D00+0x84);                                   //PA set  0X84  17dBm  0X87  20dBm
	
  SPIWrite(0x4D00+0x87);                                   //PA set  0X84  17dBm  0X87  20dBm  *
  SPIWrite(LR_RegHopPeriod+0xFF);                          //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1+0x01);                       //DIO0=00, DIO1=00, DIO2=00, DIO3=01  DIO0=00--RXDONE
      
  SPIWrite(LR_RegIrqFlagsMask+0x3F);                       //Open RxDone interrupt & Timeout
  RFM96_LoRaClearIrq();   
  
  //TODO
 // SPIWrite(LR_RegPayloadLength+21);                       //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)
    
  addr = SPIRead((u8)(LR_RegFifoRxBaseAddr>>8));           //Read RxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr+addr);                        //RxBaseAddr -> FiFoAddrPtr　 
  SPIWrite(LR_RegOpMode+0x8D);                        //Continuous Rx Mode
  
}

/**********************************************************
**Name:     RFM96_LoRaRxWaitStable
**Function: Determine whether the state of stable Rx 查询RX 状态
**Input:    none
**Output:   none
**********************************************************/
u8 RFM96_LoRaRxWaitStable(void)
{ 
	uint8_t tmp;
	tmp=SPIRead((u8)(LR_RegModemStat>>8));
	return tmp;
}

/**********************************************************
**Name:     RFM96_LoRaRxPacket
**Function: Receive data in LoRa mode
**Input:    None
**Output:   1- Success
            0- Fail
**********************************************************/
u8 RFM96_LoRaRxPacket(u8 *buf)
{
		u8 i; 
		u8 addr;
		u8 packet_size;

	 
			
				for(i=0;i<32;i++) //清Buffer
					buf[i] = 0x00;

				addr = SPIRead((u8)(LR_RegFifoRxCurrentaddr>>8));      //last packet addr 数据包的最后地址(数据的尾地址)
				SPIWrite(LR_RegFifoAddrPtr+addr);                      //RxBaseAddr -> FiFoAddrPtr   
				delay_us(1);

				if(RFM96SpreadFactorTbl[gb_SF]==6)           //When SpreadFactor is six，will used Implicit Header mode(Excluding internal packet length)
					packet_size=21;
				else
					packet_size = SPIRead((u8)(LR_RegRxNbBytes>>8));     //Number for received bytes    

				gtmp= packet_size;
				SPIBurstRead(0x00, buf, packet_size);

				RFM96_LoRaClearIrq();
				delay_us(1);
				 return packet_size;
		 
	 
}


/**********************************************************
**Name:     RFM96_LoRaEntryTx
**Function: Entry Tx mode
**Input:    None
**Output:   None
**********************************************************/
u8 RFM96_LoRaEntryTx(u8 packet_length)
{
		u8 addr;
		u8 SysTime = 0;
		u8 temp;

		RFM96_Config(0);                                         //模块发射参数设置
		delay_us(1000);
		
		SPIWrite(LR_RegHopPeriod); //跳频关闭                             //RegHopPeriod NO FHSS
		SPIWrite(REG_LR_DIOMAPPING1+0x41);  //IO设置                     //DIO0=01, DIO1=00, DIO2=00, DIO3=01

		RFM96_LoRaClearIrq();//清除所有中断
		SPIWrite(LR_RegIrqFlagsMask+0xF7);    //开启发射中断                   //Open TxDone interrupt
		SPIWrite(LR_RegPayloadLength+packet_length);     //负载长度                  //RegPayloadLength  21byte负载和fifo的字节数的关系是什么？？

		addr = SPIRead((u8)(LR_RegFifoTxBaseAddr>>8));  //获取FIFO地址         //RegFiFoTxBaseAddr
		SPIWrite(LR_RegFifoAddrPtr+addr);    //写入指针                    //RegFifoAddrPtr
		
	while(1)
	{
		temp=SPIRead((u8)(LR_RegPayloadLength>>8) );
		if(temp==packet_length)
		{
			break; 
		}
		if(SysTime>=3)	
			return 0;
	}
 
    return 	packet_length;
}

/**********************************************************
**Name:     RFM96_LoRaTxPacket
**Function: Send data in LoRa mode
**Input:    None
**Output:   1- Send over
**********************************************************/
u8 RFM96_LoRaTxPacket(u8 *buf,u8 len)
{ 
		 
		u16 count=0;	
		BurstWrite(0x00, (u8 *)buf, len);  
		SPIWrite(LR_RegOpMode+0x03+0x08);    //进入发送模式                //Tx Mode       

		   /*
		 	gtmp= SPIRead((u8)(LR_RegIrqFlags>>8));
	//	delay_us(10);	
		gtmp= SPIRead((u8)(LR_RegIrqFlags>>8));
	//	delay_us(10);	
		gtmp= SPIRead((u8)(LR_RegIrqFlags>>8));
	//	delay_us(10);
		   */
	  
	//	while(!RF_IRQ_DIO0) ;                    //Packet send over 发送完成了IRQ 变为H,平时L
	   while(!GPIO_ReadInputDataBit(RF_GPIO, RF_IRQ_PIN))
	{
	   
		if(++count>8000)break;
		delay_ms(1);
	}
		SPIRead((u8)(LR_RegIrqFlags>>8));
		RFM96_LoRaClearIrq();                                //Clear irq
		RFM96_Standby();                                     //Entry Standby mode      

	 
		if(count>8000)
		{
			return 0;
		}	
		return len;  
}

u8 sx1276_7_8_LoRaReadRSSI(void)
{
  u16 temp=10;
  temp=SPIRead(LR_RegRssiValue);                  //Read RegRssiValue,Rssi value
  temp=temp+127-137;                                       //127:Max RSSI, 137:RSSI offset
  return (u8)temp;
}





