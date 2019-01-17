#ifndef _OLED_H_
#define _OLED_H_




#include "board.h"

/*Initialize the OLED module*/
void LCD_Init( void );

/*flush the whole screen*/
void LCD_Fill(INT8U bmp_dat);

/*Display a character*/
void LCD_Dis_Char( INT8U page, INT8U column, char ch );

/*Display a string*/
void LCD_Dis_Str( INT8U page, INT8U column, char *str );

void LCD_Dis_Logo( void );
void LCD_Dis_16x16( INT8U page, INT8U column, const INT8U *p_data );
void LCD_Dis_8x16( INT8U page, INT8U column, const INT8U *p_data );
#endif //_OLED_H_
