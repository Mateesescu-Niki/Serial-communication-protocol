#include <c8051F040.h>	// declaratii SFR
#include <uart1.h>
#include <Protocol.h>
#include <UserIO.h>

extern unsigned char STARE_NOD;		// starea initiala a nodului curent
extern unsigned char TIP_NOD;			// tip nod initial: Nu Master, Nu Jeton

extern nod retea[];

extern unsigned char timeout;		// variabila globala care indica expirare timp de asteptare eveniment
//***********************************************************************************************************
void TxMesaj(unsigned char i);	// transmisie mesaj destinat nodului i
void bin2ascii(unsigned char ch, unsigned char *ptr);	// functie de conversie octet din binar in ASCII HEX

//***********************************************************************************************************
void TxMesaj(unsigned char i){					// transmite mesajul din buffer-ul i
	unsigned char sc, j;

																				// daca este un mesaj de interogare (POLL=0)
																					// calculeaza direct sc
																																						
																				// altfel...
																					// initializeaza SC	cu adresa HW a nodului destinatie
																					// ia in adresa_hw_src
																					// ia in calcul tipul mesajului
																					// ia in calcul adresa nodului sursa al mesajului
																					// ia in calcul adresa nodului destinatie al mesajului
																					// ia in calcul lungimea datelor
																						
																						// ia in calcul datele
																					// stocheaza suma de control
 																			
	
																				// urmeaza transmisia octetului de adresa
																				// validare Tx si Rx UART1
																				// validare Tx si Rx RS485

																				// transmite adresa HW a nodului dest
																				
																				// daca caracterul primit e diferit de cel transmis ...
																						// dezactivare Tx UART1
																						// dezactivare Tx RS485
																						// afiseaza Eroare coliziune
																						// asteapta 1 secunda
																						// termina transmisia (revine)
																				

																				// urmeaza tranmisia octetilor de date
																				// dezactivare Rx UART1

																				// transmite adresa HW a nodului sursa
																				// transmite tipul mesajului
	
																				// Daca mesajul este de date ...
																					// transmite adresa nodului sursa mesaj
																					// transmite lungimea mesajului
    																				
																						// transmite octetii de date
																			
																				// transmite suma de control

																				// activare Rx UART1
																				
																				// slave-ul considera acum ca a transmis mesajul
	
																				// // asteapta transmisia/receptia ultimului caracter

																				// dezactivare Tx si RX UART1
																				// dezactivare Tx si Rx RS485
}

//***********************************************************************************************************
void bin2ascii(unsigned char ch, unsigned char *ptr){	// converteste octetul ch in doua caractere ASCII HEX puse la adresa ptr
	unsigned char first, second;
	first = (ch & 0xF0)>>4;						// extrage din ch primul digit
	second = ch & 0x0F;								// extrage din ch al doilea digit
	if(first > 9) *ptr++ = first - 10 + 'A';	// converteste primul digit daca este litera
	else *ptr++ = first + '0';				// converteste primul digit daca este cifra
 	if(second > 9) *ptr++ = second - 10 + 'A';	// converteste al doilea digit daca este litera
	else *ptr++ = second + '0';				// converteste al doilea digit daca este cifra
}
