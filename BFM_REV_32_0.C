 
////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////			//	 
            //VASA APPLIED THECHNOLOGIES LTD. 
			// (BFM) 
			 //// MASTER CPU SOFTWARE
			 //////////////////////////////////////////////////////////////////////////////////
			 //////////////////////////////////////////////////////////////////////////////////
			 ////////////////////FIRST CONSEPT	//////////////////////////////////////////
			 ////////////////////
			 ////////////////////    rev 1.0		///////////////////////////////////////////
			 ///////////////////     28.12.2011		/////////////////////////////////////////
			 /////////////////////////////////////////////////////////////////////////////////
			 /////////////////////////////////////////////////////////////////////////////////

// Target:         C8051F320
// Tool chain:     Keil C51 
// Command Line:   None
//
// Release 1.0
//    -Initial Revision 
//    -28 dec 2011
//	   Rafael halfon
 
//.........................................
//***************************
// LAST UPDATE 12/08/13  TIME: 18:00
// BY RAFAEL HALFON
//***************************
// NOT WORKING WILL CHRAGING

//rev 31.1 09/4/2013
// add test mode
//	BUG  FIXED
//:::::::::::::::::::::::::::::::
// READ WRIT TO LOCAL MEMORY
// Seting the monitor   17/8/2012
//
// check battrey ()
//
			
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f320.h>                 // SFR declarations
//#include <stdio.h>
//#include <string.h>
#include <LCD_controll_32_0.c>
#include <Memory_mangment_32_0.c>
#include <rtc_controll_32_0.c>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F31x
//-----------------------------------------------------------------------------
sfr16 ADC0 = 0xBD;                     // 10-bit ADC0 result

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------


#define REV1  		'M'
#define	REV2 		'0'
#define	REV3 		'4'
///////////////////////////////

#define	Truth 	1
#define	False	0
#define AckChar	'q'



//#define Blevl_1	750
//#define Blevl_2	730


///////////////////////////////////////


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------


int 	atoi				(void *string);
//void	Check_LCD 		(void);

void 	Delay 				(unsigned char);
void 	PORT_Init 			(void);
void 	SPI0_Init 			(void);

//void 	Start_LCD		(void);
void 	Send_LCD_comm		(int,char);

void 	Timer_Init			(void);

void 	UART0_Init 			(void);
//void 	Monitor_Reset 	(void);
//void 	LCD_Delay		(void);

void 	Print_LCD 			(void);
char 	Num_To_Seg 			(char);
char 	Num_To_Seg_Lower 	(char,char);
void	Restart_LCD	   		(void);
void 	SEG_care 			(char,char);
char 	KeyPad 				(void);

void 	ADC0_Init 			(void);
void  	CheckInput 			(void);
void 	Sent_Ack			(void);
void 	GO_OUT				(char,char);
//void 	LCD_OUT 			(void);
void 	Reset_LCD 			(void);
void	Check_Menu			(void);

//void 	Print_SetupScreen  	(void);
//void 	Print_Info_Screen	(void);

void 	Tirgom 				(unsigned int);
void	Light_Intensity		(char);

//void	InputValueFromKeyPad(void);
//int 	TranslatValue 		(void);
//void	CleanScreen 		(void);
void 	peep				(void);
void 	Longpeep			(void);
void	Reset_Prob 			(void);
void 	Error 				(char);
void 	ChackValidProb 		(void);
void 	Get_Thr 			(void);
void 	ChackSoftProb 		(void);
char	Sensor_CheckUp		(void);
void	BlankLCD 			(void);
void 	Metering_ON 		(void);
void 	WarmUPSensor 		(void);
//void 	SetSnake 		(void);
void	Turn_off_Monitor 	(void);
void 	ChackBattry 		(void);
void 	ChackUSB 			(void);
//---------------------------------------

void 	Get_Date_Time		(void);
void 	Start_Display		(void);
char	Fetch_SPI			(char);
//void 	Update_TimeDate 	(void);
void	Update_RTC 			(void);
void 	PrintLowCharSegment	(void);
void 	SetUpperFigers		(void);					  
void 	GetUpperFigers		(void);
void 	PCA_Init			(void);
//void 	SelectBaby			(void);
void 	Display_Last_24hr 	(void);
void	Cal_Last_24hr	 	(void);
void 	UpdateSirealNum		(void);
void 	PreMetering 		(void);
void 	EndMetering 		(void);


//----------------------------------------------------

//    MEMORY

//void 	MemAddControl		(char);
//void 	ReadMemoryBasic		(void);
//void 	SentMemToTerminal 	(void);
//char 	Memory_Redy			(void);
//void 	ReadMemory			(void);
//void 	UpdateBasicMem		(void);
//void	Write_SetionTo_Mem	(void);
//void	DisplayMemory 		(void);
//char	GetNextAdd			(void);
//void 	History				(void);
//void 	Open_Memory 		(void);
//void 	Close_Memory 		(void);
//void 	SentToTherminal		(void);

//----------------------------------------------------------------            
// Global Variables
//-------------------------------------------------------------------------	
char BuffLength, CursorClock, Segment=0, Scalibration, ClockQ;
unsigned char HtrPwr, PowerSaveCunter, DelayT;//, LightDisplay;

//unsigned int DelayT = 0;
int LightDisplay;
unsigned int MainThrBuff, CompThrBuff, OverDiff; //thermistor	 values

unsigned int BattryLevel;
unsigned int Volume ;
unsigned long Sub_Volume;
//-------------------------------------------------------------------------
// Global Flages
//-------------------------------------------------------------------------
char Feed_Time, FeedSide, Units, PrevKey, ChargeMode, OK_Flag=0;		//, memory_24_flag=0;

char Pos, OutFlag, InFlag, ProbOK, SoftRevOK, 	
	 Menu_Mode, ACK_Flag, NumOfBabies, Baby=1;
int	 Clock, VolumeOf_24H; 
char SlaveRev[3];
char SlvEndOfTran, FlowFlag, MeterOnFlag, StabeTime, StabeFlag, SensorWatchDog;
char StartTimeStatus, MemoryReadCunter, HistoryMode;
//-------------------------------------------------------------------------
// Global BITS
//-------------------------------------------------------------------------
bit Cursor=0;
//-----------------------------------------------
//          ARRAYS
//----------------------------------------------- 

char pdata Comm_Buffer[14];	//input buffer 
char pdata BuffZ [4];		// translation buffer
char pdata LCD_Buffer[16];	//  LCD image
int  pdata ThrmDiffBuff[3];	
char pdata DateTime[10];	//={8,5,3,2,7,5,2,1,1,3};		// m,10m,H,10H,Y,10Y,M,10M,D,10D
char pdata GenBuff[6];
char pdata BurthDate[6];  //	={7,5,1,1,1,2};

//--------------------------------------------------
// SET PORT PINS
//------------------------------------------------
			// PORT 0

//sbit 	SPI CLK		=	P0^0;	// SPI CLOCK
//sbit 	SPI MISO	= 	P0^1;	// SPI MASTER IN 
//sbit	SPI_MOSI	=	P0^2;	// SPI MASTER OUT
//sbit  CEX0		=	P0^3;	// display ilumintion
//sbit 	TX			=	P0^4;	// UART TRANSMITION
//sbit	RX			=	P0^5;	// UART RECIVER
//sbit	unused		=	P0^6;	// unused
sbit	WACT_CS		=	P0^7;	// WACT_CS


			// PORT 1

sbit	Kpad_7	=	P1^0;	// KEY PAD 7
sbit	Kpad_2 	=	P1^1;	// KEY PAD 2
sbit	Kpad_3 	=	P1^2;	// KEY PAD 3
sbit	Kpad_4 	=	P1^3;	// KEY PAD 4
sbit	Kpad_5 	=	P1^4;	// KEY PAD 5
sbit	Kpad_6	=	P1^5;	// KEY PAD 6 
sbit	USB_ON 	=	P1^6;	// USB active flag
//sbit	spare 	=	P1^7;	// spare

			//PORT 2

//sbit 	BATTARY		=	P2^0;	// BATTARY SENSOR
sbit	Mem_CS		=	P2^1;	// Memory CS
sbit 	SW_OFF		=	P2^2;	// TURN OFF THE monitor
sbit	SWICTH		=	P2^3;	// ON/OFF SWITCH input
sbit	Buzzr		=	P2^4;	// Buzzr
sbit	LCD_CLK		=	P2^5;	// LCD_clk
sbit 	LCD_DATA 	=	P2^6;	// LCD_DATA  
sbit	LCD_CS 		=	P2^7;	// LCD_CS 

		//PORT 3

