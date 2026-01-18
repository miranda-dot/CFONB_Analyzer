//
// Created by i2202852 on 15/12/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cfonb_types.h"
// Conversion du caractère signé en montant
Montant decoderMontant(const char* montantStr, int nbDecimales)
{
    //montantStr 0000001925107{   -->  19 251, 07
    int lg = strlen(montantStr);
    char carac =  montantStr[lg-1];
    SensOperation sens;

    if (nbDecimales < 0 || nbDecimales > lg)
    {
        printf("%s\n", "Nombre de décimales invalide");
    }

    //correspondance carac positif
    if (carac == '{')
    {
        sens = SENS_CREDIT;
    }
    if (carac >= 'A' && carac <= 'I')
    {
        sens = SENS_CREDIT;
    }
    //correspondance carac negatif
    if (carac == '}')
    {
        sens = SENS_DEBIT;
    }
    if (carac >= 'J' && carac <= 'R')
    {
        sens = SENS_DEBIT;
    }

    Montant mt;
    mt.sens = sens;
    char rsltStr[14] = "";
    strncpy(rsltStr, montantStr, lg - 1);  // Copie tous sauf le dernier
    rsltStr[lg - 1] = '\0';
    mt.centimes = atoi(rsltStr);
    return mt;

}

// Extraction d'une sous-chaîne (positions CFONB sont en base 1)
void extraireChamp(const char* ligne, int debut, int fin, char* dest)
{
    int longueur = fin - debut + 1;

    strncpy(dest, ligne + debut - 1, longueur);
    dest[longueur] = '\0';
}

// Parsing d'une date JJMMAA
DateCFONB parseDate(const char* dateStr){
    DateCFONB date;

    char jour_str[3] = {dateStr[0], dateStr[1], '\0'};
    char mois_str[3] = {dateStr[2], dateStr[3], '\0'};
    char annee_str[3] = {dateStr[4], dateStr[5], '\0'};

    int jour = atoi(jour_str);
    int mois = atoi(mois_str);
    int annee = atoi(annee_str);

    if (jour < 1 || jour > 31) {
        printf("Erreur dans le parsing de la date, format incorrect : jour incorrect");
    }

    if (mois < 1 || mois > 12) {
        printf("Erreur dans le parsing de la date, format incorrect : mois incorrect");
    }

    if (annee < 0 || annee > 99) {
        printf("Erreur dans le parsing de la date, format incorrect : annee incorrecte");
    }

    date.jour = jour;
    date.mois = mois;
    date.annee = annee;

    return date;
}

void afficherMontant(Montant m) {
    // Calculer les euros et les centimes
    unsigned long euros = m.centimes / 100;
    unsigned long cents = m.centimes % 100;

    // Afficher le signe
    char signe = (m.sens == SENS_CREDIT) ? '+' : '-';

    // Affichage simple sans séparateur de milliers
    printf("\n %c%lu,%02lu euros", signe, euros, cents);
}

int comparerDates(DateCFONB d1, DateCFONB d2) {
    //-1 si d1 > d2, 0 si egaux, 1 si d1 < d2
    if (d1.annee < d2.annee) {
        return -1;
    } else if (d1.annee > d2.annee) {
        return 1;
    }
    if (d1.mois < d2.mois) {
        return -1;
    } else if (d1.mois > d2.mois) {
        return 1;
    }
    if (d1.jour < d2.jour) {
        return -1;
    } else if (d1.jour > d2.jour) {
        return 1;
    }

    //si identiques
    return 0;
}