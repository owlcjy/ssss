#include "matrixKey.h"
#include "board.h"


/*本文件的函数，主要实现矩阵键盘的功能。矩阵键盘使用PC0到PC6引脚，PF6到PF9.其中，PF6到PF9固定为输入，PC0到PC6固定为
开漏输出。即，无键按下时，对应PF6到PF9为1，有键按下时，PF6到PF9中，对应的引脚为0。*/
u8 FaceKeyStatus=0;
u8 CheckKeyTimer;
u16 CheckPKeyTimer=0x1ff;

//keyin PF6~PF9(硬件上上拉至VCC)   STM8 PA2/PC0/PC1,
//keyout PC0~PC7 						STM8 PB0/PB1,
void InitMKey(void) //初始化矩阵键盘要使用的GPIO口。
{
   ;//在前面初始化了


}
/***********************************************************************
* 名称：matrixKeyScan()
* 功能：组合按键扫描程序，最多支持双键
* 入口参数：
* 出口参数：keyVal:键值 高八位:各占4位的XY坐标，低八位:各占4位的XY坐标
//简化处理jycheng2018年12月9日16:41:08
* 作者: tthe 15.11.18
***********************************************************************/
u16 matrixKeyScan(void)
{
    u16 keyVal,keyValTemp;
    u8 x,y;
    u16 outState = 0;
	//PA0~3-KEYOUT1~4
 	//PB5-7,KEYIN1~3,
 	
    //GPIOC->ODR&=~0X007F;
     GPIO_ResetBits( GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 );
    delay_us(300);
    if((SET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))&&\
	   (SET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))&&\
	   (SET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) )  //如果，PF6到PF9全为1，则，没有键按下。此时，返回值为0xff.
    {
        return 0xff;
    }
        keyVal = 0;
	for(y=0;y<4;y++)
    {
        outState = 0XFFFF&(~(1<<y));   //PA //将PC0-6的输出依次为低。

        GPIOA->ODR|=0X000F;
        GPIOA->ODR&=outState;

        delay_us(300);

        //keyValTemp = (~(GPIOB->IDR>>5))&0X0f; //PB5~7//得到PF6-9 取反的值
		keyValTemp = GPIOB->IDR; 
		keyValTemp =keyValTemp >>5;
		keyValTemp =~keyValTemp;
		keyValTemp =keyValTemp&0X07;
		

        for(x=0;x<4;x++)
        {
            if(keyValTemp&(1<<x))
            {
                keyVal<<=4;
                keyVal|=(x+1);
                keyVal<<=4;
                keyVal|=(y+1);
            }
        }
    }
    return keyVal;
	
	
}

/***********************************************************************
* 名称：getComKeyVal(void)
* 功能：组合键 键值转化
* 入口参数：
* 出口参数：KeyValue:坐标值 .双键: 高八位存储先按下的键值，低八位存储最后按下的键值  单键:存储在低八位
* 作者: tthe 15.11.20
***********************************************************************/
	u16 getComKeyVal(void)
	{
		u16 keyXYValue,keyValue = 0;
		static u16 oneKeyValueOld = 0; //记录上一次单键的值，在双键按下时进行对比判断按键先后
		u8 valueX = 0,valueY = 0,i;
		static u8 oldFlag = 0;
	
		static u8 Power_val = 0;
	
		keyXYValue = KeyProcess();
		//将坐标值 转化为键值。 分别存储在keyValue的高八位与低八位。
		for(i=0;i<2;i++)
		{
			valueX= (keyXYValue>>((i<<3)+4))&0x0f;
			valueY= (keyXYValue>>(i<<3))&0x0f;
			if((0!=valueY)&&(0x0f!=valueY)) keyValue |= (valueX+(valueY<<2)-4)<<(i<<3); //键值不是特殊指令值，进行键值转化
			else keyValue |= 0<<(i<<3);
		}
	
		if(KEY_PWON_ID != Power_val)
		{
			if(Bit_RESET==KEY_PWON_ID)	
				{
				CheckPKeyTimer = OFF_CHECK_TIME;
				}
			Power_val = KEY_PWON_ID;
		}
	
		if(0==CheckPKeyTimer)
		{
			CheckPKeyTimer = 0x1FF;
			return KEY_PWR_OFF;
		}
		if(0x1FE==CheckPKeyTimer)
		{
			CheckPKeyTimer = 0x1FF;
			return KEY_SET;
		}
		switch(keyXYValue)
		{
			case KEY_RELEASE:  //按键松开 0x20
				oldFlag = 0;
				keyValue = KEY_RELEASE;
				oneKeyValueOld = 0;
				break;
			case KEY_CHANGE:  //组合键改变 0x30
				oldFlag = 1;
				return 0xff;
			case KEY_NO_KEY:
				oldFlag = 0;
				return 0xff; //无键值反馈，跳出 0xff
			default:
				if(oldFlag)
				{
					oldFlag = 0;
					if(0==(keyValue>>8)) oneKeyValueOld = keyValue;
					else if(oneKeyValueOld==(keyValue&0xff)) keyValue = (oneKeyValueOld<<8)|(keyValue>>8);
				}
				else
				{
					oneKeyValueOld = keyValue;
				}
				break;	 //返回键值，输出
		}
	
		return keyValue;
	}


