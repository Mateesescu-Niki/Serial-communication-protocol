#include <c8051F040.h>						// declaratii SFR
#include <osc.h>
#include <Protocol.h>
#include <uart0.h>
#include <lcd.h>
#include <keyb.h>
#include <UserIO.h>

void Afisare_meniu(void);					// afisare meniu initial
void Afisare_mesaj(void);					// afisare mesaj receptionat
void Citire_mesaj(unsigned char i);			// preluare mesaj pentru a fi transmis catre nodul i
void Delay(int ms);								// intarziere programabila
void Error(char *ptr);						// afisare mesaj de eroare

unsigned char TERM_Input(void);
unsigned char AFISARE = 1;

extern unsigned char LCD_line,LCD_col;

//***********************************************************************************************************
extern unsigned char ADR_MASTER;
extern unsigned char TIP_NOD;
extern unsigned char STARE_IO;
extern nod retea[];

//***********************************************************************************************************
void UserIO(void){					// interfata cu utilizatorul
	static unsigned char tasta, cmd, dest, lng;	// variabile locale statice
	
	if(0 == (tasta = TERM_Input())){
		tasta = KEYB_Input();
		if(tasta) LCD_Putch(tasta);
	}
	if(tasta){
		switch(STARE_IO){
			
			case 0:	switch(tasta){
							case '1':
								Error("\n\rTx MSG:> Nod = ");										// afiseaza Tx Msg:> Nod = 
								AFISARE = 0;																							// blocheaza afisarea (AFISARE = 0)
								STARE_IO = 1;																							// trece in starea 1
								cmd = '1';																								// comanda este '1'
								break;
							
							case '2': 	
								Error("\n\rStare Nod = ");										// afiseaza Stare Nod:
								AFISARE = 0;																				// blocheaza afisarea (AFISARE = 0)
								STARE_IO = 1;																				// trece in starea 1
								cmd = '2';
								break;																											// comanda este '2'
						}
						break;			
			case 1:		
				if(cmd == '1' && tasta < '5' && tasta >= '0' && tasta != ADR_NOD+'0'){											// daca comanda e '1' si adresa intre 0-4, mai putin adresa proprie
					dest = tasta - '0';									// extrage dest din tasta
					if(retea[dest].full){									// Daca este deja un mesaj in buffer ...
						UART0_Putstr("\n\r!Bufplin!");
						LCD_PutStr(1,0,"\n\r!Bufplin!");						// afiseaza Buffer plin
						STARE_IO = 0;								// trece in starea 0, s-a terminat tratarea comenzii '1'
						Afisare_meniu();								// afisare meniu
						}										
					else{								// altfel ...
						if(TIP_NOD == MASTER){								// daca nodul es master
							retea[dest].bufbin.adresa_hw_dest = dest;							// pune in bufferul dest adresa hw dest
						}
						else{
							retea[dest].bufbin.adresa_hw_dest = ADR_MASTER;
						}
						retea[dest].bufbin.adresa_hw_src = ADR_NOD;								
						retea[dest].bufbin.dest = dest;									// pune in bufferul dest  adresa hw dest
						retea[dest].bufbin.src = ADR_NOD;
						UART0_Putstr("\n\r Mesajul:> ");
						LCD_PutStr(1,0,"\n\r Mesajul:> ");
						lng=0;						// initializeaza lng = 0 
						STARE_IO=2;												// trece in starea 2, sa astepte mesajul
					}
				}
																
				if(cmd == '2' && tasta < '5' && tasta >= '0' )			 // daca comanda e '2' si adresa intre 0-4, mai putin adresa proprie
					{
					dest = tasta - '0';							// extrage dest din tasta
					if(retea[dest].full){							// Daca este deja un mesaj in buffer ...
						UART0_Putstr("\n\r!Bufplin!");
						LCD_PutStr(1,0,"\n\r!Bufplin!");					// Afiseaza Buffer plin
					}else						// altfel
						UART0_Putstr("\n\r!Bufgol!");  // Afiseaza Buffer gol
						LCD_PutStr(1,0,"\n\r!Bufgol!");
						STARE_IO=0;							// trece in starea 0, s-a terminat tratarea comenzii
						Afisare_meniu();								// afisare meniu
					}
						break;
		
			case 2:	
				if(tasta !='\r' && tasta != '*' && lng<NR_CHAR_MAX){
					retea[dest].bufbin.date[lng++] = tasta;										// stocheaza codul tastei apasate in bufferul de date si incrementeaza lng
				}
				if(tasta == '\r' || tasta == '*' || lng == NR_CHAR_MAX){										// daca s-a atins nr maxim de caractere sau s-a apasat Enter ('\r')
					retea[dest].bufbin.lng = lng;									// stocheaza lng
					retea[dest].bufbin.tipmes = USER_MES;										// pune in bufbin tipul mesajului (USER_MES)
					retea[dest].full = 1;										// marcheaza buffer plin
					STARE_IO = 0;									// trece in starea 0, s-a terminat tratarea comenzii
					Afisare_meniu();									// afisare meniu
					}
						break;	
		}
	}
}