//sbit C2DAT		= 	P3^0;	// FOR LOAD SOFTWARE 




////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void main (void)
{
int a;

	EA = 0;
	PCA0MD = 0x00;   		// WDTE = watchdog Disenable)


	// Initialize Internal Oscillator//
	//----------------------------
	REG0CN    = 0x80;  //voltage regolator not active 	

    CLKMUL    = 0x80;
    for (a = 0; a < 20; a++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    CLKSEL    = 0x02;
 	OSCICN    = 0x83;

    VDM0CN    = 0x80;
    for (a = 0; a < 350; a++);  // Wait 100us for 

	RSTSRC  = 0x04;         // Enable missing clock detector

	PORT_Init();         	// Initialize Port I/O 	
	UART0_Init();			// Initialize UART0
	ADC0_Init ();		  	// Initialize ANALOG TO DIGIDAL	 
	SPI0_Init();			// Initialize SPI
	Timer_Init();			// Initialize Timer0
	PCA_Init();				// Initialize PCA for display light control

//////--------------------
/// INTERAPT ENABLE AND PRIORITY
//////------------------------------

	IE		= 0x30;		//enable interapt of:  TIMER2, UART, 
	IP		= 0x30;		//SET PRIORITY FOR:	 TIMER2, UART
	EIE1	= 0x08;		//extended interrupt for ADC, SMBUS  0x09
//	EIP1	= 0x01;		//set SMBUS priority
//	IE		= 0xf2;
	EA = 1;				// enable global interrupts	
//////////////////////////////////////////
//	MemoryReadCunter=1;	  // the firs record on memory is 1
	Feed_Time=0;
	FeedSide=0;
	Pos=0;
	OutFlag=0;
	InFlag=0;
//	More_then_100_flag=0;
	Menu_Mode=0;
	Units =0;
	ACK_Flag=0;
//	SecClock=0;
//	MinClock=0;
	Clock=0;	  //**********************************
	NumOfBabies=1;
	SlvEndOfTran=0;
	FlowFlag=0;
	MeterOnFlag=0;
	SensorWatchDog=0;
	Volume=0;
	Sub_Volume=0;
	HtrPwr=65;
	PowerSaveCunter=0;
	HistoryMode=0;
	LightDisplay=125;

// printing to RS232 '$MR%'	
	SBUF0 = '$';
	Delay (2);
	SBUF0 = 'M';
	Delay (2);
	SBUF0 = 'R';
	Delay (2);
	SBUF0 = '%';
	Delay (2);
//	Led=0;
//	WACT_CS=0;
	WACT_CS=1;		 // open SPI RTc chanell
	Fetch_SPI (0x58);	 // reset rtc
	Fetch_SPI (0x10);
	WACT_CS=0;
	Get_Date_Time();
	Restart_LCD();
	Reset_Prob ();	
	ReadMemoryBasic ();
//	ChargeMode=0;
	ChackBattry ();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************************************************
// 			E X E C U T I V E *********************************
//***************************************************************

  	while(1){
//			 PCA0CPH4 = 0xFF;		// set WD interval 
			 if(SWICTH) Turn_off_Monitor();			 
			 if (InFlag == 5) InFlag = 0, CheckInput();
			 if(ChargeMode==1){
			 				/// for chargin sign on the dark LCD				 				 			 					
			 					if(ClockQ > 40){		  // timing 40 = 0.5 secend
												if(a++ > 3)a=0;
												ClockQ=0;			 								 	
										 	 	BlankLCD (); 	 // clean LCD buffer								
										  	 	SEG_care (6,1);									
										 	 	if(a==1) SEG_care (9,1);	
										 	 	if(a==2) SEG_care (9,1), SEG_care (8,1);	
										 	 	if(a==3) SEG_care (9,1), SEG_care (8,1), SEG_care (7,1);
										 	 	Print_LCD ();
											}
								 }
			 	else Check_Menu();
			 SetUpperFigers ();
			 ChackUSB();

	//		 if(Menu_Mode==0 |Menu_Mode==10) Get_Date_Time();
			
//====================================================================
///  C L O C K  	
//===================================================

	
	if(Clock > 400){	 // 400 ==> 5 seconds
						if((Menu_Mode==0 || Menu_Mode==10) & !HistoryMode )  Get_Date_Time();
																			 						
						Clock=0;			
						ChackBattry ();

						// counting feed time and memories the begining minute
						if((DateTime[0] != StartTimeStatus) && MeterOnFlag){
																			Feed_Time++;
																			StartTimeStatus = DateTime[0];
																			PowerSaveCunter=0;			   // reset power save counter
																			}
						if(PowerSaveCunter++ >120 & ChargeMode==0 ){					 //  120===> 10 MINUTE
												 					Longpeep();						
						 						 					SW_OFF=0;  //Turn_off_Monitor 
												 				}
				
					}
	if(Cursor)SensorWatchDog++;
	if(SensorWatchDog > 5 && MeterOnFlag) Error(8);			//PROBE DISCONNECTED
		}
}		
  //-----------------------------------
 /////  END MAIN ROUTIN
 //----------------------------------

/////-------------------------------------------------------------------
/////           GLOBAL ROUTINE
////------------------------------------------------------------------------

	

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
// Configure the Crossbar and GPIO ports.
//
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{   
///			PORT  0
// 	P0MDIN 	= 0xff;		//configure pin 	 default=1	1=digital	0=analog	
//	P0	   	= 0;		//configure pin as logic high			
	P0MDOUT = 0x9d;		//configure pin as puhspull	 default=0  1 for pushpull 0 opne drain


//			PORT 1

//	P1MDIN 	= 0xff;		//configure pin as digital			digital 	
//	P1	   	= 0;		//configure pin as logic high		default=1
//	P1MDOUT = 0xe0;		//configure pin as puhspull			default=0 1= pusepull
//	P0SKIP	= 0;		//configure pin to spishel signal	default=0

//			PORT  2
			
	P2MDIN 	= 0xfe;		//configure pin 2-0 AS ANALOG INPUT
//	P2	   	= 0;		//configure pin as logic high		
	P2MDOUT = 0xf6;		//configure pin as puhspull=1			
	P2SKIP 	= 0x01;		//skip pins for analog signal	
			
//			PORT 3

//	P3MDIN 	= 0xff;		//for LCD	
//	P3	   	= 0;		//configure pin as logic high			
//	P3MDOUT = 0x1a;		//configure pins as puhspull	
 
  	XBR0    = 0x03;                     // Enable UART on P0.4(TX) and P0.5(RX)
										// & SPI   P0.0=CLK 	P0.1=MISO	P0.2=MOSI
									
   	XBR1    = 0xc1;                     // Enable crossbar & disable weak pull-ups +ENABLE PWM

}




///------------------------------------------------------------
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
	SCON0 	= 0x10;		// 8-bit variable bit rate
// 	TCON 	= 0x40;		// turen on timer 1
//	TMOD 	= 0x20;     // timer 1 in 8-bit autoreload 
//	TH1		= 0x2b;  	// baud rate 4800
}


//-----------------------------------------------------------------------------
// Configures ADC0 to make single-ended analog measurements on Port 1 according
// to the values of <ANALOG_INPUTS> .
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   	ADC0CN = 0xC0;                  // ADC0 enable SAMPLE ON ADC0BUSY 	
	ADC0CF = 0xC0;					// Set SAR clock to 1MHz
	AMX0N = 0x1F;                   // ADC0 negative input = GND
    AMX0P = 0x08;	                // battery pin
	REF0CN = 0x08;                  //  VREF = VDD
}




//-----------------------------------------------------------------------------
// Configures SPI0 to use 3-wire Single Master mode. The SPI timing is
// configured for Mode 0,0 (data centered on first edge of clock phase and
// SCK line low in idle state).
//
//-----------------------------------------------------------------------------
void SPI0_Init(void)
{
	SPI0CFG   = 0x40;	// Enable the SPI as a Master MODE 0
                        // CKPHA = '0', CKPOL = '0'
	SPI0CN    = 0x01;   // 3-wire Single Master, SPI enabled
	SPI0CKR   =	0x03;	// spi frequency 1MHz=0x0b   0x03=3MHz
}

//-----------------------------------------------------------------------------
// Timer_Init
//-----------------------------------------------------------------------------

