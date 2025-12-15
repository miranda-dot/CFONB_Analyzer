//
// Created by i2202852 on 15/12/2025.
//
#ifndef cfonb_types
#define cfonb_types
// Types d'enregistrements CFONB
typedef enum {
    RECORD_OLD_BALANCE = 1,    // 01 - Ancien solde
    RECORD_OPERATION = 4,      // 04 - Opération
    RECORD_COMPLEMENT = 5,     // 05 - Complément
    RECORD_NEW_BALANCE = 7     // 07 - Nouveau solde
} RecordType;

// Sens de l'opération
typedef enum {
    SENS_CREDIT,    // Entrée d'argent
    SENS_DEBIT      // Sortie d'argent
} SensOperation;

// Codes retour pour la validation
typedef enum {
    VALID_OK = 0,
    VALID_ERR_SEQUENCE,       // Séquence 01-04-07 non respectée
    VALID_ERR_COMPTE,         // Numéro de compte incohérent
    VALID_ERR_SOLDE,          // Solde recalculé != solde déclaré
    VALID_ERR_FORMAT          // Erreur de format
} ValidationResult;

#endif