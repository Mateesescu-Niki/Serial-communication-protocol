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
unsigned char ascii2bin(unsigned char *ptr);			// functie de conversie 2 caractere ASCII HEX in binar

//***********************************************************************************************************
unsigned char RxMesaj(unsigned char i){					// receptie mesaj															   
	unsigned char j, ch, sc, adresa_hw_src, screc, src, dest, lng, tipmes;
	
																			// dezactivare Tx, validare RX UART1
																			// dezactivare Tx, validare RX RS485
																			// receptie doar octeti de adresa

																			// Daca nodul este master...
																				// M: asteapta cu timeout raspunsul de la slave
																				// M: timeout, terminare receptie, devine master

																				// M: raspunsul de la slave a venit, considera ca mesajul anterior a fost transmis cu succes	
																				// M: adresa HW gresita, terminare receptie
																		
																		
																			
																					// S: ignora mesajul in curs de transmisie
																					
																					// S: inceput mesaj nou, dupa o pauza in transmisie
			
																					// slave-ul nu raspunde
																				
																				// S: iese doar atunci cand mesajul ii este adresat acestui nod slave
 																			
	
																			// receptie octeti de date

																			// M+S: initializeaza screc cu adresa HW dest
	
																			// M+S: Asteapta cu timeout receptia adresei HW sursa
																			// mesaj incomplet
	
																			// M+S: ia in calcul in screc codul functiei

																			// actualizeaza adresa master
	
																			// M+S: Asteapta cu timeout receptia codului functiei
																			// M+S: mesaj incomplet

																			
																				// M+S: ignora restul mesajului
																				
																				// M+S: tip mesaj eronat, terminare receptie
																			
																			// M+S: ia in calcul in screc codul functiei
	
																			// M+S: Daca mesajul este unul de date
																				// M+S: asteapta cu timeout adresa nodului destinatie (M) sau sursa	(S)
																				
																				// M+S: ia in calcul in screc adresa src
		
																				// M+S: asteapta cu timeout adresa nodului destinatie (M) sau sursa	(S)
																				
																				// M+S: ia in calcul in screc adresa src
		
																				// Daca nodul este master...
																					// M: bufferul destinatie este deja plin, terminare receptie
																				

																				// M+S: asteapta cu timeout receptia lng
																				
																				// M+S: ia in calcul in screc lungimea datelor
		
																				// Daca nodul este master...
																					// M: stocheaza adresa HW sursa	
																					// M: stocheaza tipul mesajului	
																					// M: stocheaza la src codul nodului sursa al mesajului	
																					// M: stocheaza la src codul nodului destinatie a mesajului	
																					// M: stocheaza lng
																					
																						// M: asteapta cu timeout un octet de date
																						
																						// M: ia in calcul in screc datele
																						
																					
																					// M: Asteapta cu timeout receptia sumei de control
																					

																					
																						// M: mesaj corect, marcare buffer plin
																						
																					
																					// M: eroare SC, terminare receptie
																															
																				// nodul este slave
																					// S: stocheaza la destsrc codul nodului sursa al mesajului	
																					// S: stocheaza lng
																					
																						// S: asteapta cu timeout un octet de date
																						
																													// S: ia in calcul in screc datele
																						
																					
																					// S: Asteapta cu timeout receptia sumei de control
																					

																					
																						// S: mesaj corect, marcare buffer plin
																						
																		
																					// S: eroare SC, terminare receptie
																		
																		
																		
																				// M+S: Asteapta cu timeout receptia sumei de control
																				
																				
																				// M+S: eroare SC, terminare receptie
																		
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


