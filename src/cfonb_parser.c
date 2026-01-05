//
// Created by i2202852 on 15/12/2025.
//
#include <stdio.h>
#include "../include/cfonb_parser.h"

#include <stdlib.h>
#include <string.h>

#include "cfonb_utils.h"

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
int parseOperation(const char* ligne, Operation* op)
{
    extraireChamp(ligne, 22, 32, op->numeroCompte); //num compte
    extraireChamp(ligne, 8, 11, op->codeOperation); //codeOp

    //
    //DateCFONB date;
    //extraireChamp(ligne, 35, 40, date); //date op
    //Placeholder pour le parsing de la date et l'ajout dans la structure

    //DateCFONB date;
    //extraireChamp(ligne, 44, 49, date); //date val
    //Placeholder pour le parsing de la date et l'ajout dans la structure

    extraireChamp(ligne, 50, 80, op->libelle); //libelle
    char* mont = malloc(strlen(ligne) + 1);
    extraireChamp(ligne, 92, 105, mont); //montant
    Montant m = decoderMontant(mont, 2);
    free(mont);
    op->montant = m;
    extraireChamp(ligne, 106, 120, op->reference); //ref

    return 0;

}