/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.c
** Descriptions:		Have been tested SPI ILI9320
**						
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2012-1-31
** Version:				2.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			Doug Szumski
** Modified date:		04/07/2013
** Version:
** Descriptions:		Ported to FRDM-KL25Z
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "../system/spi.h"
#include "ili9320.h"
#include "../lib/AsciiLib.h"
#include "../utils/delay.h"

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : LCD write register address
* Input          : - index: register address
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteIndex(uint8_t index)
{

	GPIOA_PCOR = GPIO_PIN(CS); //Low
    /* SPI write data */
	spi_send_recv(SPI_START | SPI_WR | SPI_INDEX);   /* Write : RS = 0, RW = 0       */
	spi_send_recv(0);
	spi_send_recv(index);
	GPIOA_PSOR = GPIO_PIN(CS); //High
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : LCD write register data
* Input          : - data: register data
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteData( uint16_t data)
{
	GPIOA_PCOR = GPIO_PIN(CS); //Low
	spi_send_recv(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
	spi_send_recv((data >>   8));                    /* Write D8..D15                */
	spi_send_recv((data & 0xFF));                    /* Write D0..D7                 */
	GPIOA_PSOR = GPIO_PIN(CS); //High
}

/*******************************************************************************
* Function Name  : LCD_Write_Data_Start
* Description    : Start of data writing to the LCD controller
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Write_Data_Start(void) 
{
	spi_send_recv(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
}

/*******************************************************************************
* Function Name  : LCD_Write_Data_Only
* Description    : Data writing to the LCD controller
* Input          : - data: data to be written  
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Write_Data_Only( uint16_t data)
{
	spi_send_recv((data >>   8));      /* Write D8..D15                */
	spi_send_recv((data & 0xFF));      /* Write D0..D7                 */
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : LCD read data
* Input          : None
* Output         : None
* Return         : return data
* Attention	 : None
*******************************************************************************/
uint16_t LCD_ReadData(void)
{ 
	uint16_t value;
	GPIOA_PCOR = GPIO_PIN(CS); //Low
	spi_send_recv(SPI_START | SPI_RD | SPI_DATA);    /* Read: RS = 1, RW = 1         */
	spi_send_recv(0);                                /* Dummy read 1                 */
	value   = spi_send_recv(0);                      /* Read D8..D15                 */
	value <<= 8;
	value  |= spi_send_recv(0);                      /* Read D0..D7                  */
	GPIOA_PSOR = GPIO_PIN(CS); //High
	return value;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteReg( uint16_t lcd_reg, uint16_t lcd_reg_value)
{
	/* Write 16-bit Index, then Write Reg */  
	LCD_WriteIndex(lcd_reg);
	/* Write 16-bit Reg */
	LCD_WriteData(lcd_reg_value);
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
uint16_t LCD_ReadReg( uint16_t lcd_reg)
{
	uint16_t lcd_ram;
	
	/* Write 16-bit Index (then Read Reg) */
	LCD_WriteIndex(lcd_reg);
	/* Read 16-bit Reg */
	lcd_ram = LCD_ReadData();
    	
	return lcd_ram;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
	/* 0x9320 */
	LCD_WriteReg(0x0020, Xpos );     
	LCD_WriteReg(0x0021, Ypos );           
}

void reset_controller(void) {

	GPIOD_PSOR = GPIO_PIN(RESET);
	delay_ms(50);
	GPIOD_PCOR = GPIO_PIN(RESET);
	delay_ms(50);
	GPIOD_PSOR = GPIO_PIN(RESET);
	delay_ms(100);
}

/*******************************************************************************
* Function Name  : LCD_Initialisation
* Description    : Initialise TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void initialise_controller(void)
{
	reset_controller();

	uint16_t dev_code;
	
	dev_code = LCD_ReadReg(0x0000);		/* Read ID	*/
	/* Different driver IC initialization*/
	if( dev_code == 0x9320 || dev_code == 0x9300 )
	{
		LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100); /* Driver Output Contral */
		LCD_WriteReg(0x02,0x0700); /* LCD Driver Waveform Contral */
		LCD_WriteReg(0x03,0x1038); /* Set the scan mode */
		LCD_WriteReg(0x04,0x0000); /* Scalling Contral */
		LCD_WriteReg(0x08,0x0202); /* Display Contral 2 */
		LCD_WriteReg(0x09,0x0000); /* Display Contral 3 */
		LCD_WriteReg(0x0a,0x0000); /* Frame Cycle Contal */
		LCD_WriteReg(0x0c,(1<<0)); /* Extern Display Interface Contral 1 */
		LCD_WriteReg(0x0d,0x0000); /* Frame Maker Position */
		LCD_WriteReg(0x0f,0x0000); /* Extern Display Interface Contral 2 */     
		delay_ms(50); 
		LCD_WriteReg(0x07,0x0101); /* Display Contral */
		delay_ms(50);    
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4)); /* Power Control 1 */
		LCD_WriteReg(0x11,0x0007);                              /* Power Control 2 */
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));                /* Power Control 3 */
		LCD_WriteReg(0x13,0x0b00);                              /* Power Control 4 */
		LCD_WriteReg(0x29,0x0000);                              /* Power Control 7 */
		LCD_WriteReg(0x2b,(1<<14)|(1<<4)); 
	
		LCD_WriteReg(0x50,0);       /* Set X Start */
		LCD_WriteReg(0x51,239);	    /* Set X End */
		LCD_WriteReg(0x52,0);	    /* Set Y Start */
		LCD_WriteReg(0x53,319);	    /* Set Y End */
		delay_ms(50);

		LCD_WriteReg(0x60,0x2700); /* Driver Output Control */
		LCD_WriteReg(0x61,0x0001); /* Driver Output Control */
		LCD_WriteReg(0x6a,0x0000); /* Vertical Srcoll Control */
		
		LCD_WriteReg(0x80,0x0000); /* Display Position? Partial Display 1 */
		LCD_WriteReg(0x81,0x0000); /* RAM Address Start? Partial Display 1 */
		LCD_WriteReg(0x82,0x0000); /* RAM Address End-Partial Display 1 */
		LCD_WriteReg(0x83,0x0000); /* Displsy Position? Partial Display 2 */
		LCD_WriteReg(0x84,0x0000); /* RAM Address Start? Partial Display 2 */
		LCD_WriteReg(0x85,0x0000); /* RAM Address End? Partial Display 2 */
		
		LCD_WriteReg(0x90,(0<<7)|(16<<0)); /* Frame Cycle Contral */
		LCD_WriteReg(0x92,0x0000);         /* Panel Interface Contral 2 */
		LCD_WriteReg(0x93,0x0001);         /* Panel Interface Contral 3 */
		LCD_WriteReg(0x95,0x0110);         /* Frame Cycle Contral */
		LCD_WriteReg(0x97,(0<<8)); 
		LCD_WriteReg(0x98,0x0000);         /* Frame Cycle Contral */    
		LCD_WriteReg(0x07,0x0133); 
	}		
    delay_ms(100);   /* delay 50 ms */		
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Fill the screen as the specified color
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear( uint16_t Color)
{
	uint8_t index = 0;
	
	LCD_SetCursor(0,0); 

	LCD_WriteIndex(0x0022);
	GPIOA_PCOR = GPIO_PIN(CS); //Low
	LCD_Write_Data_Start();

	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_Write_Data_Only(Color);
	}
	GPIOA_PSOR = GPIO_PIN(CS); //High
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB To BBBBBGGGGGGRRRRR
* Input          : - color: BRG Color value  
* Output         : None
* Return         : RGB Color value
* Attention		 : None
*******************************************************************************/
static uint16_t LCD_BGR2RGB( uint16_t color)
{
	uint16_t  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : Get color value for the specified coordinates
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint( uint16_t Xpos, uint16_t Ypos)
{
	uint16_t dummy;
	
	LCD_SetCursor(Xpos,Ypos);

	LCD_WriteIndex(0x0022);  
	
	dummy = LCD_ReadData();   /* An empty read */
	dummy = LCD_ReadData(); 

	return  LCD_BGR2RGB( dummy );
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : Drawn at a specified point coordinates
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint( uint16_t Xpos, uint16_t Ypos, uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: A point line coordinates
*                  - y1: A point column coordinates 
*		   - x2: B point line coordinates
*		   - y2: B point column coordinates 
*		   - color: Line color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      /* The definition of the X Y axis increase the value of the variable */
    short temp;       

    if( x0 > x1 )     
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
    if( y0 > y1 )     
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }
  
	dx = x1-x0;       
	dy = y1-y0;       

    if( dx == 0 )     
    {
        do
        { 
            LCD_SetPoint(x0, y0, color);   
            y0++;
        }
        while( y1 >= y0 ); 
		return; 
    }
    if( dy == 0 )     
    {
        do
        {
            LCD_SetPoint(x0, y0, color);
            x0++;
        }
        while( x1 >= x0 ); 
		return;
    }
	
    if( dx > dy )                        
    {
	    temp = 2 * dy - dx;                
        while( x0 != x1 )
        {
	        LCD_SetPoint(x0,y0,color);  
	        x0++;                        
	        if( temp > 0 )               
	        {
	            y0++;                    
	            temp += 2 * dy - 2 * dx; 
	 	    }
            else         
            {
			    temp += 2 * dy;          
			}       
        }
        LCD_SetPoint(x0,y0,color);
    }  
    else
    {
	    temp = 2 * dx - dy;                     
        while( y0 != y1 )
        {
	 	    LCD_SetPoint(x0,y0,color);     
            y0++;                 
            if( temp > 0 )           
            {
                x0++;               
                temp+=2*dy-2*dx; 
            }
            else
			{
                temp += 2 * dy;
			}
        } 
        LCD_SetPoint(x0,y0,color);
	}
} 


/******************************************************************************
* Function Name  : PutChar
* Description    : Lcd screen displays a character
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - ASCI: Displayed character
*				   - charColor: Character color  
*				   - bkColor: Background color 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* get font data */
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( ((tmp_char >> (7 - j)) & 0x01) == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* Character color */
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* Background color */
            }
        }
    }
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : Displays the string
* Input          : - Xpos: Horizontal coordinate
*                  - Ypos: Vertical coordinate 
*				   - str: Displayed string
*				   - charColor: Character color   
*				   - bkColor: Background color  
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, char *str,uint16_t Color, uint16_t bkColor)
{
    char temp_char;
    do
    {
        temp_char = *str++;  
        PutChar( Xpos, Ypos, temp_char, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}
