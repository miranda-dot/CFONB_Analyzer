//
// Created by i2202852 on 15/12/2025.
//
#include <stdio.h>
#include "../include/cfonb_parser.h"

#include  <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "cfonb_utils.h"


// Libère la mémoire
void libererFichier(FichierCFONB* fichier)
{
    if (fichier != NULL)
    {
        free(fichier);
    }
}


RecordType detecterTypeLigne(const char* ligne) {

    char type_str[3] = {ligne[0], ligne[1], '\0'};

    int type_code = (type_str[0] - '0') * 10 + (type_str[1] - '0');

    switch (type_code) {
    case 1:
        return RECORD_OLD_BALANCE;
    case 4:
        return RECORD_OPERATION;
    case 5:
        return RECORD_COMPLEMENT;
    case 7:
        return RECORD_NEW_BALANCE;
    default:
        return -1;
    }
}

// Parse un enregistrement 04
/*
// Une opération (enregistrement 04)
typedef struct {
    char numeroCompte[12];
    char codeOperation[3];   // 2 + \0
    DateCFONB dateOperation;
    DateCFONB dateValeur;
    char libelle[32];        // 31 + \0
    Montant montant;
    char reference[17];      // 16 + \0
    // Compléments éventuels (04 peut avoir plusieurs 05)
    char complements[5][71]; // Max 5 compléments de 70 chars
    int nbComplements;
} Operation;
*/
int parseOperation(const char* ligne, Operation* op) {

    if (strlen(ligne) < 120) {
        return -1;
    }

    if (ligne[0] != '0' || ligne[1] != '4') {
        return -1;
    }

    memset(op, 0, sizeof(Operation));

    // num compte
    extraireChamp(ligne, 22, 32, op->numeroCompte);

    // code op
    extraireChamp(ligne, 33, 34, op->codeOperation);

    // date op
    char dateOpStr[7];
    extraireChamp(ligne, 35, 40, dateOpStr);
    op->dateOperation = parseDate(dateOpStr);

    //date val
    char dateValStr[7];
    extraireChamp(ligne, 44, 49, dateValStr);
    op->dateValeur = parseDate(dateValStr);

    // libelle
    extraireChamp(ligne, 50, 80, op->libelle);
    for (int i = 30; i >= 0 && op->libelle[i] == ' '; i--) {
        op->libelle[i] = '\0';
    }

    // montant
    char montantStr[14];
    extraireChamp(ligne, 92, 104, montantStr);

    char dernierChar = montantStr[12];
    op->montant.sens = SENS_CREDIT;

    if (dernierChar == '}') {
        montantStr[12] = '0';
        op->montant.sens = SENS_DEBIT;
    } else if (dernierChar >= '{' && dernierChar <= '|') {
        montantStr[12] = '0' + (dernierChar - '{');
        op->montant.sens = SENS_CREDIT;
    } else if (dernierChar >= 'A' && dernierChar <= 'I') {
        montantStr[12] = '1' + (dernierChar - 'A');
        op->montant.sens = SENS_DEBIT;
    } else if (dernierChar >= 'J' && dernierChar <= 'R') {
        montantStr[12] = '1' + (dernierChar - 'J');
        op->montant.sens = SENS_CREDIT;
    }

    op->montant.centimes = 0;
    for (int i = 0; i < 13; i++) {
        if (isdigit(montantStr[i])) {
            op->montant.centimes = op->montant.centimes * 10 + (montantStr[i] - '0');
        }
    }

    // ref
    extraireChamp(ligne, 105, 120, op->reference);
    for (int i = 15; i >= 0 && op->reference[i] == ' '; i--) {
        op->reference[i] = '\0';
    }

    return 0;
}