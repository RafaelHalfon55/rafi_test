


                 
				 //////////////////////////////////////////////////////
				 /////// M E M O R Y ////////////////////////////////
				 ////////////////////////////////////////////////////



//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------


void 	MemAddControl		(char);
void 	ReadMemoryBasic		(void);
void 	SentMemToTerminal 	(void);
char 	Memory_Redy			(void);
void 	ReadMemory			(void);
void 	UpdateBasicMem		(void);
void	Write_SetionTo_Mem	(void);
void	DisplayMemory 		(void);
char	GetNextAdd			(void);
void 	History				(void);
void 	Open_Memory 		(void);
void 	Close_Memory 		(void);

char	Fetch_SPI			(char);
void 	Error 				(char);
void 	Delay 				(unsigned char);
void 	Tirgom 				(unsigned int);
void	BlankLCD 			(void);
void 	SEG_care 			(char,char);
void 	ChackBattry 		(void);
void 	SetUpperFigers		(void);
void 	PrintLowCharSegment	(void);
void 	Print_LCD 			(void);
void 	Enable_Write_To_Memory (void);
void 	SentToTherminal		(void);
void 	PrepareWritingMemory (void);
void	Cal_Last_24hr	 	(void);
void	Get_Date_Time		(void);
void 	Longpeep			(void);
void 	Display_Last_24hr 	(void);
void 	Fising_Memory 		(void);
int		Cal_Memory_Add 		(void);
void 	UpdateSirealNum		(void);


///////////
extern char pdata BurthDate[6];  //	={7,5,1,1,1,2};
extern char NumOfBabies, Units;
extern char pdata Comm_Buffer[14];	//input buffer 
extern char pdata BuffZ [4];		// translation buffer
extern char BuffLength, Feed_Time, FeedSide, Baby, MemoryReadCunter;
extern char pdata DateTime[10];
extern unsigned int Volume ;
extern int VolumeOf_24H;
//extern char memory_24_flag;


//--------------------------------------------------------
///	  check if memory ready for writing and reading   
///-------------------------------------------------------
char Memory_Redy(void)
{
char a;
//	SPI0CFG   = 0x40;	  //configer SPI to 00 mode
	for (a=0; a<5; a++){
						Open_Memory(); 		// enable memory 			
						Fetch_SPI (5);	// call status register addres
				//		Fetch_SPI (0x00);	// get status register 	
						if((Fetch_SPI (0) & 0x01)==0)break; 						
						}
	Close_Memory(); 		// disable memory
	if(a < 4) return 1;
	else return 0;							//else Error(9);   
}
  
//--------------------------------------------------------
///	  geting the next addres (page) for writing the next section  
///-------------------------------------------------------
char GetNextAdd(void)
{
char a;

	if(Memory_Redy()){
						Open_Memory(); 				// enable memory
						Fetch_SPI (3);			// read enable
						Fetch_SPI (0);			// addres byte  1
						Fetch_SPI (0);			// addres byte  2
				//		Fetch_SPI (0);			// get data byte 1
						a = Fetch_SPI (0);						
						Close_Memory(); 				// close memory
						return (a);  		// 	return NEXT page 
						}
	else Error(9);
//	else return (0);  		// 	return 0						 
}

/////////////////////////////////////////////////////////
/// Write page number in the first byet of the memory
/////////////////////////////////////////////////////////
void MemAddControl(char Page)
{
char a;
const char name [] = " Vasa A.T. / Rafael Halfon/ ";		

	PrepareWritingMemory ();

	Fetch_SPI (0);	// addres 1
	Fetch_SPI (0);	// addres 2
	Fetch_SPI(Page);
	Fetch_SPI(' ');
	Fetch_SPI('/');
	Fetch_SPI(Page+0x30);
	Fetch_SPI(' ');
	for (a=0;a<27;a++)Fetch_SPI (name [a]);									
	Close_Memory(); 	// close memory														
					
	
}


/////////////////////////////////////////////////////////
/// Write srial number the memory
/////////////////////////////////////////////////////////
void UpdateSirealNum()
{
char a;
//const char name [] = "*S/N 000000000000";
/////////////////////////////////////////////

	PrepareWritingMemory ();
	Fetch_SPI (0);	// addres 1
	Fetch_SPI (32);	// addres 2

	Fetch_SPI (' ');
	Fetch_SPI ('*');
	for(a=0; a<6; a++) Fetch_SPI (BurthDate[a]+0x30);
	Fetch_SPI ('*');
	Fetch_SPI (Units+0x30);
	Fetch_SPI (NumOfBabies+0x30);
	///////////////////////////////////////////
	Fetch_SPI ('*');	
	Fetch_SPI ('S');
	Fetch_SPI ('/');
	Fetch_SPI ('N');
	Fetch_SPI (' ');

	for(a=0; a<12; a++) Fetch_SPI (Comm_Buffer[a+1]);
	Fetch_SPI ('*');																				
	Close_Memory(); 	// close memory		
	
}

 /////////////////////////////////////////////////////////
