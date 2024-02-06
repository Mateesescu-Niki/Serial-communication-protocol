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
	unsigned char j, ch, sc, adresa_hw_dest, adresa_hw_src, screc, src, dest, lng, tipmes, *ptr;
															// Daca nodul este master...
															// M: asteapta cu timeout primul caracter al raspunsului de la slave
															// M: timeout, terminare receptie
															// M: raspunsul de la slave vine, considera ca mesajul anterior a fost transmis cu succes	

															
															// M: ignora restul mesajului
																
															// M: adresa HW ASCII gresita, terminare receptie
																														
															// M: initializare pointer in bufferul ASCII
															
															// M: asteapta cu timeout primul caracter ASCII al adresei HW
															// M: timeout, terminare receptie

															// M: asteapta cu timeout al doilea caracter al adresei HW
															// M: timeout, terminare receptie

															
																
															// M: ignora restul mesajului
															
															// M: adresa HW ASCII gresita, terminare receptie
															
															
															
															// Daca nodul este slave...
															
															// S: asteapta cu timeout primirea primului caracter al unui mesaj de la master
															// S: timeout, terminare receptie
																
															// S: asteapta sincronizarea cu inceputul mesajului
			
															// S: initializeaza pointerul in bufferul ASCII
															// S: asteapta cu timeout primul caracter ASCII al adresei HW
															// S: timeout, terminare receptie

															// S: asteapta cu timeout al doilea caracter al adresei HW
															// S: timeout, terminare receptie

															// S: determina adresa HW destinatie
															
															// S: iese doar cand mesajul era adresat acestui slave
															
															
															
															// M+S: pune in bufasc restul mesajului ASCII HEX
															// M+S: timeout, terminare receptie
															
															
	
															// M+S: reinitializare pointer in bufferul ASCII
															// M+S: initializeaza screc cu adresa HW dest
   
															// M+S: determina adresa HW src
															
															// M+S: aduna adresa HW src

															// Slave actualizeaza adresa Master
	
															// M+S: determina tipul mesajului
																
															// M+S: cod functie eronat, terminare receptie
  															// M+S: ia in calcul in screc codul functiei

															// M+S: Daca mesajul este unul de date
															// M+S: determina sursa mesajului
																
															// M+S: ia in calcul in screc adresa src
		
															// M+S: determina destinatia mesajului
																
															// M+S: ia in calcul in screc adresa dest
		
															// Daca nodul este master...
															// M: bufferul destinatie este deja plin, terminare receptie

															// M+S: determina lng
																	
															// M+S: ia in calcul in screc lungimea datelor
		
															// Daca nodul este master...
															// M: stocheaza in bufbin adresa HW src	
															// M: stocheaza in bufbin tipul mesajului	
															// M: stocheaza in bufbin adresa nodului sursa al mesajului	
															// M: stocheaza in bufbin adresa nodului destinatie al mesajului	
															// M: stocheaza lng
			
																		
															// M: determina un octet de date
																			
															// M: ia in calcul in screc octetul de date
																			
																	
															// M: determina suma de control
																	
															// M: pune sc in bufbin
																		
															// M: mesaj corect, marcare buffer plin
																			
																	
															// M: eroare SC, terminare receptie
																
															// Daca nodul este slave ...
															// S: stocheaza la destsrc codul nodului sursa al mesajului	
															// S: stocheaza lng
																	
															// S: determina un octet de date
																		
															// S: ia in calcul in screc octetul de date
																	
															
															// S: determina suma de control
																		
																		
															// S: mesaj corect, marcare buffer plin
																
																	
															// S: eroare SC, terminare receptie
															
																
																
															// M+S: determina suma de control
																
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


