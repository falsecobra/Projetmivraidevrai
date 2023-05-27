#include <stdio.h>
#include <stdlib.h>
#define ARETE 8

// il y a 8 pions du haut des croix(valeur=2) 8pions du bas des o
// (valeur=1) si une pièce est promue elle sera en majuscule vaudra 3 si rond,
//  si 4 majuscule croix, une case vide = 0

// pour trouver la pièce a promouvoir on dispose de l'entier dernier coup qui
// garde le dernier coup joué, ainsi que d'un tableau de tableau nommé "prom"
// qui assigne à chaque pion un entier unique, une signature en quelque sorte,
// ce tableau est modifié de la même façon que le tableau des pions ce qui nous
// permet, à la fin du tour, de savoir quelle pièce est à promouvoir.

int *initialisation(int **colonne, int num_manche);

int lancer_manche(int **colonnes, int **colonnep, int nb_manche, int manche_tt,
                  int temps, int *score_j1, int *score_j2, char *j1, char *j2,
                  int *dernier_coup, int **prom, int *existe_prom_j1,
                  int *existe_prom_j2,int ordo_promx,int abci_promx,int ordo_promo,int abci_promo);

char renvoi_signepion(int pion);

int renvoi_coul_pion(
    int pion); // permet d'afficher la couleur d'un pion partir de l'indice

int menuprincipal();

void selecjoueur1(char *tab);

void selecjoueur2(char *tab);

void demande_choix_pion(char *nom_joueur, int num_joueur, int *abcisse,
                        int *ordonnee, int **colonnep, int **colonnec);
//renvoie 1 si il y a une sauvegarde
//à utiliser pour le premier tour
void demande_deplacement_pion(char *nom_joueur, int num_joueur,
                              int abcisse_debut, int ordonnee_debut,
                              int *abcisse, int *ordonnee, int **colonnep,
                              int *estdeplace);


int deplacement_possible(int abcissepion, int ordonneepion, int abcissedeplace,
                         int ordonneedeplace, int **colonnep, int **colonnec,
                         int num_joueur);

int demande_manche();

void demande_choix_pion_2(char *nom_joueur, int num_joueur, int *abcisse,
                          int *ordonnee, int **colonnep, int **colonnec,
                          int couleur_active);
//à utiliser pour les tours autre que le premier tour //renvoie 1 si il y a une sauvegarde

int max(int ent1, int ent2);

void init_prom(int **prom); // remplit ou réinitialise le tableau prom

void promouvoir(int **colonnep, int **prom, int dernier_coup,int* ordo_prom, int* abci_prom);
// la procédure promouvoir promeut la pièce ayant offert la victoire

int deplacement_possible_prom(int abcissepion, int ordonneepion,
                              int abcissedeplace, int ordonneedeplace,
                              int **colonnep, int **colonnec, int num_joueur);

int reprendre_manche(int **colonnec, int **colonnep, int nb_manche, int manche_tt,
                  int temps, int *score_j1, int *score_j2, char *j1, char *j2,
                  int *dernier_coup, int **prom, int *existe_prom_j1,
                  int *existe_prom_j2);//reprend une partie