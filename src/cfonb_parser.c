//
// Created by i2202852 on 15/12/2025.
//
#include <stdio.h>
#include "../include/cfonb_parser.h"

#include <stdlib.h>
#include <string.h>

// Charge un fichier complet
/*// Fichier CFONB complet
typedef struct {
    char nomFichier[256];
    BlocCompte* blocs;         // Tableau dynamique de blocs
    int nbBlocs;
    int capaciteBlocs;
} FichierCFONB;
*/
FichierCFONB* chargerFichier(const char* nomFichier)
{
    FichierCFONB* fichier = (FichierCFONB*) malloc(sizeof(FichierCFONB));
    strcpy(fichier->nomFichier, nomFichier);
    FILE* f = fopen(nomFichier, "r");
    if (f == NULL)
    {
        printf("Erreur d'ouverture du fichier\n");
        return NULL;
    }
    char ligne[256];
    /*
    // Bloc complet d'un compte (01 + opérations + 07)
typedef struct {
    InfoCompte ancienSolde;    // Enregistrement 01
    Operation* operations;     // Tableau dynamique d'opérations
    int nbOperations;
    int capaciteOperations;    // Pour la gestion dynamique
    InfoCompte nouveauSolde;   // Enregistrement 07
} BlocCompte;

    // Informations de compte (enregistrements 01 et 07)
typedef struct {
    char codeBanque[6];      // 5 + \0
    char codeGuichet[6];     // 5 + \0
    char devise[4];          // 3 + \0 (EUR)
    int nbDecimales;
    char numeroCompte[12];   // 11 + \0
    DateCFONB date;
    char titulaire[51];      // 50 + \0
    Montant solde;
} InfoCompte;
    */
    while (fgets(ligne, sizeof(ligne), f) != NULL)
    {
        printf("%s", ligne);
    }
    libererFichier(fichier);

    return fichier;
}
// Libère la mémoire
void libererFichier(FichierCFONB* fichier)
{
    if (fichier != NULL)
    {
        free(fichier);
    }
}