void Timer_Init(void)
{

//	TH0 	= 0xcf; 	//timer set for LCD 250KHZ	
	TH1		= 0x98;  	// baud rate 4800=0x2b///  baud rate 9600 =0x98 for 320	 4800 = 0x30
   	TMOD 	= 0x22;    	// Timer0 in 8-bit mode	& Timer1 in 8-bit mode
   	CKCON 	= 0x04;    	// Timer0 uses system clkc  	

  	TMR2RLH = 0x9b; 	// set to 40Hz . 25mS
 	TMR2CN  = 0x04;     // Enable Timer2 in auto-reload mode

//	TH0 = 0xf6; //0xf6;	//=100uS			//0xfd;	= 25uS
//	TL0 = 0x69;	//=100uS			        //0xab;	= 25uS


	TR0	= 0;			// turn on timer 0
	TR1 = 1;			// turn on timer 1


}



//-----------------------------------------------------------------------------
// PMW configortion
//-----------------------------------------------------------------------------

void PCA_Init(void)
{
    
    PCA0CN    	= 0x40;	  		//PCA Enable
	PCA0MD 		= 0x80;   		// WDTE = watchdog Disenable)
    PCA0CPM0  	= 0x42;			// pulse width modulation enable //0x42
    PCA0CPH0  	= 125;			// pwm=5/255

//    PCA0CN     =  0x40;         //****		// PCA counter enable
 //   PCA0MD    &= ~0x40 ;        //****		// Watchdog timer disabled-clearing bit 6
//    PCA0MD    &=  0xF1;        	 //****	// timebase selected - System clock / 12
//    PCA0CPL4   =  0xFF;        	 //****	// Offset value


//	PCA0MD  &= ~0x40;         //****      // WDTE = 0 (clear watchdog timer
                               //****      // enable)
//	PCA0L    = 0x00;            //****    	// Set lower byte of PCA counter to 0  
//	PCA0H    = 0x00;            //****    	// Set higher byte of PCA counter to 0
//	PCA0CPL4 = 0xFF;            //****    	// Write offset for the WDT 
//	PCA0MD  |= 0x40;            //****    	// enable the WDT
				    	
	

}

/////////////////////////////////////////////////////////////////// 

 
//---------------------------------------
// delay subroutine for 250uS
//---------------------------------------
void Delay (unsigned char z)
{
	DelayT = z;
	while (DelayT > 1) ;
		

}



//---------------------------------------
// Get Start Display 
//---------------------------------------
void Start_Display()
{
	Feed_Time=0;
	Volume=0;
//	FeedSide=0;
	// units
	if(Units)SEG_care (10,1);	  //fl+oz ON
		else SEG_care (12,1);	  //ml ON

	// bobs side
	SEG_care (2,1);	  //R ON
	SEG_care (4,1);	  //L ON
	FeedSide=0;
	SEG_care (1,1);	  //[] R ON
	SEG_care (3,0);	  //[]L OFF


	SEG_care(6,1);	   //battry simbol ON
	Get_Date_Time();

	SetUpperFigers();
	SEG_care(5,1);	   //decimal point

	SEG_care (19,0);	 // birth segment OFF
//	Cursor=0;
	SEG_care (21,0);  //setup OFF
	ChackBattry();


}



//---------------------------------------------
//	 Chack Battry

//---------------------------------------------

void ChackBattry ()
{
	SEG_care (6,1);
	AD0BUSY =1;
	while (AD0INT);
//	if(USB_ON) BattryLevel=800;
// FACTOR (3.2 / 1000)X2			
	if(BattryLevel >= 730){			  // > 4.67 volt
							SEG_care (7,1);
							SEG_care (8,1);
							SEG_care (9,1);
							}
	if(BattryLevel < 730){				// < 4.5 volt	
			  				SEG_care (7,0);
			  				SEG_care (8,1);
			  				SEG_care (9,1);
						}
	if(BattryLevel < 600){			// < 3.9 volt		
			  				SEG_care (7,0);
			  				SEG_care (8,0);
			  				SEG_care (9,1);
										}
	if(BattryLevel < 550){			// < 3.5 volt		
							SEG_care (7,0);
							SEG_care (8,0);
							SEG_care (9,0);
							Longpeep();									
							SW_OFF = 0;						  
						}
	ChargeMode=0;
	if(BattryLevel > 860){
							if(USB_ON) ChargeMode=2;
							else{
									ChargeMode=1;
							 		PCA0CPM0  = 0x40;	//TURN OFF LCD
							 		BlankLCD ();
							 		Print_LCD ();
								}

						  }	 
//	if(BattryLevel > 860 & !USB_ON){ 	
//									PCA0CPM0  = 0x40;	//TURN OFF LCD 						  
//									ClockQ = 100;
//									ChargeMode=2;
//									}
	if(!ChargeMode){
			 		PCA0CPM0  = 0x42;	  //TURN ON LCD
			 		PCA0CPH0  = 125;
//			 		ChargeMode=0;
			 		}											
			   							
 }

///////////////////////////////////////////////////
//translate numbers to 7 segment UPER numberse
///////////////////////////////////////

char Num_To_Seg (char num)
{
char SEG;

//	if(!Cursor & FigureID==Pos &CursorON) SEG=0x10;	//cursor
//	else{	
		switch(num){
					case 0: SEG=0xd7; break;																			
					case 1:	SEG=0x06; break;
					case 2:	SEG=0xe3; break;
					case 3:	SEG=0xa7; break;																			
					case 4:	SEG=0x36; break;
					case 5:	SEG=0xb5; break;
					case 6:	SEG=0xf5; break;																			
					case 7:	SEG=0x07; break;
					case 8:	SEG=0xf7; break;
					case 9:	SEG=0xb7; break;
					case 99:SEG=0x00; break;
					
					case 11: SEG=0xf1; break;  //E
					case 12: SEG=0x60; break;  //r
			//		case 13: SEG=0xe0; break;  // L
			//		case 14: SEG=0xc6; break; //o 			
					default: SEG=0xd7;
				}
//		 }
	return(SEG);										   
	
}



///////////////////////////////////////////////////
//translate numbers to 7 segment big numbers
///////////////////////////////////////

char Num_To_Seg_Lower (char num, char Pos)
{
char SEG;

	if((Pos==Segment) && Cursor) SEG=0x00;	//cursor
	else{
	
		switch(num){
					case 0:	SEG=0x5f; break;
					case 1:	SEG=0x50; break;																			
					case 2:	SEG=0x6b; break;
					case 3:	SEG=0x79; break;
					case 4:	SEG=0x74; break;																			
					case 5:	SEG=0x3d; break;
					case 6:	SEG=0x3f; break;
					case 7:	SEG=0x58; break;																			
					case 8:	SEG=0x7f; break;
					case 9:	SEG=0x7d; break;										
					case 20:SEG=0x36; break;   //h
					case 21:SEG=0x22; break;   //r
					case 99:SEG=0x00; break;
					default: SEG=0x5f;				
					}
	  }
	return(SEG);										   
				
				
}