//***********************************************************************************************************
void Afisare_meniu(void){				  			// afisare meniu initial
	AFISARE = 1;
	UART0_Putstr("\n\rTema ");
	LCD_PutStr(0,0,"T");
	UART0_Putch(TEMA + '0');
	LCD_Putch(TEMA + '0');
	
#if(PROTOCOL == MS)
	if(TIP_NOD == MASTER){
		UART0_Putstr(" Master ");	// daca programul se executa pe nodul master
		LCD_PutStr(LCD_line, LCD_col, " Master:");
	}
	else{ 
		UART0_Putstr(" Slave ");						// daca programul se executa pe un nod slave
		LCD_PutStr(LCD_line, LCD_col, " Slave:");
	}
#elif(PROTOCOL == JETON)
	if(TIP_NOD == JETON){
		UART0_Putstr(" Jeton ");
		LCD_Putstr(LCD_line, LCD_col, " Jeton:");
	}
	else{
		UART0_Putstr(" NoJet ");
		LCD_Putstr(LCD_line, LCD_col, "NoJet:");
	}
#endif
	
	UART0_Putch(ADR_NOD + '0');						// afiseaza adresa nodului
	LCD_Putch(ADR_NOD + '0');
#if(TEMA == 1 || TEMA == 3)
	UART0_Putstr(":ASC" );								// afiseaza parametrii specifici temei
	LCD_PutStr(LCD_line, LCD_col, " ASC");
#elif(TEMA == 2 || TEMA == 4)
	UART0_Putstr(":BIN" );
	LCD_PutStr(LCD_line, LCD_col, " BIN");
#endif
	UART0_Putstr("\n\r> 1-TxM 2-Stare :>");	// meniul de comenzi
	LCD_PutStr(1,0, "1-TxM 2-Stare :>");
}


//***********************************************************************************************************
void Afisare_mesaj(void){          		// afisare mesaj din bufferul de receptie i
	unsigned char j, lng, *ptr;
	if(retea[ADR_NOD].full){						// exista mesaj in bufferul de receptie?
		lng = retea[ADR_NOD].bufbin.lng;
		UART0_Putstr("\n\r>Rx: De la nodul ");
		LCD_DelLine(1);
		LCD_PutStr(1,0, "Rx: ");		
		UART0_Putch(retea[ADR_NOD].bufbin.src + '0');			// afiseaza adresa nodului sursa al mesajului
		LCD_Putch(retea[ADR_NOD].bufbin.src + '0');
		
		UART0_Putstr(": ");
		LCD_PutStr(LCD_line, LCD_col, ">: ");	
		
		for(j = 0, ptr = retea[ADR_NOD].bufbin.date; j < lng; j++) UART0_Putch(*ptr++);	// afiseaza mesajul, caracter cu caracter
		for(j = 0, ptr = retea[ADR_NOD].bufbin.date; j < lng; j++) LCD_Putch(*ptr++);		// afiseaza mesajul, caracter cu caracter

		retea[ADR_NOD].full = 0;					// mesajul a fost afisat, marcheaza buffer gol
	}
}

//***********************************************************************************************************
void Error(char *ptr){
	if(AFISARE){
		UART0_Putstr(ptr);
		LCD_DelLine(1);
		LCD_PutStr(1,0, ptr+2);
	}
}

unsigned char TERM_Input(void){

	unsigned char ch, SFRPAGE_save = SFRPAGE;
	
	SFRPAGE = LEGACY_PAGE;
	
	ch = 0;
	if(RI0) ch = UART0_Getch(1);
	
	SFRPAGE = SFRPAGE_save;
	
	return ch;
}