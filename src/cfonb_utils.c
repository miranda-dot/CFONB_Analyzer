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
