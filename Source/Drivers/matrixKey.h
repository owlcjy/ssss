#ifndef __MATRIXKEY_H
#define __MATRIXKEY_H
//#include "sys.h"
#include "stm32f10x.h"

#define RD_KEYPress_L2H         0x93

#define KEY_IDLE         0
#define KEY_FIRST        1
#define KEY_SCAN         2
#define KEY_WAIT         3
#define KEY_SECOND       4
#define KEY_OFF          5

#define KEY_NO_KEY        0xff      // �޼�����
#define KEY_RELEASE       0x20      // ���ɿ�,�κμ�����
#define KEY_CHANGE        0x30      // ��ϼ��ı�

#define KEY_ENTER            1   //S3 ���ù���  ԭKEY_FUN1//SCOPE
#define KEY_UP              2   //S4 ���Ϸ����
#define KEY_RETURN            3   //S5 ���ù���  ԭKEY_FUN2//MEG O

#define KEY_LEFT            5   //S8 �������
#define KEY_CONFIRM         6   //S9 ȷ�ϼ�//��������м�ȷ�ϼ�
#define KEY_RIGHT           7   //S10 ���ҷ����

#define KEY_DIAL          9   //S13 ���ż�   ԭKEY_DILA
#define KEY_DOWN            10  //A//S14 ���·����


#define KEY_RESTE            KEY_Y  //B//HOME��
#define KEY_ENTER          KEY_X  //B//HOME��

#define KEY_SET 100


#define KEY_FREQ_DOWN             KEY_ONE
#define KEY_FREQ_UP           KEY_THREE  

#define KEY_SPAN_DOWN             KEY_FOUR 
#define KEY_SPAN_UP           KEY_SIX 

#define KEY_MODE_DOWN           KEY_SEVEN   
#define KEY_MODE_UP           KEY_NINE 



#define POW_OFF            GPIO_ResetBits( GPIOB, GPIO_Pin_9 )
#define POW_ON            GPIO_SetBits( GPIOB, GPIO_Pin_9 )

#define KEY_PWON_ID         GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)

#define KEY_ONE             1    //D// 1 S17//
#define KEY_TWO             2    //E// 2 S18
#define KEY_THREE           3    //F// 3 S19

#define KEY_FOUR            5    //11// 4 S22
#define KEY_FIVE            6    //12// 5 S23
#define KEY_SIX             7    //13// 6 S24

#define KEY_SEVEN           9    //15// 7 S27
#define KEY_EIGHT           10    //16// 8 S28
#define KEY_NINE            11    //17// 9 S29

#define KEY_STAR            25    //19// +- S32
#define KEY_PL_MI            25    //19// +- S32//owl����


#define KEY_ZERO            14    //1A// 0 S33
#define KEY_X          13    //1B//. S34
#define KEY_Y          15    //1B//. S34//owl����



#define KEY_CH1              4     // ����ѡ��ͨ��1 S6 S7//WA
#define KEY_CH2              8     // ����ѡ��ͨ��2 S11 S12//Tŷķ 
#define KEY_CH3              12    //C// ����ѡ��ͨ��3 S15 S16//�س���
#define KEY_CH4              16    //10// ����ѡ��ͨ��4 S20 S21//um
#define KEY_CH5              20    //14// ����ѡ��ͨ��5 S25 S26//nk
#define KEY_CH6              24    //18// ����ѡ��ͨ��6 S30 S31//GM
#define KEY_CH7              28    //1C// ����ѡ��ͨ��7 S35 S36//SHIFT

#define KEY_WA              4     // ����ѡ��ͨ��1 S6 S7//WA//owl����
#define KEY_TO              8     // ����ѡ��ͨ��2 S11 S12//Tŷķ //owl����
#define KEY_BIG_ENTER              12    //C// ����ѡ��ͨ��3 S15 S16//�س���//owl����
#define KEY_um              16    //10// ����ѡ��ͨ��4 S20 S21//um//owl����
#define KEY_nk              20    //14// ����ѡ��ͨ��5 S25 S26//nk//owl����
#define KEY_GM              24    //18// ����ѡ��ͨ��6 S30 S31//GM//owl����
#define KEY_SHIFT              28    //1C// ����ѡ��ͨ��7 S35 S36//SHIFT//owl����


#define KEY_PPT              28//added by wchen,20151214   

#define KEY_PWR_OFF          30
#define OFF_CHECK_TIME       300

extern u8 FaceKeyStatus;
extern u8 CheckKeyTimer;
extern u16 CheckPKeyTimer;


void InitMKey(void);//IO��ʼ��
// u8 Matrixkey_scan(void);    //���󰴼�ɨ�躯��
u16 matrixKeyScan(void);
u16 KeyProcess(void);//������������add by zqh 13.7.16
u16 getComKeyVal(void);

#endif

