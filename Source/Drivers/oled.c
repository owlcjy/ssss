
#include "OLED.h"
#include "font.h"

/*Write a command to OLED module*/
static void LCD_WrCmd(INT8U cmd);

/*Write a byte to OLED module*/
static void LCD_WrDat(INT8U dt);

/*Set the position of the OLED*/
static void LCD_Set_Pos( INT8U page, INT8U column );


/*
=================================================================================
LCD_Dis_16x16( );
Function : Display 16x16
INTPUT   : page, the page of the LCD, 0-7
           column, the column of the LCD, 0-127
           p_data, the data array for display
OUTPUT   : None
=================================================================================
*/
void LCD_Dis_16x16( INT8U page, INT8U column, const INT8U *p_data )
{
    INT8U i;

    LCD_Set_Pos( page, column );
    for( i = 0; i < 16; i ++ )
    {
        LCD_WrDat( *p_data ++ );
    }
    LCD_Set_Pos( page+1, column );
    for( i = 0; i < 16; i ++ )
    {
        LCD_WrDat( *p_data ++ );
    }

}

void LCD_Dis_8x16( INT8U page, INT8U column, const INT8U *p_data )
{
    INT8U j;

    LCD_Set_Pos( page, column );

    for( j = 0; j < 8; j ++ )
    {
        LCD_WrDat(*p_data ++ );
    }
    LCD_Set_Pos( page+1, column );
    for( j = 0; j < 8; j ++ )
    {
        LCD_WrDat(*p_data ++ );
    }

}