void SEG_care (char Dcod,char ON)
{
char a;

	switch(Dcod){
						
				case 1:	a =	LCD_Buffer[0];
				 		if(ON) LCD_Buffer[0] = a |0x08;	 //R[] T6
							else LCD_Buffer[0] = a & 0xf7;							
				 		break; 
						 
				case 2: a =	LCD_Buffer[1];
						if(ON)LCD_Buffer[1] = a|0x08;	 //R R
							else LCD_Buffer[1] = a &0xf7;
				 		break;
						  																			
				case 3:	a =	LCD_Buffer[2];
				 		if(ON)LCD_Buffer[2] = a|0x08;	 //L [] T5
							else LCD_Buffer[2] = a &0xf7;
				 		break;
						  
				case 4:	a =	LCD_Buffer[3];
				 		if(ON)LCD_Buffer[3] = a|0x08;	 //L L
							else LCD_Buffer[3] = a&0xf7;
				 		break;
				
				case 5: a =	LCD_Buffer[4];
						if(ON) LCD_Buffer[4] = a|0x08;	 //decimal point uper P1
							else LCD_Buffer[4] = a&0xf7;
				 		break;
 //--------------------------------------------------------------------------
						  
				case 6: a = LCD_Buffer[5];
						 if(ON) LCD_Buffer[5] = a|0x01;	 // battrey	T1
							else LCD_Buffer[5] = a &0xfe;				
				 		break; 				
				 
				case 7: a = LCD_Buffer[5];
						 if(ON)LCD_Buffer[5] = a|0x02;  // full battrey	   T2
							else LCD_Buffer[5] = a&0xfd;
				 		 break;

				case 8: a = LCD_Buffer[5];
				 		if(ON) LCD_Buffer[5] = a|0x04; // medume battery	  T3
							else LCD_Buffer[5] = a&0xfb;
						 break;

				case 9: a = LCD_Buffer[5];
				 		 if(ON) LCD_Buffer[5] = a|0x08;  // Low battery T4
				 		 	else LCD_Buffer[5] = a &0xf7;
						 break;

				case 10: a = LCD_Buffer[5];
				 		 if(ON) LCD_Buffer[5] = a|0x30;  //fl+oz	 S10+S9
				 		   	else LCD_Buffer[5] = a &0xcf;
						 break;

//				case 11: a = LCD_Buffer[5];
//				 		 if(ON) LCD_Buffer[5] = a|0x20;  //oz	 S9
//				 		   	else LCD_Buffer[5] = a &0xdf;
//						 break;			
				
				case 12:a = LCD_Buffer[5];
				 		if(ON) LCD_Buffer[5] = a|0x40;  //ml	S8
				 		  	else LCD_Buffer[5] = a& 0xbf;
						 break;
				case 13: a = LCD_Buffer[5];
						if(ON) LCD_Buffer[5] = a|0x80;  //Units 	S7
						  	else LCD_Buffer[5] = a & 0x7f;
						 break;

  //------------------------------------------------------------
				case 14: a = LCD_Buffer[6];
						 if(ON) LCD_Buffer[6] = a|0x80;  //USB P5
				 		  	else LCD_Buffer[6] = a&0x7F;
						 break;

				case 15: a = LCD_Buffer[7];
				 		 if(ON) LCD_Buffer[7] = a|0x80;  //Time S6
				 		  	else LCD_Buffer[7] = a&0x7F;
						 break;
				case 16: a = LCD_Buffer[8];
						 if(ON) LCD_Buffer[8] = a|0x80; //coln  P4
						  	else LCD_Buffer[8] = a&0x7F; 
						 break;

				case 17: a = LCD_Buffer[9];
						 if(ON) LCD_Buffer[9] = a|0x80; //Babies  S5
						  	else LCD_Buffer[9] = a&0x7f; 
						 break;

				case 18: a = LCD_Buffer[10];
						 if(ON) LCD_Buffer[10] = a|0x80; //Date S4
						  	else LCD_Buffer[10] = a&0x7f; 
						 break;

				case 19: a = LCD_Buffer[11];
						 if(ON) LCD_Buffer[11] = a|0x80; //Birth  S3
						  	else LCD_Buffer[11] = a&0x7f; 
						 break;
		
				case 20: a = LCD_Buffer[12];
						 if(ON) LCD_Buffer[12] = a|0x80; //point P3
						  	else LCD_Buffer[12] = a&0xfe; 
						 break;

				case 21: a = LCD_Buffer[13];
						 if(ON) LCD_Buffer[13] = a|0x80; //Setup  S2
						  	else LCD_Buffer[13] = a&0x7f; 
						 break;

				case 22: a = LCD_Buffer[14];
						 if(ON) LCD_Buffer[14] = a|0x80; //point P2
						  	else LCD_Buffer[14] = a&0x7f; 
						 break;

				case 23: a = LCD_Buffer[15];
						 if(ON) LCD_Buffer[15] = a|0x80; //Time S1
						  	else LCD_Buffer[15] = a&0x7f; 
						 break;	
					
		
				}

}

	
//-----------------------------------------------------------------------------
// BUZZER PEEPS SORT  
//-----------------------------------------------------------------------------
 void peep() 
 {
int a, b;


	for(a=0; a<200 ;a++){						 //200
                     	for(b=0;b<300;b++);		//200				
						Buzzr = ~Buzzr;	  		  
				     }
	Buzzr =0;
  }
	
//-----------------------------------------------------------------------------
// BUZZER PEEPS LONG  
//-----------------------------------------------------------------------------

  void Longpeep() 
 {

int b,a;

	for(a=0; a<1000 ;a++){						 //1000
                     	for(b=0;b<300;b++);		//200					
						Buzzr = ~Buzzr;	  		  
				     }
	Buzzr =0;
  }

	
//-----------------------------------------------------------------------------
// Pre Metering 
//-----------------------------------------------------------------------------

void PreMetering ()
{
	
 	Feed_Time=0;
	Sub_Volume=0;
	Volume=0;
	
	if(FeedSide) SEG_care (3,1);  //L [] on
		else SEG_care (1,1);	  //R [] on
	SEG_care (2,1);
	SEG_care (4,1);

	Print_LCD();
	OK_Flag=1;											 		 
	if(NumOfBabies==1){													 			 																											
						if(Sensor_CheckUp()){
												WarmUPSensor ();																							 														
												Menu_Mode=10;
											}
							else  Menu_Mode=12;
					}
}

	
//-----------------------------------------------------------------------------
// End of Metering 
//-----------------------------------------------------------------------------

void EndMetering ()
{
	Reset_Prob ();
	MeterOnFlag=0;
	Menu_Mode=0;											
	Write_SetionTo_Mem();
	SEG_care (3,0);	  // [] off
	SEG_care (1,0);	  // [] off
//	SEG_care (2,1);
//	SEG_care (4,1);
//	Print_LCD();
	OK_Flag=0;
	}	
//-----------------------------------------------------------------------------
// Key Pad 
//-----------------------------------------------------------------------------

char KeyPad ()

{
char INKey=99;				// RESET KEY BUFFER TO Z


	if(Kpad_2)INKey = '1';		 // LEFT 
	if(Kpad_3)INKey = '2';		//OK
	if(Kpad_4)INKey = '3';		// SET UP
	if(Kpad_5)INKey = '4';		// DOWN
	if(Kpad_6)INKey = '5';		 // UP
	if(Kpad_7)INKey = '6';		 // RIGTH 
	if(INKey == PrevKey ) INKey=99 ;
		else PrevKey=INKey;
	
	if(INKey != 99)	PowerSaveCunter=0;
							
	return (INKey);
}
////////////////////////////////////////////////
	