u8 Check_key_state(void)
{
   // GPIOC->ODR&=~0X007F;// PC0~6 输出全部拉为低电平

    GPIO_ResetBits( GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 );
    delay_us(300);
    if((RESET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))||\
	   (RESET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))||\
	   (RESET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) )  //如果，PF6到PF9全为1，则，没有键按下。此时，返回值为0xff.
   //如果，PF6到PF9全为1，则，没有键按下。此时，返回值为0xff.
        return 1;
    else
        return 0;
}

u16 KeyProcess(void)
{
    u16 MkeyValue,keyValue;
    static u16 keyValueTemp;
    switch(FaceKeyStatus){
        case KEY_IDLE:
            if (Check_key_state())
            {
                CheckKeyTimer = 3;                     //防抖时间设定，值为3*10ms
                FaceKeyStatus = KEY_SCAN;
                return KEY_NO_KEY;
            }
            return KEY_NO_KEY;
        case KEY_SCAN:
            if(0 !=CheckKeyTimer)
                return KEY_NO_KEY;
            keyValue = matrixKeyScan();
            if(keyValue == KEY_NO_KEY)
            {
                FaceKeyStatus = KEY_IDLE;
                return KEY_NO_KEY;
            }
            else
            {
                keyValueTemp = keyValue;
                MkeyValue = keyValue;
                FaceKeyStatus = KEY_WAIT;
                return MkeyValue;
            }
            break;
        case KEY_WAIT:
            keyValue = matrixKeyScan();
            if(keyValue == KEY_NO_KEY)
            {
                FaceKeyStatus = KEY_OFF;
                MkeyValue = KEY_NO_KEY;
                CheckKeyTimer = 3;
                return KEY_NO_KEY;
            }
            else if(keyValue==keyValueTemp) //键值与上一次相等，跳出状态机，状态不变
            {
                //keyValueTemp = keyValue;
                //FaceKeyStatus = KEY_WAIT;
                return KEY_NO_KEY;
            }
            else                            //键值发生改变
            {
                keyValueTemp = keyValue;
                FaceKeyStatus = KEY_SECOND;
                CheckKeyTimer = 3;
                return KEY_CHANGE;
            }
            break;
        case KEY_SECOND:
            if(0 !=CheckKeyTimer)
                return KEY_NO_KEY;
            keyValue = matrixKeyScan();
            if(keyValue == KEY_NO_KEY)
            {
                FaceKeyStatus = KEY_OFF;
                return KEY_NO_KEY;
            }
            else if(keyValueTemp == keyValue)
            {
                MkeyValue = keyValue;
                FaceKeyStatus = KEY_WAIT;
                return MkeyValue;
            }
            else
            {
                FaceKeyStatus = KEY_IDLE;
                return KEY_NO_KEY;
            }
            break;
        case KEY_OFF:
            if(0 !=CheckKeyTimer)
                return KEY_NO_KEY;
            keyValue = matrixKeyScan();
            if(keyValue == KEY_NO_KEY)
            {
                FaceKeyStatus = KEY_IDLE;
                return KEY_RELEASE;
            }
            else
            {
                FaceKeyStatus = KEY_WAIT;
                return KEY_NO_KEY;
            }
            break;
    }
}

