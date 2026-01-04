# Compte-rendu de TP
<p>
Compte-rendu commun du projet. 
</p>
<p>
Il détaillera notamment :
</p>
<p>
- Notre analyse des différentes fonctions, pour déterminer quelles fonctions ont besoin de fonctions préalable.<br>
- La répartition des tâches qui en découle.
</p>

## Le besoin client

Le directeur financier de JM Comptabilité souhaite automatiser le traitement de ces relevés. Il vous demande de développer un **outil en ligne de commande** capable de :

- Lire et parser les fichiers CFONB120
- Vérifier l'intégrité et la cohérence des données
- Extraire des statistiques et indicateurs clés


## Flux logique de l'application

- DEBUT
- Récupération du fichier
- Validation du fichier
- Récupérer les informations présentes dans le fichier, et les stocker dans les types appropriés
- Lire les arguments passés
- Pour chaque argument:
- - Récupérer les informations recherchées
- - Renvoyer l'affichage des infos recherchées
- - Afficher les statistiques correspondantes (?)
- FIN / Redemander si on veut des informations (?)

## Diagramme de dépendances des fonctions

__Etape 1:__
```
// Charge un fichier complet
FichierCFONB* chargerFichier(const char* nomFichier);

// Libère la mémoire
void libererFichier(FichierCFONB* fichier);

// Extraction d'une sous-chaîne (positions CFONB sont en base 1)
void extraireChamp(const char* ligne, int debut, int fin, char* dest);

// Parsing d'une date JJMMAA
DateCFONB parseDate(const char* dateStr);

// Conversion du caractère signé en montant
Montant decoderMontant(const char* montantStr, int nbDecimales);

// Affichage formaté d'un montant
void afficherMontant(Montant m);

// Comparaison de dates
int comparerDates(DateCFONB d1, DateCFONB d2);
```
__Etape 2:__
```
// Parse une ligne selon son type
RecordType detecterTypeLigne(const char* ligne);

// Parse un enregistrement 01 ou 07
int parseInfoCompte(const char* ligne, InfoCompte* info);

// Parse un enregistrement 04
int parseOperation(const char* ligne, Operation* op);

// Parse un enregistrement 05 et l'ajoute à l'opération courante
int parseComplement(const char* ligne, Operation* op);

```
__Etape 3:__
```
// Calcule les stats d'un bloc
StatsCompte calculerStatsBloc(BlocCompte* bloc);

// Affiche les stats de tous les comptes
void afficherStatsFichier(FichierCFONB* fichier);

// Recherche des opérations selon critères
Operation** rechercherOperations(FichierCFONB* fichier,
                                  const char* numeroCompte,
                                  long montantMin,
                                  DateCFONB* date,
                                  int* nbResultats); 
```

__Etape 4:__
```
// Valide la structure d'un bloc (séquence 01-04-07)
RapportValidation validerStructureBloc(BlocCompte* bloc);

// Vérifie la cohérence du numéro de compte
RapportValidation validerCoherenceCompte(BlocCompte* bloc);

// Recalcule et vérifie le solde
RapportValidation validerSolde(BlocCompte* bloc);

// Validation complète d'un fichier
RapportValidation* validerFichier(FichierCFONB* fichier, int* nbRapports);

```


## Répartition des tâches

Tristan:
1)
- chargerFichier
- libererFichier
- extraireChamp
- decoderMontant
2)
- parseOperation
- parseComplement
3)
- rechercherOperations
4)
- validerStructureBloc
- validerCoherenceCompte

Miranda:

1)
- parseDate
- afficherMontant
- comparerDate
2)
- detecterInfoCompte
- parseInfoCompte
3)
- calculerStatsBloc
- afficherStatsFichier
4)
- validerSolde
- validerFichier

## Compilation : Instructions pour compiler le projet <br>
## Utilisation : Exemples de commandes <br>
## Fonctionnalités : Liste des fonctionnalités implémentées <br>
## Difficultés rencontrées : Section réflexive (optionnelle mais valorisée) <br>