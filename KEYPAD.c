/*
 * KEYPAD.c
 *
 * Created: 30/01/2016 06:38:37 م
 *  Author: hossam
 */ 
#include "DIO.h"

void KPD_Init(void);
void KPD_ReadVal(char* ValuePtr);

/*Local Symbols*/
#define KPD_COL_PORT ((u8)0x03)
#define KPD_ROW_PORT ((u8)0x03)
#define KPD_COL_MASK ((u8)0x70)
#define KPD_ROW_MASK ((u8)0x0f)
#define KPD_COL_PIN_NUM 4u
#define KPD_ROW_PIN_NUM 0u
/**************************************************/
 

void KPD_Init(void)
{
    DIO_InitPortDirection(KPD_COL_PORT,KPD_COL_MASK,0x00u);
    DIO_InitPortDirection(KPD_ROW_PORT,KPD_ROW_MASK,0xffu);
    DIO_WritePort(KPD_ROW_PORT,KPD_ROW_MASK,0x00u);
}

void KPD_ReadVal(char* ValuePtr)
{
    static const unsigned char KeysLut[]= {'1' , '2' , '3' , '4' , '5' , '6','7' , '8' , '9','*' , '0' , '#'};
	unsigned char Rowdata;
	unsigned char ColData = 0u;
	unsigned char * VALPTR = &ColData;
	unsigned char LoopTermnate = 0u;
	for(Rowdata = 0u ; (Rowdata < 4u) && (LoopTermnate == 0u) ; Rowdata ++)
	{
	    DIO_WritePort(KPD_ROW_PORT,KPD_ROW_MASK,((u8)(1u<<(u16)Rowdata)<<(KPD_ROW_PIN_NUM)));
        DIO_WritePort(KPD_COL_PORT,KPD_COL_MASK,(*VALPTR));
		*(VALPTR) = (*(VALPTR)) >> KPD_COL_PIN_NUM;

		if(ColData != 0u)
		{
			*ValuePtr = (char) KeysLut[(Rowdata*3u) + (ColData/2u)];
			LoopTermnate = 1u;
		}
		else
		{
			*ValuePtr = 'n';
		}
	}

	
	
}

