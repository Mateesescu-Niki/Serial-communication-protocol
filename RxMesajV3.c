#include <c8051F040.h>	// declaratii SFR
#include <uart1.h>
#include <Protocol.h>
#include <UserIO.h>

extern nod retea[];						// reteaua Master-Slave, cu 5 noduri

extern unsigned char STARE_NOD;		// starea initiala a nodului curent
extern unsigned char TIP_NOD;			// tip nod
extern unsigned char ADR_MASTER;	// adresa nodului master

extern unsigned char timeout;		// variabila globala care indica expirare timp de asteptare eveniment
//***********************************************************************************************************
unsigned char RxMesaj(unsigned char i);				// primire mesaj de la nodul i
	unsigned char ascii2bin(unsigned char *ptr);					// functie de conversie 2 caractere ASCII HEX in binar

	//***********************************************************************************************************
	unsigned char RxMesaj(unsigned char i){					// receptie mesaj															   
	unsigned char j, sc, ch, adresa_hw_src, screc, src, dest, lng, tipmes, *ptr;

	UART1_TxRxEN(0, 1);																					// dezactivare Tx, validare RX UART1
	UART1_RS485_XCVR(0,1);																	// dezactivare Tx, validare RX RS485
	UART1_MultiprocMode(MULTIPROC_ADRESA);									// receptie doar octeti de adresa

	if(TIP_NOD == MASTER){													   // Daca nodul este master...
		ch = UART1_Getch_TMO(WAIT);												// M: asteapta cu timeout primul caracter al raspunsului de la slave
		if (timeout) return TMO;											   // M: timeout, terminare receptie
		else {																   // M: raspunsul de la slave vine, considera ca mesajul anterior a fost transmis cu succes
			retea[i].full = 0;
			if (ch != ADR_NOD + '0') return ERA;							   // M: adresa HW ASCII gresita, terminare receptie
		}							   									   
	}	
	else{																	   // Daca nodul este slave...
		do{
			ch = UART1_Getch_TMO(2*WAIT + ADR_NOD*WAIT);	// S: asteapta cu timeout primirea primului caracter al unui mesaj de la master
			if (timeout) return TMO;	// S: timeout, terminare receptie
		}while(ch != ADR_NOD + '0');	// S: iese doar cand mesajul era adresat acestui slave
	}																					
																				
																			

	UART1_MultiprocMode(MULTIPROC_DATA);								// receptie octeti de date

	ptr = retea[ADR_NOD].bufasc;											  // M+S: pune in bufasc restul mesajului ASCII HEX
	*ptr = ADR_NOD + '0';																			
																				
	do{
		*(++ptr) = UART1_Getch_TMO(5);
		if(timeout) return CAN;											   // M+S: timeout, terminare receptie
	}while(*ptr != 0x0A);																			
		
		
	//ptr = retea[i].bufasc; 							
	ptr = retea[ADR_NOD].bufasc;	// M+S: reinitializare pointer in bufferul ASCII
																					// M+S: initializeaza screc cu adresa HW dest
	screc = *ptr++ - '0';

	adresa_hw_src = ascii2bin(ptr);		// M+S: determina adresa HW src
	ptr+=2;
	screc += adresa_hw_src; 				// M+S: aduna adresa HW src

																					
	if(TIP_NOD == SLAVE){ 				
		ADR_MASTER = adresa_hw_src;					// Slave actualizeaza adresa Master
	}

	tipmes = ascii2bin(ptr);	// M+S: determina tipul mesajului
	ptr+=2;
		
	if(tipmes > 1)return TIP;	// M+S: cod functie eronat, terminare receptie

	screc += tipmes;// M+S: ia in calcul in screc codul functiei

	if(tipmes == USER_MES){													// M+S: Daca mesajul este unul de date
				
			src = ascii2bin(ptr);											   // M+S: determina sursa mesajului
			ptr += 2;
			screc += src;													   // M+S: ia in calcul in screc adresa src

			dest = ascii2bin(ptr);											  // M+S: determina destinatia mesajului
			ptr += 2;
			screc += dest;													  // M+S: ia in calcul in screc adresa dest		
		
			if(TIP_NOD == MASTER){																	// Daca nodul este master...
				if(retea[dest].full == 1) return OVR;										// M: bufferul destinatie este deja plin, terminare receptie
			}
			
			lng = ascii2bin(ptr);																					// M+S: determina lng
			ptr += 2;
			screc += lng;
			
			if(TIP_NOD == MASTER){																					// Daca nodul este master...
				retea[dest].bufbin.adresa_hw_src = ADR_NOD;															// M: stocheaza in bufbin adresa HW src 
				retea[dest].bufbin.tipmes = tipmes;																	// M: stocheaza in bufbin tipul mesajului 
				retea[dest].bufbin.src = src;																		   // M: stocheaza in bufbin adresa nodului sursa al mesajului  
				retea[dest].bufbin.dest = dest;																		// M: stocheaza in bufbin adresa nodului destinatie al mesajului
				retea[dest].bufbin.lng = lng;																		  // M: stocheaza lng
							
					
				for(j=0;j<retea[dest].bufbin.lng;j++)
				{																	
					retea[dest].bufbin.date[j]=ascii2bin(ptr);																	
					ptr+=2;																 // pune in bufasc datele
					screc = screc + retea[dest].bufbin.date[j];
				}																							 

				sc = ascii2bin(ptr);																				// M: determina suma de control
								
				if(sc == screc){
					retea[dest].full = 1 ;																			 // M: pune sc in bufbin
					return ROK;																						// M: mesaj corect, marcare buffer plin
				}
				else return ESC;																					 // M: eroare SC, terminare receptie
								
			}
			else {
			retea[ADR_NOD].bufbin.src = src ;					 // S: stocheaza la destsrc codul nodului sursa al mesajului 
			retea[ADR_NOD].bufbin.lng = lng;					 // S: stocheaza lng
							
			for(j=0;j<retea[ADR_NOD].bufbin.lng;j++)
			{																	
				retea[ADR_NOD].bufbin.date[j]=ascii2bin(ptr);																	
				ptr+=2;																 // pune in bufasc datele
				screc = screc + retea[ADR_NOD].bufbin.date[j];
			}																						// S: ia in calcul in screc octetul de date
									
			sc = ascii2bin(ptr);																				// S: determina suma de control
			if(sc == screc){																					 // S: mesaj corect, marcare buffer plin
				retea[ADR_NOD].full = 1 ;
				return ROK;
			}
			else return ESC;																					  // S: eroare SC, terminare receptie
			}
	}
		else{
		sc = ascii2bin(ptr);																					// S: determina suma de control
		if(sc == screc) return ROK;
		else return ESC; // M+S: eroare SC, terminare receptie
		}
		//return TMO;
	}																										
																							
																			
																					


//***********************************************************************************************************
unsigned char ascii2bin(unsigned char *ptr){			// converteste doua caractere ASCII HEX de la adresa ptr
	unsigned char bin;
	
	if(*ptr > '9') bin = (*ptr++ - 'A' + 10) << 4;	// contributia primului caracter daca este litera
	else bin = (*ptr++ - '0') << 4;									// contributia primului caracter daca este cifra
	if(*ptr > '9') bin  += (*ptr++ - 'A' + 10);			// contributia celui de-al doilea caracter daca este litera
	else bin += (*ptr++ - '0');											// contributia celui de-al doilea caracter daca este cifra
	return bin;
}