void Check_Menu	()	
{
char INKey,a;
	INKey = KeyPad ();

	if(Menu_Mode==33){
						if(INKey != 99){
										Comm_Buffer[0] = 'S';
										Comm_Buffer[1] = 'w';										
										Comm_Buffer[2] = 'N';
										Comm_Buffer[3] = 'r';
										Comm_Buffer[4] = INKey;											
										GO_OUT (5,'T');
										peep();
										}				  																		 
					  	
					  }
	if(Menu_Mode==0){
						SEG_care (16,Cursor);		//:
						Cursor =~ Cursor;
						Segment=20;
						SEG_care (2,1);
						SEG_care (4,1);													 
					  switch (INKey){ 				  						
									 
								    // OK
									case '2':
											 peep();
											 if(HistoryMode){
																HistoryMode=0;	//reset history mode
																Restart_LCD ();
																}
												else{
													if(ChargeMode==0)	PreMetering();
														else{
															 Longpeep();
															 Longpeep();
															 Longpeep();
															 Menu_Mode=0;
															 }
													}																												
									break;

								    // SET
									case '3':

											if(MeterOnFlag || HistoryMode)Longpeep();
												else{
								 					Menu_Mode=1;
										//			SEG_care (21,1);	  //setup ON
													Segment=9;			// set day 														
													peep();
													Delay(50);
													}	   																		
									break;

									// LEFT SIDE

									case '1':								 			
											if(MeterOnFlag || HistoryMode)Longpeep();
												else{

												// bob side = left
													SEG_care (3,1);	  //L [] ON
													SEG_care (1,0);	  //R [] OFF								
													FeedSide=1;																												
													peep();
													}	   																		
									break;

									case '6':								 			
											if(MeterOnFlag || HistoryMode)Longpeep();
												else{
												// bob side = rhite
													SEG_care (3,0);	  //L [] OFF
													SEG_care (1,1);	  //R [] ON										
													FeedSide=0;																												
													peep();
													}	   																		
									break;

									case '4':
									 		if(HistoryMode){
									 						MemoryReadCunter++;									 						
									 						peep();
															HistoryMode=1;
															DisplayMemory();
															}
											else Light_Intensity('P');
													   																		
									break;

									case '5':
											if(HistoryMode){
															MemoryReadCunter--;
															if(MemoryReadCunter <=0){
																					 MemoryReadCunter=1;
																					 Longpeep();
																					}
															peep();
															HistoryMode=1;
															DisplayMemory();
															}
											else Light_Intensity('N');
												   																		
									break;


									case 99:								 		   																		
									break;

									default: Longpeep();
									}
						INKey=99;
						PrintLowCharSegment();
						SetUpperFigers();	
								  
					}

	  // setup mode	update DATE & TIME

 	if(Menu_Mode==1){
					BlankLCD();
					if(Segment <10 && Segment > 3) SEG_care (18,Cursor);	 // date segment ON
				
					if(Segment < 4 && Segment >= 0) SEG_care (15,Cursor);	 // time segment ON
				
					SEG_care (21,1);	  //setup ON
					SEG_care (20,1);		//point ON
					SEG_care (22,1);		//point ON
					SEG_care (16,1);		//:
					Cursor =~ Cursor;					
					switch (INKey){				
									case '1':							
											Segment--;				 
											if(Segment<0)Segment=0,	Longpeep();
											peep();																										
									break;
										// OK out of setup mode
									case '2':							
											Menu_Mode=2;
									//		SEG_care (21,0);	  //setup OFF
											Update_RTC ();
									//		Cursor=0;							
											peep();											
											Delay(20);																			
									break;
									 // history mode enterens
									case '3':
											Menu_Mode=0;									
											HistoryMode=5;
											MemoryReadCunter=1;
											Longpeep();
											VolumeOf_24H =0;
											Cal_Last_24hr ();
								//			memory_24_flag=1;
								//			DisplayMemory ();	
														
									break;

								    // DOWNE
									case '4':
								 			DateTime[Segment]--;
											if(Segment==9)if(DateTime[Segment] < 0) DateTime[Segment]=3;  //day limit
											if(Segment==7)if(DateTime[Segment] < 0) DateTime[Segment]=1;  // mount limit
											if(Segment==3)if(DateTime[Segment] < 0) DateTime[Segment]=2; // hoers limit
											if(Segment==1)if(DateTime[Segment] < 0) DateTime[Segment]=5; // minits limit
											if(DateTime[Segment] < 0) DateTime[Segment]=9;																												
											peep();																												
									break;

								//UP
									case '5':
											DateTime[Segment]++;
											if(Segment==9)if(DateTime[Segment] > 3) DateTime[Segment]=0;  //day limit
											if(Segment==7)if(DateTime[Segment] > 1) DateTime[Segment]=0;  // mount limit
											if(Segment==3)if(DateTime[Segment] > 2) DateTime[Segment]=0; // hoers limit
											if(Segment==1)if(DateTime[Segment] > 5) DateTime[Segment]=0; // minits limit
											if(DateTime[Segment] > 9) DateTime[Segment]=0;
											if(DateTime[Segment] < 0) DateTime[Segment]=1;																				
											peep();																		
									break;

								  //move right to left volume/flow screen
									case '6':
											Segment++;				 
											if(Segment>9)Segment=9, Longpeep();										
									 		peep();																		
									break;
									}
						INKey=99; 
						PrintLowCharSegment();
					}
		
	// seting birth date of babies

  if(Menu_Mode==4){
  					BlankLCD();
  			
					SEG_care (18,Cursor);	 // date segment ON
					SEG_care (19,Cursor);	 // birth segment ON					
					SEG_care (21,1);	  //setup ON
					SEG_care (20,1);		//point ON
					SEG_care (22,1);		//point ON
					DateTime[3]=99;
					Cursor =~ Cursor;
					switch (INKey){
									case 99:								 		   																		
									break;
									case '1':							
											Segment--;				 
											if(Segment<4)Segment=4,	Longpeep();
											peep();																										
									break;

										// OK out of setup mode
									case '2':
								//			Cursor=0;							
											Menu_Mode=0;									
											for(a=4; a<10; a++) BurthDate[a-4]= DateTime[a];											
											Delay(20);																		
											peep();										
											Start_Display();
											ChackBattry ();
											UpdateBasicMem();																			
									break;
									 //set
									case '3':							
									//		Cursor=0;							
											Menu_Mode=0;																													
											peep();										
											Start_Display();															
									break;

								    // DOWNE
									case '4':
								 			DateTime[Segment]--;
											if(DateTime[Segment] < 0) DateTime[Segment]=9;																												
											peep();																												
									break;

								//UP
									case '5':
											DateTime[Segment]++;							
											if(DateTime[Segment] > 9) DateTime[Segment]=0;																				
											peep();																		
									break;

								  //move right to left volume/flow screen
									case '6':
											Segment++;				 
											if(Segment>9)Segment=9, Longpeep();										
									 		peep();																		
									break;
									}						
					INKey=99;
					PrintLowCharSegment();
				}
	
	// seting number of babeis

  if(Menu_Mode==3){
  					BlankLCD();	
					SEG_care (17,Cursor);	 // babies segment blink 		
					SEG_care (21,1);	  //setup ON
					Segment=99;
					Cursor =~ Cursor;			
					DateTime[3]=NumOfBabies;		
					switch (INKey){
									case 99:								 		   																		
									break;
										// OK out of setup mode
									case '2':
											Menu_Mode=4;										
											peep();
											Segment=9;
											for(a=4; a<10; a++)DateTime[a] = BurthDate[a-4];
											Delay(20);								
									break;							

								    // DOWNE
									case '4':
											DateTime[3]=1;
											NumOfBabies=1;
											peep();																												
									break;
									// set
									case '3':							
									//		Cursor=0;							
											Menu_Mode=0;																													
											peep();										
											Start_Display();															
									break;

								//UP
									case '5':
											DateTime[3]=2;
											NumOfBabies=2;
											peep();																		
									break;

								  //move right to left volume/flow screen
									
									default: Longpeep();									
									}
					INKey=99;
					PrintLowCharSegment();
				} 
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
	// seting feeding babeis

  if(Menu_Mode==12){
  					BlankLCD();	
					SEG_care (17,Cursor);	 // babies segment blink		 			
		//	 		Segment=99;
					Cursor =~ Cursor;
					for(a=0;a<10;a++)DateTime[a]=99;			
					DateTime[3]=Baby;
					SEG_care (2,1);	  //R ON
					SEG_care (4,1);	  //L ON
					if(FeedSide) SEG_care (3,1);	  //L [] blink
							else SEG_care (1,1);	  //R []  blink		
					switch (INKey){
									case 99:								 		   																		
									break;
										// OK out of feeding babeis
									case '2':
											peep();									
											Get_Date_Time();
											SEG_care (17,0);	// babies segment off 																								
											if(Sensor_CheckUp()) WarmUPSensor ();											
									
											Menu_Mode=10;																																			
																	
									break;							

								    // DOWNE
									case '4':
											Baby=1;
											peep();																												
									break;
							
								//UP
									case '5':
											Baby=2;											
											peep();																		
									break;							
									
									default: Longpeep();									
									}
					INKey=99;
					PrintLowCharSegment();
					SetUpperFigers();
				} 






 //////////////////////////////////////////////
 ///////////////////////////////////////////////
 ///////////////////////////////////////////////

	//seting units 
					
	if(Menu_Mode==2){
					BlankLCD();			
					SEG_care (13,Cursor);	 // Units blinks
					SEG_care (21,1);	  //setup ON
					Segment=99;
					Cursor =~ Cursor;
					for(a=0;a<10;a++)DateTime[a]=99;
			
					if(Units)SEG_care (10,1);	 // FLOZ ON	
						else SEG_care (12,1);	 // ML ON					
					switch (INKey){
									case 99:								 		   																		
									break;
										// OK out of setup mode
									case '2':
											Menu_Mode=3;											
											peep();
											Delay(20);								
									break;

									case '3':							
											Cursor=0;							
											Menu_Mode=0;																													
											peep();										
											Start_Display();															
									break;

								

								    // DOWNE
									case '4':
											Units = 0;
								 	//		SEG_care (12,1);	 // ML ON											
									//		SEG_care (10,0);	 // FLOZ OFF
											peep();																												
									break;

								//UP
									case '5':
											Units = 0;
									//		SEG_care (10,1);	 // FLOZ ON											
									//		SEG_care (12,0);	 // ML OFF										
											peep();																		
									break;

								  //move right to left volume/flow screen
									
									default: Longpeep();
									}
						INKey=99;
						PrintLowCharSegment();
				}

				// metering mode
	if(Menu_Mode==10){
						HistoryMode=0;
						Cursor =~ Cursor;
						if(FeedSide) SEG_care (3,Cursor);	  //L [] blink
							else SEG_care (1,Cursor);	  //R []  blink
						SEG_care (16,Cursor);		//:
						SEG_care (2,1);	  //R ON
						SEG_care (4,1);	  //L ON
						if(Units)SEG_care (10,1);	 // FLOZ ON	
							else SEG_care (12,1);	 // ML ON													
						switch (INKey){
									case 99:								 		   																		
									break;
										// OK out working mode
									case '2':
											peep();
											EndMetering();
																			
									break;
									case '4':
											Light_Intensity('P');	   																		
									break;

									case '5':								 			
											Light_Intensity('N');	   																		
									break;


									default: Longpeep();								
								}
					  }
	Print_LCD();																			
	Delay(20);
}

