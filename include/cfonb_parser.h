//
// Created by i2202852 on 15/12/2025.
//
#ifndef PARSER
#define PARSER

#include "cfonb_types.h"

// Charge un fichier complet
FichierCFONB* chargerFichier(const char* nomFichier);
// Libère la mémoire
void libererFichier(FichierCFONB* fichier);
#endif