/*
=================================================================================
LCD_Dis_Logo( );
Function : Display the manufacture LOGO
INTPUT   : None
OUTPUT   : None
=================================================================================
*/
void LCD_Dis_Logo( void )
{

	//��ǿ
    LCD_Dis_16x16( 0, 0*16, &Our_Logo[32*2] );
    LCD_Dis_16x16( 0, 1*16, &Our_Logo[32*3] );
	//Ƶ��
    LCD_Dis_16x16( 2, 0*16, &Our_Logo[32*0] );
    LCD_Dis_16x16( 2, 1*16, &Our_Logo[32*1] );
	//����
    LCD_Dis_16x16( 4, 0*16, &Our_Logo[32*4] );
    LCD_Dis_16x16( 4, 1*16, &Our_Logo[32*5] );
	//ģʽ
    LCD_Dis_16x16( 6, 0*16, &Our_Logo[32*6] );
    LCD_Dis_16x16( 6, 1*16, &Our_Logo[32*7] );	
	//��ֵ
    LCD_Dis_16x16( 6, 3*16, &Our_Logo[32*12] );
    LCD_Dis_16x16( 6, 4*16, &Our_Logo[32*13] );	
	LCD_Dis_16x16( 6, 5*16, &Our_Logo[32*14] );
    LCD_Dis_16x16( 6, 6*16, &Our_Logo[32*15] );	
//Ӣ���ַ�
	//0":"//-1"B"	//-2"H"	//-3"M"	-4d//5k//6"m"	//-7"z"
	//dBm
LCD_Dis_8x16( 0, 12*8, &Font8x16_MHz[16*4] );
LCD_Dis_8x16( 0, 13*8, &Font8x16_MHz[16*1] );
LCD_Dis_8x16( 0, 14*8, &Font8x16_MHz[16*6] );

//MHz
LCD_Dis_8x16( 2, 12*8, &Font8x16_MHz[16*3] );
LCD_Dis_8x16( 2, 13*8, &Font8x16_MHz[16*2] );
LCD_Dis_8x16( 2, 14*8, &Font8x16_MHz[16*7] );

//kHz
LCD_Dis_8x16( 4, 12*8, &Font8x16_MHz[16*5] );
LCD_Dis_8x16( 4, 13*8, &Font8x16_MHz[16*2] );
LCD_Dis_8x16( 4, 14*8, &Font8x16_MHz[16*7] );



	
}
/*
=================================================================================
LCD_Set_Pos( );
Function : Set the position of the OLED
INTPUT   : page, 0-7
           column, 0-128
OUTPUT   : None
=================================================================================
*/
void LCD_Set_Pos( INT8U page, INT8U column )
{
    LCD_WrCmd( 0xb0 + ( page & 0x07 ) );
    LCD_WrCmd( 0x10 + ( ( column>>4 ) & 0x0F ) );
    LCD_WrCmd( column & 0x0F );
}
/*
=================================================================================
LCD_Dis_Str( );
Function : Display a character
INTPUT   : page, 0-7
           column, 0-128
           ch, the character to be displayed
OUTPUT   : None
=================================================================================
*/
void LCD_Dis_Char( INT8U page, INT8U column, char ch )
{
    INT16U pos;
    INT8U j;
    if( ch < ' ' )  { return; }

    pos = 16*( ch - ' ' );
    LCD_Set_Pos( page, column );

    for( j = 0; j < 8; j ++ )
    {
        LCD_WrDat( Font8x16[ pos++ ] );
    }
    LCD_Set_Pos( page+1, column );
    for( j = 0; j < 8; j ++ )
    {
        LCD_WrDat( Font8x16[ pos++ ] );
    }
}
/*
=================================================================================
LCD_Dis_Str( );
Function : Display a string
INTPUT   : page, 0-7
           column, 0-128
           str, the string to be displayed
OUTPUT   : None
=================================================================================
*/
void LCD_Dis_Str( INT8U page, INT8U column, char *str )
{
    while( *str )
    {
        LCD_Dis_Char( page, column, *str++ );
        column += 8;
    }
}
/*
=================================================================================
LCD_WrCmd( );
Function : Write a command to OLED module
INTPUT   : cmd, the command byte
OUTPUT   : None
=================================================================================
*/
void LCD_WrCmd(INT8U cmd)
{
	SPI2_Init_CPOL(0);

    OLED_CSN_L( );
    OLED_CMD_L( );//LCD_DC=0;
    SPI_ExchangeByte( cmd );
    OLED_CSN_H( );
	//SPI2_Init_CPOL(1);
}
/*
=================================================================================
LCD_WrDat( );
Function : Write a byte to OLED module
INTPUT   : dt, the data byte
OUTPUT   : None
=================================================================================
*/
void LCD_WrDat(INT8U dt)
{
	SPI2_Init_CPOL(0);

    OLED_CSN_L( );
    OLED_CMD_H( );//LCD_DC=1;
    SPI_ExchangeByte( dt );
    OLED_CSN_H( );
	//SPI2_Init_CPOL(1);
}
/*
=================================================================================
LCD_Fill( );
Function : flush the whole screen
INTPUT   : bmp_dat, the data value
OUTPUT   : None
=================================================================================
*/
void LCD_Fill(INT8U bmp_dat)
{
    INT8U y,x;
    for(y=0;y<8;y++)
    {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10);
        for(x=0;x<128;x++)
        LCD_WrDat(bmp_dat);
    }
}
/*
=================================================================================
LCD_Init( );
Function : Initialize the OLED module
INTPUT   : none
OUTPUT   : None
=================================================================================
*/
void LCD_Init(void)
{
 //   INT16U x;
    OLED_RST_L() ;//LCD_RST=0;
    delay_ms(1000);//for( x = 0; x < 10000; x ++ );
    OLED_RST_H( );//LCD_RST=1;       //���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���
    LCD_WrCmd(0xae);//--turn off oled panel
    LCD_WrCmd(0x00);//---set low column address
    LCD_WrCmd(0x10);//---set high column address
    LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    LCD_WrCmd(0x81);//--set contrast control register
    LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
    LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    LCD_WrCmd(0xa6);//--set normal display
    LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    LCD_WrCmd(0x3f);//--1/64 duty
    LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    LCD_WrCmd(0x00);//-not offset
    LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    LCD_WrCmd(0xd9);//--set pre-charge period
    LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    LCD_WrCmd(0xda);//--set com pins hardware configuration
    LCD_WrCmd(0x12);
    LCD_WrCmd(0xdb);//--set vcomh
    LCD_WrCmd(0x40);//Set VCOM Deselect Level
    LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    LCD_WrCmd(0x02);//
    LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
    LCD_WrCmd(0x14);//--set(0x10) disable
    LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    LCD_WrCmd(0xaf);//--turn on oled panel
    LCD_Fill(0x00);  //��ʼ����
}
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