//============================
// Lighting_Intensity
//=============================

void Light_Intensity(char a)
{
	if(a=='P') LightDisplay = (LightDisplay+25);
		else LightDisplay = (LightDisplay-25);
	if (LightDisplay>250){
							LightDisplay=250;
							Longpeep();
						}	
	if (LightDisplay<10){
						  LightDisplay=10;
						  Longpeep();
						}
	PCA0CPH0  = LightDisplay;									
	peep();
		
}




//============================
// Get Date & Time
//=============================
void Get_Date_Time()
{
//DateTime[5];			// m,10m,H,10H,Y,10Y,M,10M,D,10D	
char a, b;

	Update_TimeDate	();	   // get time & date from calender
	SEG_care (18,1);	  //date ON
	SEG_care (15,1);	  //time ON
	SEG_care (20,1);		//point ON
	SEG_care (22,1);		//point ON
	SEG_care (16,1);		//:
	for(b=6; b<16; b++){
						a=LCD_Buffer[b]& 0x80;		 //pull out extra bit
						LCD_Buffer[b]=Num_To_Seg_Lower(DateTime[b-6],99)|a;	  // translat numbers to 7 segmanet
						}

}
//===================================
//	Print Low Char Segment
//
//====================================
void PrintLowCharSegment()
{

char a, b;

	for(b=6; b<16; b++){
						a=LCD_Buffer[b]& 0x80;		 //pull out extra bit
						LCD_Buffer[b]=Num_To_Seg_Lower(DateTime[b-6],(b-6))|a;	  // translat numbers to 7 segmanet
						}

}


//====================================

//============================
// Get Upper Figers
//=============================
void SetUpperFigers()
{

unsigned int VolumeZmani;

	Tirgom (Feed_Time);
	if(BuffZ[2]==0)GenBuff[0]=99;
		else GenBuff[0]= BuffZ[2];
//	GenBuff[0]= BuffZ[2];
	GenBuff[1]= BuffZ[3]; 
	SEG_care(5,1);	   			//decimal point
	if(Volume >=1000){
						VolumeZmani=Volume/10;
						Tirgom (VolumeZmani);
						SEG_care(5,0);
						}
	
	else Tirgom (Volume);

	if(BuffZ[1]==0)GenBuff[2]=99;
		else GenBuff[2]= BuffZ[1];

	GenBuff[3]= BuffZ[2];	
	GenBuff[4]= BuffZ[3];

	if(HistoryMode==5)SEG_care (23,0);
		else SEG_care (23,1);	  //upper time ON
	GetUpperFigers();
}


//============================
// Print Upper Figers
//=============================
	
void GetUpperFigers()
{
char a, b;
		
	for(b=0; b<5; b++){
						a=LCD_Buffer[b]& 0x08;		 //pull out extra bit
						LCD_Buffer[b]=Num_To_Seg(GenBuff[b])|a;	  // translat numbers to 7 segmanet
						}
}
//====================================
//============================
// Chack USB  & print icon on screen
// Display battry status
//=============================

void ChackUSB()
{
	if(USB_ON) 	SEG_care(14,1);			
	else SEG_care(14,0);				
}


//======================================
//  translat integer to ascii
//======================================

void Tirgom (unsigned int Zmani){

		BuffZ[3] = (Zmani%10);	// + 0x30; 		// x 1
		Zmani=Zmani/10;
		BuffZ[2] = (Zmani%10);	// + 0x30;   	// x 10
		Zmani=Zmani/10;
		BuffZ[1] = (Zmani%10); 		//+ 0x30;     	 // x100
	   	BuffZ[0] = Zmani/10;		//    x 1000
		

}






////------------------------------------------------------
	// Test input communuction caming from PC or slave 				
//----------------------------------------------------
void  CheckInput()
{
int a,b,x;
	switch (Comm_Buffer[0]){
									
						//// Replaing TO THE S L E V E																		

							case 'k':
									ProbOK=0;
									a = (Comm_Buffer[1]+ Comm_Buffer[2]+ Comm_Buffer[3]); 
									b = (Comm_Buffer[6]+ Comm_Buffer[7]+ Comm_Buffer[8]+
												Comm_Buffer[9]+ Comm_Buffer[10]);  
									x = (a%10) * (b%10);
									if(x>0){									
											a = (Comm_Buffer[4]-0x30)*10;						
											b = (Comm_Buffer[5]-0x30);
											a = a+b;
											if(x == a) ProbOK=1;	//, CodOK=1;
											}
									Sent_Ack();								
									SlvEndOfTran=1;																								 															
							break;

			// get theremistors values in ascii code				
							case 'c':
									 MainThrBuff = atoi(&Comm_Buffer[1]);
									 CompThrBuff = atoi(&Comm_Buffer[5]);									 									 								 
									 Sent_Ack();
									 SlvEndOfTran=1;
			//						 if(OldStyleFlag == 6){
			//										  		OverDiff = (MainThrBuff-CompThrBuff);
			//										  		MeterOn ();
			//										  	}
			//						 else ACK_Flag=1;				
																				  
							break;

			// get theremistors values in ascii code	+ overage different bitween them
							case 'd':
									 MainThrBuff = atoi(&Comm_Buffer[1]);
									 CompThrBuff = atoi(&Comm_Buffer[5]);
									 OverDiff = MainThrBuff-CompThrBuff;																	 									 								 
			//						 MeterOn ();												  
							break;

//////////////////////////////////////////////////////////////////////////////////////////
// get in the overage of 30 readings of (MAIN - COMP) 
//////////////////////////////////////////////////////////////////////////////////////////							
			 				case 's':
			 						 OverDiff = atoi(&Comm_Buffer[1]);
						 			 if(MeterOnFlag)Metering_ON ();
									 SensorWatchDog=0;	 
									 
		 					break;
							
		// gets from SLAVE ACK signal					
							case 'q':
									 ACK_Flag=1;
									 SlvEndOfTran=1;  								 								 
																			  
							break;

			//get from slave software revtion number				
							case 'v':						
									 SlaveRev[0] = Comm_Buffer[1];
									 SlaveRev[1] = Comm_Buffer[2];
									 SlaveRev[2] = Comm_Buffer[4];
									 Sent_Ack();
									if(SlaveRev[0]=='S'&&SlaveRev[1]=='1'&&SlaveRev[2]=='5')SoftRevOK=1;
									 	else SoftRevOK=0;
								 	SlvEndOfTran=1;
																			  
							break;							
			////////////////////////////////////////////////////////			
 			////////////////////// A N S W R I N G    P C
			////////////////////////////////////////////////////////////
						   	case 'R':
							
					//				 Monitor_Reset ();
					//				 OldStyleFlag = 0;
					//				 PCmode=0;
									 RSTSRC=0x10; 			
					//				 SBUF0 = 'R';
					//				 BuffLength=1;										  
							break;	


                           //  Transmiting Master program vertion 

                            case 'V':
									Comm_Buffer[0]='V';
									Comm_Buffer[1]=REV1;
									Comm_Buffer[2]=REV2;
									Comm_Buffer[3]='.';
									Comm_Buffer[4]=REV3;									
									GO_OUT (5, 'T');
																					
							break;
							case 'Z':
									Delay(20);
									SBUF0 = 'Z';
									BuffLength=1;
																					
							break;
				///printing on PC the revetion number of prob software

							case 'W':
									Comm_Buffer[0]='W';
									Comm_Buffer[1]=SlaveRev[0];
									Comm_Buffer[2]=SlaveRev[1];
									Comm_Buffer[3]='.';
									Comm_Buffer[4]=SlaveRev[2];							
									GO_OUT (5,'T');
																				
							break;						
							
				// entering the defult power for heater			
							case 'P':
									 HtrPwr = atoi(&Comm_Buffer[1]);
									 if(HtrPwr > 254)HtrPwr=254;
									 if(HtrPwr < 2)HtrPwr=1;
									 Delay (50);	//update																	
									 SBUF0 = 'P';
									 BuffLength=1;											
							break;

				// format memory entering the beginig (1) of the addres memory 
				// write defpult parameters
				 			case 'F':
									 BurthDate [0] =7;
									 BurthDate [1] =5;
									 BurthDate [2] =1;
									 BurthDate [3] =1;
									 BurthDate [4] =1;
									 BurthDate [5] =2;
									 Units=0;
									 NumOfBabies = 1;

				 					 MemAddControl(2);
									 Delay (4);									 
									 UpdateBasicMem ();	//writing defalt basic prameters
									 BuffLength=1;
				 					 SBUF0 = 'F';
				 					 
									 																		
				 			break;
					//writing to memory burth date

							case 'H':
									 for(a=0;a<6;a++) BurthDate[a]=(Comm_Buffer[6-a]-0x30);									 							 
									 UpdateBasicMem ();	//writing defalt basic prameters
									 BuffLength=1;
				 					 SBUF0 = 'H'; 							 																		
				 			break;


				/// read the memory

							case 'M':
									 ReadMemory();
																									
							break;

			   /// Update Basic Units Prameters to memory

							case 'U':
									 if(Comm_Buffer[1]=='m' & Comm_Buffer[2]=='l') Units=0;
									 if(Comm_Buffer[1]=='o' & Comm_Buffer[2]=='z') Units=0;
									 UpdateBasicMem();
									 SBUF0 = 'U';
									 BuffLength=1;																									
							break;


			   /// Update number of babies
							case 'B':
									 if(Comm_Buffer[1]=='1') NumOfBabies=1;
									 if(Comm_Buffer[1]=='2') NumOfBabies=2;
									 UpdateBasicMem();
									 SBUF0 = 'B';
									 BuffLength=1;																									
							break;

			   
								// enter date 

							case 'D':
									for(a=0;a<10;a++)	DateTime[a]=(Comm_Buffer[10-a]-0x30);
									Update_RTC ();
									SBUF0 = 'D';															
							break;

						   ////	Update Sireal Number
							case 'S':
										UpdateSirealNum();
									  	SBUF0 = 'S';
										BuffLength=1;																								
							break;


						   ////	Get Sireal Number
							case 'G':
										GetSirealNum();								
										GO_OUT (12,'G');
																												
							break;


					  ///// TEST MODE
							case 'T':
										Menu_Mode=33;
										SBUF0 = 'T';
										BuffLength=1;
																												
							break;

					//// TEST LCD
						   case 'L':
										Restart_LCD();
										SBUF0 = 'L';
										BuffLength=1; 																												
							break;

							case 'J':
										ChackBattry ();
										Tirgom (BattryLevel);
										Comm_Buffer[0]=BuffZ[0]+0x30;
										Comm_Buffer[1]=BuffZ[1]+0x30;
										Comm_Buffer[2]=BuffZ[2]+0x30;
										Comm_Buffer[3]=BuffZ[3]+0x30;							
										GO_OUT (4,'G'); 																												
							break;			


				}
				
}

