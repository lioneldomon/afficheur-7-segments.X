#ifndef AFFICHEUR__H
#define	AFFICHEUR__H

/**
 * Établit la valeur de chacun des digits selon le nombre à faire paraître.
 * @param nombre Le nombre à afficher.
 */
void afficheurEtablitDigits(unsigned char nombre);

/**
 * Rend la valeur du digit de la position indiquée.
 * @return La valeur du digit, encodé en BCD 7 segments.
 */
unsigned char digit(unsigned char position);


#ifdef TEST
int testAfficheur();
#endif


#endif