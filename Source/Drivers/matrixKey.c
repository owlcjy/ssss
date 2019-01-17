#include "matrixKey.h"
#include "board.h"


/*���ļ��ĺ�������Ҫʵ�־�����̵Ĺ��ܡ��������ʹ��PC0��PC6���ţ�PF6��PF9.���У�PF6��PF9�̶�Ϊ���룬PC0��PC6�̶�Ϊ
��©����������޼�����ʱ����ӦPF6��PF9Ϊ1���м�����ʱ��PF6��PF9�У���Ӧ������Ϊ0��*/
u8 FaceKeyStatus=0;
u8 CheckKeyTimer;
u16 CheckPKeyTimer=0x1ff;

//keyin PF6~PF9(Ӳ����������VCC)   STM8 PA2/PC0/PC1,
//keyout PC0~PC7 						STM8 PB0/PB1,
void InitMKey(void) //��ʼ���������Ҫʹ�õ�GPIO�ڡ�
{
   ;//��ǰ���ʼ����


}
/***********************************************************************
* ���ƣ�matrixKeyScan()
* ���ܣ���ϰ���ɨ��������֧��˫��
* ��ڲ�����
* ���ڲ�����keyVal:��ֵ �߰�λ:��ռ4λ��XY���꣬�Ͱ�λ:��ռ4λ��XY����
//�򻯴���jycheng2018��12��9��16:41:08
* ����: tthe 15.11.18
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
	   (SET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) )  //�����PF6��PF9ȫΪ1����û�м����¡���ʱ������ֵΪ0xff.
    {
        return 0xff;
    }
        keyVal = 0;
	for(y=0;y<4;y++)
    {
        outState = 0XFFFF&(~(1<<y));   //PA //��PC0-6���������Ϊ�͡�

        GPIOA->ODR|=0X000F;
        GPIOA->ODR&=outState;

        delay_us(300);

        //keyValTemp = (~(GPIOB->IDR>>5))&0X0f; //PB5~7//�õ�PF6-9 ȡ����ֵ
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
* ���ƣ�getComKeyVal(void)
* ���ܣ���ϼ� ��ֵת��
* ��ڲ�����
* ���ڲ�����KeyValue:����ֵ .˫��: �߰�λ�洢�Ȱ��µļ�ֵ���Ͱ�λ�洢����µļ�ֵ  ����:�洢�ڵͰ�λ
* ����: tthe 15.11.20
***********************************************************************/
	u16 getComKeyVal(void)
	{
		u16 keyXYValue,keyValue = 0;
		static u16 oneKeyValueOld = 0; //��¼��һ�ε�����ֵ����˫������ʱ���жԱ��жϰ����Ⱥ�
		u8 valueX = 0,valueY = 0,i;
		static u8 oldFlag = 0;
	
		static u8 Power_val = 0;
	
		keyXYValue = KeyProcess();
		//������ֵ ת��Ϊ��ֵ�� �ֱ�洢��keyValue�ĸ߰�λ��Ͱ�λ��
		for(i=0;i<2;i++)
		{
			valueX= (keyXYValue>>((i<<3)+4))&0x0f;
			valueY= (keyXYValue>>(i<<3))&0x0f;
			if((0!=valueY)&&(0x0f!=valueY)) keyValue |= (valueX+(valueY<<2)-4)<<(i<<3); //��ֵ��������ָ��ֵ�����м�ֵת��
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
			case KEY_RELEASE:  //�����ɿ� 0x20
				oldFlag = 0;
				keyValue = KEY_RELEASE;
				oneKeyValueOld = 0;
				break;
			case KEY_CHANGE:  //��ϼ��ı� 0x30
				oldFlag = 1;
				return 0xff;
			case KEY_NO_KEY:
				oldFlag = 0;
				return 0xff; //�޼�ֵ���������� 0xff
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
				break;	 //���ؼ�ֵ�����
		}
	
		return keyValue;
	}


u8 Check_key_state(void)
{
   // GPIOC->ODR&=~0X007F;// PC0~6 ���ȫ����Ϊ�͵�ƽ

    GPIO_ResetBits( GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 );
    delay_us(300);
    if((RESET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))||\
	   (RESET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))||\
	   (RESET==GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) )  //�����PF6��PF9ȫΪ1����û�м����¡���ʱ������ֵΪ0xff.
   //�����PF6��PF9ȫΪ1����û�м����¡���ʱ������ֵΪ0xff.
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
                CheckKeyTimer = 3;                     //����ʱ���趨��ֵΪ3*10ms
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
            else if(keyValue==keyValueTemp) //��ֵ����һ����ȣ�����״̬����״̬����
            {
                //keyValueTemp = keyValue;
                //FaceKeyStatus = KEY_WAIT;
                return KEY_NO_KEY;
            }
            else                            //��ֵ�����ı�
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

