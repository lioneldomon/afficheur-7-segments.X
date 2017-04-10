#include "test.h"
#include <xc.h>
#include <stdio.h>


/** Les digits de l'afficheur. */
static unsigned char digits[2];

/** Conversion BCD*/
static char ascii7Segments[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void afficheurEtablitDigits(char nombre) {
    char dixaine = 0;
    if(nombre >=99){             // Valeur max 99         
        nombre = 99;
    }
    while(nombre>=10){          // Boucle de séparation dixaines/unités
        nombre = nombre-10;
        dixaine++;
      }
      digits[0]=dixaine;
      digits[1]=nombre;

}

unsigned char digit(unsigned char position) {    
 
   
            
    if(position == 0){
        if(digits[0] == 0){
            return 0;
        }
        return ascii7Segments[digits[0]];
    }
    else{
        return ascii7Segments[digits[1]];
    }
    

   
}


#ifdef TEST

int testAffichePasDeZeroAGauche() {
    afficheurEtablitDigits(0);
    testeEgaliteEntiers("AZG00a", digit(1), 0x3F);
    testeEgaliteEntiers("AZG00b", digit(0), 0);

    afficheurEtablitDigits(9);
    testeEgaliteEntiers("AZG09a", digit(1), 0x6F);
    testeEgaliteEntiers("AZG09b", digit(0), 0);
}

int testAfficheDe10A99() {
    afficheurEtablitDigits(10);
    testeEgaliteEntiers("AFF10a", digit(1), 0x3F);
    testeEgaliteEntiers("AFF10b", digit(0), 0x06);

    afficheurEtablitDigits(99);
    testeEgaliteEntiers("AFF99a", digit(1), 0x6F);
    testeEgaliteEntiers("AFF99b", digit(0), 0x6F);
}

int testAfficheAuMaximum99() {
    afficheurEtablitDigits(100);
    testeEgaliteEntiers("AMX100a", digit(1), 0x6F);
    testeEgaliteEntiers("AMX100b", digit(0), 0x6F);

    afficheurEtablitDigits(255);
    testeEgaliteEntiers("AMX255a", digit(1), 0x6F);
    testeEgaliteEntiers("AMX255b", digit(0), 0x6F);
}

void testAfficheur() {
    testAffichePasDeZeroAGauche();
    testAfficheDe10A99();
    testAfficheAuMaximum99();
}

#endif