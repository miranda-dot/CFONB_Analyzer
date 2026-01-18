//
// Created by i2202852 on 15/12/2025.
//
#ifndef UTILS
#define UTILS

#include "cfonb_types.h"
// Conversion du caractère signé en montant
Montant decoderMontant(const char* montantStr, int nbDecimales);
// Extraction d'une sous-chaîne (positions CFONB sont en base 1)
void extraireChamp(const char* ligne, int debut, int fin, char* dest);
// Parsing d'une date JJMMAA
DateCFONB parseDate(const char* dateStr);
void afficherMontant(Montant m);

// Comparaison de dates
int comparerDates(DateCFONB d1, DateCFONB d2);

#endif