/// write to the memory basic parameters burth date units namber of babeis
/////////////////////////////////////////////////////////
void UpdateBasicMem(void)
{
char a;
const char name [] = "*S/N 000000000000*   ";
/////////////////////////////////////////////

	PrepareWritingMemory ();
	Fetch_SPI (0);	// addres 1
	Fetch_SPI (32);	// addres 2

	Fetch_SPI (' ');
	Fetch_SPI ('*');
	for(a=0; a<6; a++) Fetch_SPI (BurthDate[a]+0x30);
	Fetch_SPI ('*');
	Fetch_SPI (Units+0x30);
	Fetch_SPI (NumOfBabies+0x30);
	///////////////////////////////////////////
		

	for(a=0; a<21; a++) Fetch_SPI (name [a]);																				
	Close_Memory(); 	// close memory		
	
}

//////////////////////////////////////////////////////////////////
//////  get serieal number
//////////////////////////////////////////////////////////////

	
void GetSirealNum()
{
char a;
	if (Memory_Redy()==1){
							Open_Memory(); 		// open memory								
							Fetch_SPI (3);	// read state							
							Fetch_SPI (0);	// addres 1
							Fetch_SPI (48);	// addres 2							
							
						// get from memory S/N
							for (a=0; a<12; a++) Comm_Buffer[a]= (Fetch_SPI(0));																																	
							Close_Memory(); 	// close memory				
						 }
	else Error(9);
}
//////////////////////////////////////////////////////////////////
//////  read memory and update  burth day, units number of babies
//////////////////////////////////////////////////////////////

void ReadMemoryBasic(void)
{
char a;
//char num[

	if (Memory_Redy()==1){
							Open_Memory(); 		// open memory								
							Fetch_SPI (3);	// read state							
							Fetch_SPI (0);	// addres 1
							Fetch_SPI (34);	// addres 2							
							
						// get from memory berth date
							for (a=0; a<6; a++)  (BurthDate[a])= (Fetch_SPI(0)-0x30);
											
							Fetch_SPI(0);  //*
				
							Units = (Fetch_SPI(0)-0x30);							
				
							NumOfBabies=(Fetch_SPI(0)-0x30);																										
							Close_Memory(); 	// close memory				
						 }
	else Error(9);
}



///--------------------------------------------------------

///// read from memory    
///-------------------------------------------------------


void ReadMemory(void)
{
int a, add;
char b;

	add = GetNextAdd();	//*32;
	add = (add*4);
															  
	if (Memory_Redy()==1){
							Open_Memory(); 		// open memory								
							Fetch_SPI (3);	// read state							
							Fetch_SPI (0);	// addres 1
							Fetch_SPI (0);	// addres 2
			//				MemFlag =1;		// memory falg is to tell the output routin where to
											// shift out the LCD buffer
																							 
							for (a=0; a<add; a++){
													for (b=0; b<8; b++)	 Comm_Buffer[b]= Fetch_SPI(0);																
													SentToTherminal();
													}						
							Close_Memory();													
						 }
	else Error(9);
}

///--------------------------------------------------------
///	  ///	  Writing to memory   
///-------------------------------------------------------

void Write_SetionTo_Mem(void)
{
char a,b;
int Addres;
char Record[25];

	b= GetNextAdd();
	Addres = b*32;
	
	PrepareWritingMemory ();	

	Fetch_SPI (Addres >> 8);	// addres byte 1
	Fetch_SPI (Addres);			// addres byte 2
	Record[0] ='@';
					
	if(Baby == 1)Record[1] ='1';
	else Record[1] ='2';
	Record[2] ='*';

	for (a=9; a>=0; a--) Record[12-a] = (DateTime[a]+0x30);
	
	Record[13] ='*';
	
	if(FeedSide==1)Record[14] ='L';
		else Record[14] ='R';
	Record[15] ='*';
							
	Tirgom (Feed_Time);
	Record[16] = (BuffZ[2]+0x30);
	Record[17] = (BuffZ[3]+0x30);
	Record[18] ='*';
							
	Tirgom (Volume);								
	Record[19] = (BuffZ[0]+0x30);
	Record[20] = (BuffZ[1]+0x30);
	Record[21] = (BuffZ[2]+0x30);
	Record[22] = (BuffZ[3]+0x30);

	if(Units==1)Record[23] ='O';
		else Record[23] ='M';
	Record[24] ='@';

	for (a=0;a<25;a++)Fetch_SPI(Record[a]);
	Close_Memory(); 	// close memory
	b++;
	Delay (2);  
	MemAddControl(b);						
	
}

/////////////DISPLAY MEMORY SECTION ON THE MONITOR///////
///////////////////////////////////////////////////////////

