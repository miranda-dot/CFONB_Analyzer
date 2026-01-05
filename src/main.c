//
// Created by i2202852 on 15/12/2025.
//
#include <stdio.h>

#include "../include/cfonb_utils.h"
#include "../include/cfonb_parser.h"

int main(void) {
    printf("Hello, les ingénieurs de l'ENSIM !\n");

    decoderMontant("0000001925107A",2);
    decoderMontant("0000001925107J",2);

    char rslt[10];
    extraireChamp("0000001925107A",1,10, rslt);

    chargerFichier("../data/dataExemple");

}
