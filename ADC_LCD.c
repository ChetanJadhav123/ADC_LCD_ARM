//******************************************************************************
// FileName					: ADC and LCD Programming
// Microcontroller	: LPC2148
// Compiler					: Keil v-4
// Target Hardware	: ARM7 Development Board
// Company					: V V Technologies, Tumkur (3rd Floor, C-qube Central, inbetween 13th and 14th cross, SIT-Main road, Tumkur)
// Description 			:	Analog Signals are Converted into Digital form and result printed in LCD

// Pin Connection   :	LCD dataline (D0 to D7) connected to Port-1 pins of P1.16 to P1.23 respectively and 
//										Rs(Register Select) pin of LCD connected to P1.24 and 
//										En(Enable) pin of LCD connected to P1.25
//										R/W pin of LCD always ground

//										ADC channel Connected to Sensors or Resistor Pot. ADC cahnnels are - (P0.30, P0.29, P0,28, P0.25)
//******************************************************************************



#include<lpc214x.h>
#include<string.h>

void delay(void);
void LCD_init(void);
void LCD_cmd(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_string(unsigned char *ptr);

void adc_channel_select(unsigned int channel);
void adc(void);
void hex_dec(unsigned int lcd_data);


//********************* MAIN Program ******************//

int main()
{
	PINSEL0 = 0x00000000;							// P0.0 to P0.15 configured as GPIO	
	PINSEL1 = 0x15040000;							// ADC cinfigure (P0.30, P0.29, P0,28, P0.25), remaining as GPIO
	PINSEL2 = 0x00000000;							// P1.0 to P1.31 configured as GPIO	
		
	IODIR1 = 0x03FF0000;							// P1.16 to P1.25 configured as 0utput port

	LCD_init();												// LCD-init function calling
	
	LCD_cmd(0x01);										// clear the display
	LCD_cmd(0x80);										// LCD row one selection
	LCD_string("*** Welcome to ***");	// sting of data sent
	LCD_cmd(0xC0);										// LCD row second selection
	LCD_string("*V V Tech*");					// sting of data sent
	
	while(1)
	{
		adc();													// adc function calling
	}
}

// ******************** END of MAIN Program *********************//

// ******************** DELAY Program *********************//

void delay()
{
	unsigned int i, j;
	
	for(i = 0 ; i < 1000 ; i++)
		for(j = 0 ; j < 1000 ; j++);
}

// ******************** END of DELAY Program *********************//


// ******************** LCD initilization Program *********************//
void LCD_init()
{
	LCD_cmd(0x38);							//selection of the 8-data lines, 0x28 for 4-bit data
	LCD_cmd(0x0C);							//Display ON cursor OFF
	LCD_cmd(0x06);							//move the cursor right
	LCD_cmd(0x01);							//clear the LCD
	LCD_cmd(0x80);							//firat line starting address
}
// ******************** END of LCD-init Program *********************//

// ******************** LCD CMD Program *********************//
void LCD_cmd(unsigned char cmd)
{
	IOCLR1 = 0x03FF0000;										// P1.16 to P1.25 port pin made as low for clearing the data
	IOSET1 = cmd << 18 | 0x00020000;				// cmd varible content data hasbeen shifted 16-times and ORed with 0x02000000 and result copied to the IOSET1 reg.
	delay();
	IOCLR1 = 0x03FF0000;										// P1.16 to P1.25 port pin made as low for clearing the data
}

// ******************** END of LCD CMD Program *********************//


// ******************** LCD data Program *********************//
void LCD_data(unsigned char data)
{
	IOCLR1 = 0x03FF0000;											// P1.16 to P1.25 port pin made as low for clearing the data
	IOSET1 = data << 18 | 0x00030000;					// data varible content data hasbeen shifted 16-times and ORed with 0x03000000 and result copied to the IOSET1 reg.
	delay();
	IOCLR1 = 0x03FF0000;											// P1.16 to P1.25 port pin made as low for clearing the data
}
// ******************** END of LCD data Program *********************//

// ******************** LCD string Program *********************//

void LCD_string(unsigned char *ptr)
{
	while(*ptr)
		LCD_data(*ptr++);
}
// ******************** END of LCD string Program *********************//


// ******************** ADC channel selection sub Program *********************//

void adc_channel_select(unsigned int channel)
{
	switch(channel)
	{
		case 2:AD0CR=0x01200402;		//channel 2 selecting
				break;

		case 3:AD0CR=0x01200404;		//channel 3 selecting
				break;

		case 4:AD0CR=0x01200408;		//channel 4 selecting
				break;
		
		default:break;	
	}	
}

// ******************** END of ADC chan sub Program *********************//

// ******************** ADC Program *********************//
void adc()
{
	unsigned int temp, Cuurent, gas;												// Variables
	

	LCD_cmd(0x01);															// clear the display
	
	LCD_cmd(0x80);															// LCD row one selection
	LCD_string("Temp:");													// printing in LCD as S_1:
	adc_channel_select(2);											// selection of ADC channel-2
	while(!(AD0GDR&0x80000000));								// checking for DONE bit	high or not	
	temp = (AD0GDR & 0x0000FFC0) >> 6;							// take the result value from AD0GDR, Masking, Right shift 6 times and store it in variable
	hex_dec(temp);																	// Convert result value into ASCII/ decimal Value
	delay();

	LCD_cmd(0xC0);															// LCD second row selection
	LCD_string("Light:");
	adc_channel_select(3);											//selection of ADC channel 3
	while(!(AD0GDR&0x80000000));								//checking for DONE bit	high or not	
	Cuurent = (AD0GDR & 0x0000FFC0) >> 6;							//take the result value from AD0GDR, Masking, Right shift 6 times and store it in variable
	hex_dec(Cuurent);																	//Convert result value into ASCII/ decimal Value
	delay();

//	LCD_cmd(0x94);															// LCD third row selection
//	LCD_string("Gas:");
//	adc_channel_select(4);											//selection of ADC channel 3
//	while(!(AD0GDR&0x80000000));								//checking for DONE bit		
//	gas = (AD0GDR & 0x0000FFC0) >> 6;							//take the result value from AD0GDR, Masking, Right shift 6 times and store it in variable
//	hex_dec(gas);																	//Convert result value into ASCII/ decimal Value
//	delay();
//	
	
//	if(temp>45)
//	{
//		LCD_cmd(0xD4);
//		LCD_string("Temperature is high ");
//	}
//	else
//	{
//		LCD_cmd(0xD4);
//		LCD_string("                    ");
//	}
//	
//	if(ldr<250)
//	{
//		LCD_cmd(0xD4);
//		LCD_string("Light Intensity Low ");
//	}
//	else
//	{
//		LCD_cmd(0xD4);
//		LCD_string("                    ");
//	}
//	
//	if(gas>750)
//	{
//		LCD_cmd(0xD4);
//		LCD_string("   Gas Detected    ");
//	}
//	else
//	{
//		LCD_cmd(0xD4);
//		LCD_string("                    ");
//	}
	
	
}
// ******************** END of ADC Program *********************//	

// ******************** CONVERTION Program *********************//

void hex_dec(unsigned int lcd_data)	//Hex to decimal conversion
{
	unsigned int d1,d2,d3,d4;

	d1 = lcd_data % 0x0a;									//Modulus operation, reminder value stored in variable
	lcd_data = lcd_data / 0x0a;		       	//Division operation, quotient value stored in the variable

	d2 = lcd_data % 0x0a;
	lcd_data = lcd_data / 0x0a;

	d3 = lcd_data % 0x0a;
	lcd_data = lcd_data / 0x0a;

	d4 = lcd_data % 0x0a;

	LCD_data(d4+0x30);			//Data to be displayed on LCD
	LCD_data(d3+0x30);			//Data to be displayed on LCD
	LCD_data(d2+0x30);			//Data to be displayed on LCD
	LCD_data(d1+0x30);			//Data to be displayed on LCD
}
// ******************** END of CONVERTION Program *********************//

