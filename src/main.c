//
// Created by i2202852 on 15/12/2025.
//
#include <stdio.h>
#include <stdlib.h>

#include "../include/cfonb_utils.h"
#include "../include/cfonb_parser.h"

int main(void) {
    printf("Hello, les ingénieurs de l'ENSIM !\n");

    decoderMontant("0000001925107A",2);
    decoderMontant("0000001925107J",2);

    char rslt[10];
    extraireChamp("0000001925107A",1,10, rslt);

    //chargerFichier("../data/dataExemple"); //WIP
    Operation op;
    const char* ligneCFONB = "0412345ABCD67890EURN 98765432101XX250105NNN250105VIR SALAIRE JANVIER 2025         1234567XX0000001925107{SALAIRE MENSUEL";

    parseOperation(ligneCFONB, &op);
    parseDate("021204");
    parseDate("550101");

    Montant m = {12345, SENS_CREDIT};
    afficherMontant(m);

    comparerDates(d1,d2);
}
