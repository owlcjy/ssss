
#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_



extern void driver_delay_xms(unsigned long xms);


void EPD_Dis_Part(unsigned char xStart,unsigned char xEnd,unsigned long yStart,unsigned long yEnd,unsigned char *DisBuffer,unsigned char Label);
void EPD_Dis_Full(unsigned char *DisBuffer,unsigned char Label);
void EPD_init_Full(void);
void EPD_init_Part(void);
void EPD_W21_POWEROFF(void); //POWER OFF
void EPD_Dis_8x16( INT8U page, INT8U column, INT8U *p_data );
void EPD_Dis_16x16( INT8U page, INT8U column, INT8U *p_data );
void EPD_Dis_16x32( INT8U page, INT8U column, INT8U *p_data );
void EPD_Dis_32x32( INT8U page, INT8U column, INT8U *p_data );
void EPD_Dis_Str8x16( INT8U page, INT8U column, char *str );
void EPD_Dis_Str16x32( INT8U page, INT8U column, char *str );
void EPD_Dis_u32data8x16( INT8U page, INT8U column, u32 data );
void EPD_Dis_u32data16x32( INT8U page, INT8U column, u32 data );
u8 DatatoASC_n(u32 data ,char* buffer);
void EPD_Dis_Price8x16( INT8U page, INT8U column, u32 data );
void EPD_Dis_Price16x32( INT8U page, INT8U column, u32 data );


#endif
/***********************************************************
						end file
***********************************************************/