void Fising_Memory ()
{
int Addres;
char a;
char memory[25];

	Addres = Cal_Memory_Add ();
														  
	if (Memory_Redy()==1){
							Open_Memory(); 		// open memory								
							Fetch_SPI (3);	// read state							
							Fetch_SPI (Addres >> 8);	// addres byte 1
							Fetch_SPI (Addres);			// addres byte 2
			
							for (a=0; a<25; a++) memory[a]= (Fetch_SPI(0)-0x30);
												
							Close_Memory(); 	// close memory

							DateTime[0]	= memory[12];
							DateTime[1]	= memory[11];
							DateTime[2]	= memory[10];
							DateTime[3]	= memory[9];
							DateTime[4]	= memory[8];		 
							DateTime[5]	= memory[7];
							DateTime[6]	= memory[6];
							DateTime[7]	= memory[5];
							DateTime[8]	= memory[4];
							DateTime[9]	= memory[3];

						
							
							Feed_Time = (memory[16])*10;							
							Feed_Time = Feed_Time+ (memory[17]);
 							Volume=0;		 //reset volum counter
			 				Volume =Volume + (memory[19])*1000;								
			 				Volume =Volume + (memory[20])*100;							
			 				Volume =Volume + (memory[21])*10;					
			 				Volume =Volume + (memory[22]);

							Units = 1;
							if(memory[23]==0x1d)Units = 0;	//matric   // "M" = 0x4d - 0x30 = 0x1d
							FeedSide=0;
							if(memory[14]==0x22)FeedSide=1;	//Rhite
											 						   
	
					}
	else Error(9);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

int	Cal_Memory_Add ()
{
int  Addres;

	Addres = (GetNextAdd()-MemoryReadCunter);
	if(Addres < 2){
					MemoryReadCunter--;
					Addres=2;
					Longpeep();
				}
	Addres = (Addres*32);			 
	return (Addres);	
}

////////////////////////////////////////////////////////////////////
///////////////////////////////display on monitor//////////////////

void DisplayMemory()
{
	Fising_Memory ();
	BlankLCD();

		// units

	if(Units==0)SEG_care (12,1); //ml ON			 													
		else SEG_care (10,1);	  //fl+oz ON 																 																				 
							 												
		// bobs side
	if(FeedSide==1)SEG_care (2,1);	  //R ON
		else SEG_care (4,1);			//L
					
	SEG_care (18,1);	  //date ON
	SEG_care (15,1);	  //time ON
	SEG_care (20,1);		//point ON
	SEG_care (22,1);		//point ON
	SEG_care (16,1);		//:
	ChackBattry();
	SetUpperFigers();													
	PrintLowCharSegment();
	Print_LCD ();


}

///////////////////////////////////////////////////////////////////////////////////
/////////

// write to memory
////////////////////////////////////////////////////

void PrepareWritingMemory (void)
{
	if(Memory_Redy()==1){
						Open_Memory(); 		// open memory 										
						Fetch_SPI (6);	// write enable
						Close_Memory(); 	// close memory
						Delay (2);		//150	update
						Open_Memory(); 		// open memory
						Fetch_SPI (2);	// enable writing to memory	
					}
	else Error(9);	
}						
						
						
///////////////////////////////////////////////////////////////////////////////////
////////// Display_Last_24hr
///////////////////////////////////////////////////////////
						
void Cal_Last_24hr ()
{
char MemoryDay, MemoryHour,NowDay, NowHour;
char Maddres;

	Get_Date_Time();
	Maddres =GetNextAdd();
// calculat now day & hore 
	NowDay = (10*DateTime[9]) + DateTime[8];
	NowHour = (10*DateTime[3]) + DateTime[2];

	for(;;){ 
			Fising_Memory ();
			Feed_Time=MemoryReadCunter;
	  			 // calculat past day & hour from memory
			MemoryDay = 10*DateTime[9] + DateTime[8];
			MemoryHour = 10*DateTime[3] + DateTime[2];

			VolumeOf_24H = VolumeOf_24H + Volume;

   			if(   (NowDay-MemoryDay)!= 0  &&  (NowHour-MemoryHour)>0 ) break;	//Display_Last_24hr ();
			if ((Maddres-MemoryReadCunter) == 2) break;
			MemoryReadCunter++;
			}			 
	
	Display_Last_24hr ();		 

}


////////////////////////////////////////////////////////
///////// display volume of 24 hours on screen						
//////////////////////////////////////////////////////////////						
						
void Display_Last_24hr ()
{
	BlankLCD ();
	Print_LCD ();
	Volume = VolumeOf_24H;
	DateTime[3]=2;
	DateTime[2]=4;
	DateTime[1]=20;
	DateTime[0]=21;
	DateTime[4]=99;
	DateTime[5]=99;
	DateTime[6]=99;
	DateTime[7]=99;
	DateTime[8]=99;
	DateTime[9]=99;
	
	
	SetUpperFigers();
	PrintLowCharSegment();
	if(Units==0)SEG_care (12,1); //ml ON			 													
		else SEG_care (10,1);	  //fl+oz ON
//	SEG_care (23,0);	  //upper min OFF
	Print_LCD ();
}						
						



						
						
						
						
						
						
														 								
 