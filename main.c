#include <xc.h>
#include "test.h"
#include "afficheur.h"

/**
 * Bits de configuration:
 */
#pragma config FOSC = INTIO67  // Osc. interne, A6 et A7 comme IO.
#pragma config IESO = OFF      // Pas d'osc. au démarrage.
#pragma config FCMEN = OFF     // Pas de monitorage de l'oscillateur.

// Nécessaires pour ICSP / ICD:
#pragma config MCLRE = EXTMCLR // RE3 est actif comme master reset.
#pragma config WDTEN = OFF     // Watchdog inactif.
#pragma config LVP = OFF       // Single Supply Enable bits off.

#ifndef TEST

void low_priority interrupt bassePriorite() {
    static char n = 0;
    if (PIR1bits.TMR1IF) {
        TMR1 = 60535;
        PIR1bits.TMR1IF = 0;
        PORTA = digit(n);
        switch(n) {
            case 0:
                PORTCbits.RC1 = 1;
                PORTCbits.RC0 = 0;
                n = 1;
                break;
            case 1:
                PORTCbits.RC1 = 0;
                PORTCbits.RC0 = 1;
                n = 0;
                break;
        }
        ADCON0bits.GO = 1;
    }
    
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        afficheurEtablitDigits(ADRESH);
    }
}

/**
 * Initialise les périphériques.
 */
void initialisationHardware() {
    
    // À implémenter.

}

/**
 * Point d'entrée du programme.
 */
void main(void) {
    initialisationHardware();
    afficheurEtablitDigits(77);
    while(1);
}
#endif

#ifdef TEST

/**
 * Point d'entrée des tests unitaires.
 */
void main() {
    
    initialiseTests();
    
    testAfficheur();
    
    finaliseTests();
    
    while(1);
}
#endif
