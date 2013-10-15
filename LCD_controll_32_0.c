


 

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void 		Restart_LCD 		(void);
void 		BlankLCD 			(void);
void 		Reset_LCD 			(void);
void 		Send_LCD_comm		(int, char);
//char		Fetch_SPI			(char);
void 		Print_LCD 			(void);
void		LCD_CS_Mode 		(char);
void 		Delay 				(unsigned char);
void		LCD_DATA_Mode 		(char);
void		LCD_CLK_Mode 		(char);
void 		peep				(void);
void 		Start_Display		(void);



extern char pdata LCD_Buffer[16];	//  LCD image


void Restart_LCD ()
{
char a;
	// this rutin sent LCD restart command code
//	Reset_LCD ();
	Send_LCD_comm (0x4010,13);	  // LCD sys enable
	LCD_CS_Mode (1);	//LCD_CS	= 1;
	Send_LCD_comm (0x4290,13);	  // LCD bias
	LCD_CS_Mode (1);	//LCD_CS	= 1;
	Send_LCD_comm (0x4030,13);	  // LCD ON
	LCD_CS_Mode (1);	//LCD_CS	= 1;	

	// turn ON all segments			
	for(a=0; a<16; a++)	LCD_Buffer[a]=0xff;
	Print_LCD (); 				 	
	Delay(100);

	// turn OFF all segments
	BlankLCD ();  // clean LCD buffer
	Print_LCD ();
	Start_Display();
	peep() ;
}

//---------------------------------------
// Blank LCD display
//---------------------------------------
void BlankLCD ()	
{
char a;

	for(a=0; a<16; a++)	LCD_Buffer[a]=0x00;  // clean LCD buffer
}


//-----------------------------------------------------------------------------
// LCD_Init
//-----------------------------------------------------------------------------
void Reset_LCD (void)
{
 char a;
	LCD_CS_Mode (1);	//LCD_CS	= 1;	   
	LCD_CLK_Mode(0);		//LCD_CLK = 0;	  
	LCD_DATA_Mode(0);	//LCD_DATA=0;
	for(a=0; a<10;a++);		  // Delay
	LCD_CS_Mode (0);	//LCD_CS	= 0;
	
//=============================
// serv the LCD output
// gets b = byte to move out to serial IO
// get POS the end bit number of the byte
//=======================================

}
void Send_LCD_comm(int b, char pos)	   //b=
{
	char a, x;	
	Reset_LCD ();

	// put out 13 bits of addres and data		
	for(a=0; a<pos; a++){
						b = b<<1;	  //move the firs bit out not used
						if(b& 0x8000) LCD_DATA_Mode(1);	//LCD_DATA=1;	 //monitore the secened bit
						else LCD_DATA_Mode(0);	//LCD_DATA=0;
						for(x=0;x<10;x++);  //Delay										
						LCD_CLK_Mode(1);		//LCD_CLK = 1;   //clock the bit out, active high
						for(x=0;x<10;x++);  //Delay							
						LCD_CLK_Mode(0);		//LCD_CLK = 0;	// reset the clock 												
						}
	
}
 									
////////////////////////////////////////////////////////////////////////////////////////////////




//-----------------------------------------------------------------------------
// print LCD
//-----------------------------------------------------------------------------
//
// restart the LCD module
//---------------------------
// this rutin print on the LCD the buffer 

void Print_LCD ()
{
	char a, SEG,c, x;


	Reset_LCD ();
	Send_LCD_comm (0x5000,9);	  // write command + addres=0;

	// send to LCD data from LCD buffer
	for(a=0; a<16; a++){
						SEG=LCD_Buffer[a];	   //copy LCD buff to shift register					
						for (c=0; c<8; c++){
						 					if(SEG & 0x80) LCD_DATA_Mode(1);	//LCD_DATA=1;	 //monitorint bit
						 					else LCD_DATA_Mode(0);	//LCD_DATA=0;
											for(x=0;x<10;x++);  //Delay							 											
						 					LCD_CLK_Mode(1);		//LCD_CLK = 1;   //clock out bit out, active high
						 					for(x=0;x<10;x++);  //Delay	
						 					LCD_CLK_Mode(0);		//LCD_CLK = 0;	// reset the clock
						 					SEG = SEG<<1;
											}
						}
	LCD_CS_Mode (1);	//LCD_CS	= 1;	 // close LCD
	
}