//////////////////////////////////////////////////////////////////////////////////////
	 // Sensor_CheckUp SLAVE
	 
////////////////////////////////////////////////////////////////////////////////////////////////

char Sensor_CheckUp ()
{ 
char a;
	// reseting prob
	
	SlvEndOfTran=0;
	for (a=0; a<5; a++){
						Reset_Prob (); 						  
						if (SlvEndOfTran) break;												
						}
	if (a > 3){
				Error (1);		 // SENSOR NOT RESPANDING
		//		return(0);
				}
	
// Poll out coding of the PROB

	SlvEndOfTran=0;
	for (a=0; a<5; a++){
						ChackValidProb ();	  // chacking the coding of the prob														
						if (SlvEndOfTran) break;												
						}	
	if (!ProbOK){
			//	 Error (2);		 // Prob coding not mucth
			//	 return(1);		 // FOR TESTING ONLY
				} 		 


 // check prob software verstion

	SlvEndOfTran=0;
	for (a=0; a<5; a++){
						ChackSoftProb ();															
						if (SlvEndOfTran) break;															
						}												
	if (!SoftRevOK){
				//	 Error (3);	 // prob software is not mucth
			//		 return(0);	 // FOR TESTING ONLY
					}
// pullup termistors values
 	Delay(200);  		//WAIT UNTIL THERMISTORS READY

 	SlvEndOfTran=0;
	for (a=0; a<5; a++){
						Get_Thr ();											
						if (SlvEndOfTran) break;						
						}												
	if (a > 4){
				Error (1);	  // Sensor not respanding
			//	return(0);
				}
		// checking theremistors				

	if(MainThrBuff > 990 || CompThrBuff > 990){
										//		Error (4);		// Thermistors values too HIGH
										//		return(0);
											  }
	if(MainThrBuff < 99 || CompThrBuff < 99){
										//	Error (5);			// Thermistors values too LOW
										//	return(0);
											} 	 
	if((MainThrBuff-CompThrBuff)> 100){
								//		Error (6);			// Thermistors diffrenc value too LOW
								//		return(0);
									  } 
	return (1);
	
						
}
//-----------------------------------------------
// ERORR hendling
//-----------------------------------------------

void Error (char S)
{
	MeterOnFlag=0;
	BlankLCD ();  // clean LCD buffer 
	GenBuff[0]= 99;	//blank
	GenBuff[1]= 99; //blank
	GenBuff[2]= 11;	// 'E'
	GenBuff[3]= 12;	// 'r'
	GenBuff[4]= S;	// Erorr number		
	GetUpperFigers ();											
	Print_LCD ();
	Longpeep();
	Delay(10);
    Longpeep();
    Delay(10);
    Longpeep();
    Delay(200);
//	Restart_LCD();
//	Reset_Prob ();
	RSTSRC=0x10;

}



//------------------------------------------
// GO OUT TO TREMINAL T and for slave S
//--------------------------------------------

void GO_OUT(char length, char DIS)
{
	Comm_Buffer[length]= '%';	
	BuffLength = length+1;
	Pos =0 ;
	OutFlag = 1;
	if(DIS=='S')SBUF0 = '#';
	if(DIS=='G')SBUF0 = 'G';
	else SBUF0 = '$';	

}



//--------------------------------------
//CHACK serial number  and other numbers
//--------------------------------------------

void ChackValidProb ()
{

	Comm_Buffer[0]= 'k';
	GO_OUT (1,'S');
	Delay(10); 				//wait 	update					
	CheckInput();
}

//------------------------------------------
// SEND ACK
//--------------------------------------------

void Sent_Ack(void)
{
	Comm_Buffer[0]= AckChar;	
	GO_OUT(1, 'S');			 // S sent ACK to SLAVE
	Delay (2);

}


//--------------------------------------
//CHACK PROB SOFTWARE VERSION 
//--------------------------------------------

void ChackSoftProb ()
{

	Comm_Buffer[0]= 'v';	
	GO_OUT (1,'S');
	Delay(4); 				//wait 	update				
	CheckInput();
}														


//--------------------------------------
//Reset Prob
//--------------------------------------------

void Reset_Prob ()
{

	Comm_Buffer[0]= 'r';	
	GO_OUT(1, 'S');
	Delay(2); 				//wait  mS update										
	CheckInput();
}


//--------------------------------------
//Get thremistor from slave
//--------------------------------------------

void Get_Thr ()
{

	Comm_Buffer[0]= 'c';	
	GO_OUT (1,'S');
	Delay(5);  				//wait  mS	update					
	CheckInput();
}														


//--------------------------------------
//Get thremistor difference
//--------------------------------------------

void Get_Difference ()
{
	Comm_Buffer[0] = 's';
	GO_OUT (1,'S');
	Delay(10);						
	CheckInput();
}														



//---------------------------------------------------
// Set Heater
//--------------------------------------------------
void SetHeater (char HTR)
{
	Tirgom(HTR);
   	Comm_Buffer[0]= 'p';
   	Comm_Buffer[1]= BuffZ[1]+0x30;  
   	Comm_Buffer[2]= BuffZ[2]+0x30; 
   	Comm_Buffer[3]= BuffZ[3]+0x30;   	
	GO_OUT (4,'S');
	Delay(5);
}

