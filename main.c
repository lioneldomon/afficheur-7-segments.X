#include <xc.h>
#include "test.h"
#include "afficheur.h"
#include <pic18f25k22.h>

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

char nb=0;

void low_priority interrupt bassePriorite() {
    static char n = 0;
    if (PIR1bits.TMR1IF) {
        TMR1H = 178;                        // Mise de la valeur du timer1
        PIR1bits.TMR1IF = 0;                // Clear du bit d'interruption du TMR1
        PORTA = digit(n);                   // Place la valeur désirée sur le portC
        switch(n) {                         // Choix de l'affichage multiplexé
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
        ADCON0bits.GO = 1;                  // Démarre une conversion AD
    }
    
    if (PIR1bits.ADIF) {                    // Si conversion AD terminée
        PIR1bits.ADIF = 0;                  // Clear du bit d'interruption du convertisseur
        afficheurEtablitDigits(ADRESH);     // Appel la fonction
    }
}

/**
 * Initialise les périphériques.
 */
void initialisationHardware() {
    
    TRISA = 0x00;       // Init port A en sortie
    TRISCbits.RC0 = 0;  // Init RC0 et RC1 en sortie
    TRISCbits.RC1 = 0;
    TRISBbits.RB4 = 1;  // Init RB4 en entrée
    
    // Module Analogique
    
    // Désactive les entrées analogiques des ports A et C:
    ANSELA = 0;
    ANSELC = 0;
    
    ANSELBbits.ANSB4 = 1;   // Active RB4 en entrée analogique
    ADCON0 = 0b00101101;    // Active le convertisseur AD et AD11(RB4)
    ADCON2bits.ADFM = 0;    // Resultat dans registre ADRESH (ignore les 2 bits de poids faible)
    ADCON2bits.ACQT = 000;  // Temps d'aquisition 000
    ADCON2bits.ADCS = 011;  // Oscillateur interne
    
    // Activer le temporisateur 1:
    T1CONbits.TMR1ON =1;        // Active le temporisateur.
    T1CONbits.RD16 = 0;         // Temporisateur de 8 bits.
    T1CONbits.T1SYNC = 1;       // Pas de sychro.
    T1CONbits.T1SOSCEN = 0;     // Pas de deuxième oscillateur
    T1CONbits.T1CKPS1 = 0;      // Pas de diviseur
    T1CONbits.T1CKPS0 = 0;
    T1CONbits.TMR1CS1 = 0;      // Système clock
    T1CONbits.TMR1CS0 = 1;
    T1GCONbits.TMR1GE = 0;      // Timer1 counts regardless of Timer1 gate function
    
    // Activer les interruptions:
    RCONbits.IPEN = 1;          // Active les niveaux d'interruptions.
    INTCONbits.GIEH = 1;        // Active les interruptions de haute priorité.
    INTCONbits.GIEL = 1;        // Active les interruptions de basse priorité.
    PIE1bits.TMR1IE = 1;        // Active les interruptions du TMR1
    PIE1bits.ADIE = 1;          // Active les interruptions de conv AD
    IPR1bits.ADIP = 1;
}

/**
 * Point d'entrée du programme.
 */
void main(void) {
    initialisationHardware();
    while(1);
}
#endif

#ifdef TEST

/*
 * Point d'entrée des tests unitaires.
 */
void main() {
    
    initialiseTests();
    
    testAfficheur();
    
    finaliseTests();
    
    while(1);
}
#endif
