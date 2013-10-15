

            //////////////////////////////////////////////////////////////////
			/////     R T C        E N D L I N G      ////////////////////////
			//////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void 		Numb_2_BCD 			(char);
void 		Update_RTC 			(void);
void 		BCD_2_NUMB 			(char);
void 		Update_TimeDate 	(void);	   
char		Fetch_SPI			(char);
void 		RTC_chanal 			(char);
//void 		Close_RTC 			(void);
void 		Delay 				(unsigned char);

//----------------------------------------------------------------            
// Global Variables
//-------------------------------------------------------------------------	
			

extern char pdata DateTime[10];	//={8,5,3,2,7,5,2,1,1,3};		// m,10m,H,10H,Y,10Y,M,10M,D,10D
extern char Feed_Time;
extern unsigned int Volume ;
extern unsigned long Sub_Volume;
//////////////////////////////////
//// translat from char to BCD 
//////////////////////////////////

//

void Numb_2_BCD (char Num)
{
char ezer;

	ezer = ((DateTime[Num])& 0x0f);	 		
	ezer = ezer << 4;
	ezer = ezer + ((DateTime[Num-1]& 0x0f));		
	Fetch_SPI (ezer);								

}

///////////////////////////////////////////////////////////////////////
///////////  UP DATE in RTC
///////////////////////////////////////////////////////////

void Update_RTC (void)
{
//	SPI0CFG   = 0x60;  	// configer SPI to 01 mode 
//// m,10m,H,10H,Y,10Y,M,10M,D,10D
	RTC_chanal(1);		 // open SPI RTc chanell
	Delay(2);
	Fetch_SPI (0x13);
	Numb_2_BCD (1); 	// minute
	Numb_2_BCD (3); 	//  hour
	Numb_2_BCD (9); 	// DAY
	Numb_2_BCD (7); 	// D A M Y
	Numb_2_BCD (7); 	// MONTH
	Numb_2_BCD (5); 	// years
	Delay(2);	  		
	RTC_chanal(0);			 // CLOSE RTC
}									 								
			 												
//////////////////////////////////
//// translat BCD to CHAR
//////////////////////////////////


void BCD_2_NUMB (char Num)
{
char ezer;


	ezer = Fetch_SPI(0);
	DateTime[Num] = (ezer & 0x0f);
	DateTime[Num+1] = ((ezer >> 4)& 0x0f);


///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
	if(DateTime[6]>2 && Feed_Time >6)Sub_Volume=Sub_Volume/2;		//GROZINI//
///////////////////////////////////////////////////////////////////////////////////
/////////////===============================//////////////////////////////////////////	
}

 ////////////////////////////////////////////////////////////////////////
///////////  UPDATE monitor
///////////////////////////////////////////////////////////

void Update_TimeDate (void)
{

	//// m,10m,H,10H,Y,10Y,M,10M,D,10D
//	SPI0CFG   = 0x60;  	// configer SPI to 01 mode 									 &
	RTC_chanal(1);		 // open SPI RTS chanell
	Delay(2);
	Fetch_SPI (0x93);	 // command read clock reg minutes (3)
	BCD_2_NUMB(0);	//minutes
	BCD_2_NUMB(2);	//hour
	BCD_2_NUMB(8);	//  day
	BCD_2_NUMB(6);	//D A M Y
	BCD_2_NUMB(6);	//month
	BCD_2_NUMB(4);	//year
	Delay(2);
	RTC_chanal(0);			 // CLOSE RTC
	
}


