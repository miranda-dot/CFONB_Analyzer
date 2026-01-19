//
// Created by i2202852 on 15/12/2025.
//
#include <stdio.h>
#include "../include/cfonb_parser.h"

#include  <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "cfonb_utils.h"


FichierCFONB* chargerFichier(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (!fichier) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", nomFichier);
        return NULL;
    }

    FichierCFONB* ficCFONB = (FichierCFONB*)malloc(sizeof(FichierCFONB));

    strncpy(ficCFONB->nomFichier, nomFichier, sizeof(ficCFONB->nomFichier) - 1);
    ficCFONB->nomFichier[sizeof(ficCFONB->nomFichier) - 1] = '\0';
    ficCFONB->nbBlocs = 0;
    ficCFONB->capaciteBlocs = 10;
    ficCFONB->blocs = (BlocCompte*)malloc(ficCFONB->capaciteBlocs * sizeof(BlocCompte));

    char ligne[130];
    int indiceBlocCourant = -1;
    Operation opEnCours;
    int operationEnCours = 0;
    // ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier)) {

        size_t longueur = strlen(ligne);
        if (longueur > 0 && ligne[longueur - 1] == '\n') {
            ligne[longueur - 1] = '\0';
        }

        RecordType type = detecterTypeLigne(ligne);

        switch (type) {
            case RECORD_OLD_BALANCE: { //  01
                if (operationEnCours && indiceBlocCourant >= 0) {
                    BlocCompte* bloc = &ficCFONB->blocs[indiceBlocCourant];
                    if (bloc->nbOperations >= bloc->capaciteOperations) {
                        bloc->capaciteOperations *= 2;
                        Operation* nouvOps = (Operation*)realloc(bloc->operations,
                            bloc->capaciteOperations * sizeof(Operation));

                        bloc->operations = nouvOps;
                    }
                    bloc->operations[bloc->nbOperations++] = opEnCours;
                    operationEnCours = 0;
                }

                if (ficCFONB->nbBlocs >= ficCFONB->capaciteBlocs) {
                    ficCFONB->capaciteBlocs *= 2;
                    BlocCompte* nouveauBlocs = (BlocCompte*)realloc(ficCFONB->blocs,
                        ficCFONB->capaciteBlocs * sizeof(BlocCompte));

                    ficCFONB->blocs = nouveauBlocs;
                }

                // nouveau bloc
                indiceBlocCourant = ficCFONB->nbBlocs++;
                BlocCompte* nouveauBloc = &ficCFONB->blocs[indiceBlocCourant];

                // init
                memset(nouveauBloc, 0, sizeof(BlocCompte));
                nouveauBloc->capaciteOperations = 10;
                nouveauBloc->operations = (Operation*)malloc(
                    nouveauBloc->capaciteOperations * sizeof(Operation));
                nouveauBloc->nbOperations = 0;

                // Parsing
                InfoCompte* ancienSolde = &nouveauBloc->ancienSolde;

                // code banque
                extraireChamp(ligne, 3, 7, ancienSolde->codeBanque);

                // code guichet
                extraireChamp(ligne, 12, 16, ancienSolde->codeGuichet);

                // devise
                extraireChamp(ligne, 17, 19, ancienSolde->devise);

                // nb decimale
                ancienSolde->nbDecimales = ligne[20] - '0';

                // num compte
                extraireChamp(ligne, 22, 32, ancienSolde->numeroCompte);

                // date
                char dateStr[7];
                extraireChamp(ligne, 35, 40, dateStr);
                ancienSolde->date = parseDate(dateStr);

                // titulaire
                extraireChamp(ligne, 41, 71, ancienSolde->titulaire);
                    /*
                for (int i = 49; i >= 0 && ancienSolde->titulaire[i] == ' '; i--) {
                    ancienSolde->titulaire[i] = '\0';
                }
                */

                // montant
                char montantStr[14];
                extraireChamp(ligne, 91, 104, montantStr);

                // parsing
                char dernierChar = montantStr[12];
                ancienSolde->solde.sens = SENS_CREDIT;

                if (dernierChar == '}') {
                    montantStr[12] = '0';
                    ancienSolde->solde.sens = SENS_DEBIT;
                } else if (dernierChar >= '{' && dernierChar <= '|') {
                    montantStr[12] = '0' + (dernierChar - '{');
                    ancienSolde->solde.sens = SENS_CREDIT;
                } else if (dernierChar >= 'A' && dernierChar <= 'I') {
                    montantStr[12] = '1' + (dernierChar - 'A');
                    ancienSolde->solde.sens = SENS_DEBIT;
                } else if (dernierChar >= 'J' && dernierChar <= 'R') {
                    montantStr[12] = '1' + (dernierChar - 'J');
                    ancienSolde->solde.sens = SENS_CREDIT;
                }

                ancienSolde->solde.centimes = 0;
                for (int i = 0; i < 13; i++) {
                    if (isdigit(montantStr[i])) {
                        ancienSolde->solde.centimes = ancienSolde->solde.centimes * 10 + (montantStr[i] - '0');
                    }
                }

                break;
            }

            case RECORD_OPERATION: { //  04
                if (operationEnCours && indiceBlocCourant >= 0) {
                    BlocCompte* bloc = &ficCFONB->blocs[indiceBlocCourant];
                    if (bloc->nbOperations >= bloc->capaciteOperations) {
                        bloc->capaciteOperations *= 2;
                        Operation* nouvOps = (Operation*)realloc(bloc->operations,
                            bloc->capaciteOperations * sizeof(Operation));
                        bloc->operations = nouvOps;
                    }
                    bloc->operations[bloc->nbOperations++] = opEnCours;
                }

                // parsing
                if (parseOperation(ligne, &opEnCours) == 0) {
                    operationEnCours = 1;
                }
                break;
            }

            case RECORD_COMPLEMENT: { // 05
                if (operationEnCours) {
                    parseComplement(ligne, &opEnCours);
                }
                break;
            }

            case RECORD_NEW_BALANCE: { // 07
                if (operationEnCours && indiceBlocCourant >= 0) {
                    BlocCompte* bloc = &ficCFONB->blocs[indiceBlocCourant];
                    if (bloc->nbOperations >= bloc->capaciteOperations) {
                        bloc->capaciteOperations *= 2;
                        Operation* nouvOps = (Operation*)realloc(bloc->operations,
                            bloc->capaciteOperations * sizeof(Operation));

                        bloc->operations = nouvOps;
                    }
                    bloc->operations[bloc->nbOperations++] = opEnCours;
                    operationEnCours = 0;
                }

                // Parsing
                if (indiceBlocCourant >= 0) {
                    InfoCompte* nouveauSolde = &ficCFONB->blocs[indiceBlocCourant].nouveauSolde;

                    extraireChamp(ligne, 3, 7, nouveauSolde->codeBanque);
                    extraireChamp(ligne, 12, 16, nouveauSolde->codeGuichet);
                    extraireChamp(ligne, 17, 19, nouveauSolde->devise);
                    nouveauSolde->nbDecimales = ligne[20] - '0';
                    extraireChamp(ligne, 22, 32, nouveauSolde->numeroCompte);

                    char dateStr[7];
                    extraireChamp(ligne, 35, 40, dateStr);

                    nouveauSolde->date = parseDate(dateStr);

                    extraireChamp(ligne, 41, 90, nouveauSolde->titulaire);

                    char montantStr[14];
                    extraireChamp(ligne, 91, 104, montantStr);

                    char dernierChar = montantStr[12];
                    nouveauSolde->solde.sens = SENS_CREDIT;

                    if (dernierChar == '}') {
                        montantStr[12] = '0';
                        nouveauSolde->solde.sens = SENS_DEBIT;
                    } else if (dernierChar >= '{' && dernierChar <= '|') {
                        montantStr[12] = '0' + (dernierChar - '{');
                        nouveauSolde->solde.sens = SENS_CREDIT;
                    } else if (dernierChar >= 'A' && dernierChar <= 'I') {
                        montantStr[12] = '1' + (dernierChar - 'A');
                        nouveauSolde->solde.sens = SENS_DEBIT;
                    } else if (dernierChar >= 'J' && dernierChar <= 'R') {
                        montantStr[12] = '1' + (dernierChar - 'J');
                        nouveauSolde->solde.sens = SENS_CREDIT;
                    }

                    nouveauSolde->solde.centimes = 0;
                    for (int i = 0; i < 13; i++) {
                        if (isdigit(montantStr[i])) {
                            nouveauSolde->solde.centimes = nouveauSolde->solde.centimes * 10 + (montantStr[i] - '0');
                        }
                    }
                }
                break;
            }

            default:
                // Type d'enregistrement non reconnu, ignorer
                break;
        }
    }

    // sauvegarder derniere op si pas déjà fait
    if (operationEnCours && indiceBlocCourant >= 0) {
        BlocCompte* bloc = &ficCFONB->blocs[indiceBlocCourant];
        if (bloc->nbOperations >= bloc->capaciteOperations) {
            bloc->capaciteOperations *= 2;
            Operation* nouvOps = (Operation*)realloc(bloc->operations,
                bloc->capaciteOperations * sizeof(Operation));
            if (nouvOps) {
                bloc->operations = nouvOps;
                bloc->operations[bloc->nbOperations++] = opEnCours;
            }
        } else {
            bloc->operations[bloc->nbOperations++] = opEnCours;
        }
    }

    fclose(fichier);
    return ficCFONB;
}

void libererFichier(FichierCFONB* fichier) {

    // libérer op de chaque blocs
    if (fichier->blocs != NULL) {
        for (int i = 0; i < fichier->nbBlocs; i++) {
            // libérer tableau op du bloc
            if (fichier->blocs[i].operations != NULL) {
                free(fichier->blocs[i].operations);
                fichier->blocs[i].operations = NULL;
            }
        }

        // libérer tableau de bloc
        free(fichier->blocs);
        fichier->blocs = NULL;
    }

    // libérer struct principale
    free(fichier);
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

int parseComplement(const char* ligne, Operation* op) {

    if (strlen(ligne) < 120) {
        return -1;
    }

    if (ligne[0] != '0' || ligne[1] != '5') {
        return -1;
    }

    if (op->nbComplements >= 5) {
        return -1;  // max 5 compléments
    }

    // extraires infos
    char complement[71];  // 70 caractères + \0
    extraireChamp(ligne, 49, 118, complement);

    // suppr espaces fin
    for (int i = 69; i >= 0 && complement[i] == ' '; i--) {
        complement[i] = '\0';
    }

    strncpy(op->complements[op->nbComplements], complement, 70);
    op->complements[op->nbComplements][70] = '\0';

    op->nbComplements++;

    return 0;
}