//---------------------------------------------------
// Turn off Monitor
//--------------------------------------------------


void	Turn_off_Monitor ()
{
	peep();
	ChackBattry();

	// For switch test 
	if(Menu_Mode==33){
					  	Comm_Buffer[0] = 'S';
						Comm_Buffer[1] = 'w';										
						Comm_Buffer[2] = 'N';
						Comm_Buffer[3] = 'r';
						Comm_Buffer[4] = '7';						
						GO_OUT (5,'T');
						}
	else{
			if (ChargeMode==0){
							   if(OK_Flag) EndMetering();
							   else	PreMetering();
							   Delay(150);
							   }
			if(ChargeMode==2)Error (9);
//			Delay(150);		
			if(SWICTH ){ 	  // if SWITCH STILL PREESED TURN OFF THE MONITOR
						Longpeep();				
						Delay(80); 													
						if(ChargeMode==0) SW_OFF = 0;
						}								 
			  
		   }
}
//==================
// Warming UP 
//==================

void WarmUPSensor ()
{

char a;

	SensorWatchDog=0;
	Get_Thr ();		
	Scalibration = (MainThrBuff-CompThrBuff);
		
	SetHeater (HtrPwr);
	for(a=0; a<20; a++){
						 Delay (50);	
						 Get_Thr ();						 
						 if ((MainThrBuff-CompThrBuff)> 50)break;		
	 					}																										
	if(a>18) Error (7);		 //Probe not heating up
						
		else{
			 StabeTime=5;
			 StabeFlag = 0;
			 MeterOnFlag=1;
			 Get_Difference ();
			 StartTimeStatus=DateTime[0];
			 }
}



//=======================
// M E T E R I N G 
//=======================

void Metering_ON ()
{
unsigned int  Flow;
int Scalc, ThermBuffDiff;

	// delay time for stabilezing the sensors
	if(!StabeFlag){
					StabeTime--;
					if(StabeTime<1) StabeFlag=1;
					}
		else{		
			 Scalc=OverDiff-Scalibration;	

	// F I F O ///
/////////////////////////////////////////	
//			ThrmDiffBuff[4]=ThrmDiffBuff[3];
//			ThrmDiffBuff[3]=ThrmDiffBuff[2];
			ThrmDiffBuff[2]=ThrmDiffBuff[1];
			ThrmDiffBuff[1]=ThrmDiffBuff[0];
			ThrmDiffBuff[0] = Scalc;
//---------------------------------------------------------

				
			ThermBuffDiff = (ThrmDiffBuff[2]-ThrmDiffBuff[0]); 
			if ((ThermBuffDiff< (-2)) && FlowFlag==1 )FlowFlag=0;
			if ((ThermBuffDiff > 2) && FlowFlag==0 && Scalc < 130)FlowFlag=1;

			if(FlowFlag){
													
						Scalc = Scalc*10;												
						if (Scalc > 1300)Flow=0;
						if (Scalc <= 1300 && Scalc > 1180)Flow = (1300 - Scalc)/12;		 //0-1
						if (Scalc <= 1180 && Scalc > 1110)Flow = 10+((1180 - Scalc)/7);	 //1-2
						if (Scalc <= 1110 && Scalc > 1080)Flow = 20+((1110 - Scalc)/3);	 //2-3
						if (Scalc <= 1080 && Scalc > 1060)Flow = 30+((1080 - Scalc)/2);	 //3-4
						if (Scalc <= 1060 && Scalc > 1040)Flow = 40+((1060 - Scalc)/2);	 //4-5
						if (Scalc <= 1040 && Scalc > 1020)Flow = 50+((1040 - Scalc)/2);	 //5-6
						if (Scalc <= 1020 && Scalc > 1000)Flow = 60+((1020 - Scalc)/2);	 //6-7
						if (Scalc <= 1000 && Scalc > 980)Flow = 70+((1000 - Scalc)/2);	 //7-8
						if (Scalc <= 980 && Scalc > 960)Flow = 80+((980 - Scalc)/2);	 //8-9
						if (Scalc <= 960 && Scalc > 940)Flow = 90+((960 - Scalc)/2);	 //9-10
						if (Scalc <= 940)Flow = 100;

						Sub_Volume = Sub_Volume+(Flow*96)/10;
						Volume = Sub_Volume/1000;
						SetUpperFigers ();
			//			if(Volume > 999){
			//					 			Volume=Volume/10;
			//								More_then_100_flag=1;
			//							 }
					SBUF0='F';		//for testing only
					  }

   													   

			}
			
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
////	open external memory
///////////////////////////////////////////////////////////////

void Open_Memory (void)
{
	Mem_CS =0; 		// open memory 
}

//////////////////////////////////////////////////////////////////
////////	clsoe external memory
///////////////////////////////////////////////////////////////

void Close_Memory (void)
{
	Mem_CS =1; 		// open memory
	Delay (2);
	 
}

////////////////////////////////////////////////////////////////
/// PRINT TO TERMINAL
//////////////////////////////////////////////////////////////

void SentToTherminal	(void)
{
	if(!OutFlag){
					BuffLength=8;
					Pos =1 ;
					OutFlag = 1;	
					SBUF0 = Comm_Buffer[0];	
					while (OutFlag);   //Wait until END of transmitin 
				}
}

////////////////////////// RTC controll//////////
////////////////////////////////////////////

void 	RTC_chanal (char M)
{
	WACT_CS = M;
}

//void 	Close_RTC (void)
//{
//	WACT_CS = 0;
//}
///////////////////////////////////////////////////////////////////////////////
/////////////  L C D //////////////////////
////////////////////////////////////////////

void	LCD_CS_Mode (char m)
{
	LCD_CS = m;
}
/////////////////////////////////////////////

void	LCD_DATA_Mode (char m)
{
	LCD_DATA = m;
}
/////////////////////////////////////////////////


void	LCD_CLK_Mode (char m)
{
	LCD_CLK = m;
}



//---------------------------------------
// Wrating  to SPI
//---------------------------------------
char Fetch_SPI (char a)
{
char b;
	SPI0DAT = a;
//	Delay (2);
	for (b=0;b<100;b++);
//	a = SPI0DAT;
	return (SPI0DAT);	//return SPI buffer	   
}

													
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////													
////////////////////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// UART0_Interrupt
//-----------------------------------------------------------------------------
//
// This routine is invoked whenever a character is entered or displayed on the
// Hyperterminal.
//
//-----------------------------------------------------------------------------
 
	// servic the UART communction

void UartIsr(void) interrupt 4 using 2{		

	// RECEVING

		if (RI0)	{ 
				 RI0 = 0;		//Reset interapt flag
				 												
				 if (InFlag == 1) Comm_Buffer[Pos++] = SBUF0;	//put the next char in the input buffer
								
				 if (SBUF0 == '$'){
				 						InFlag = 1;		// begining of input string
										Pos=0;			//reset input buffer pointer
										OutFlag=0;													
										}
				 if (SBUF0 == '!'){				 					 
									 Comm_Buffer[0]= 'R';
									 InFlag = 5;								 
								  }

				 if (SBUF0 == '%') InFlag = 5;			// go to check the input
						 			  
				}
				
		// TRANSMITING 							 		
		if(TI0){ 
				 
				TI0 = 0;												//reset of interapt flag
	 	    	if (Pos < BuffLength){
						//				if(MemFlag)SBUF0 = LCD_Buf[Pos++];
										SBUF0 = Comm_Buffer[Pos++];  	// next cahracter
										}
		
				else OutFlag=0, Comm_Buffer[0]=' ';				
				}					
}



// TIMER 2 SERVING THE CLOCK AND ANALOG TO DIGITAL CONVERTER

void Timer2(void) interrupt 5
{


//---------------------------
// TIMER FOR 40Hz
//---------------------------
   	TF2H = 0;		// Reset Interrupt
//	TEST =~TEST;
	if(--DelayT < 0)DelayT=0;		// sub delay counter  
//	if(Clock++ > 79) Clock=0, SecClock++;
	Clock++;
	ClockQ++;
//	PCA0CPH4 = 0x00;
//	Cursor = ~Cursor;
	

}



 // timer 0 routin
//void Timer0(void) interrupt 1
//{
	
//	TF0 = 0 ;	
//	TEST =~TEST;

//}


// service for ADC

void A_TO_D (void) interrupt 10{

	BattryLevel = ADC0;
	AD0INT = 0;									// reset the ADC flag		
}


				
