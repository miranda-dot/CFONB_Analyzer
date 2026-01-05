//
// Created by i2202852 on 15/12/2025.
//
#ifndef PARSER
#define PARSER

#include "cfonb_types.h"


// Parsing d'une date JJMMAA
DateCFONB parseDate(const char* dateStr);
// Charge un fichier complet
FichierCFONB* chargerFichier(const char* nomFichier);
// Libère la mémoire
void libererFichier(FichierCFONB* fichier);
// Parse un enregistrement 04
int parseOperation(const char* ligne, Operation* op);
#endif
