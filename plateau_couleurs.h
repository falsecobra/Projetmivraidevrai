#include <stdio.h>
#include <stdlib.h>
#define ARETE 8

// il y a 8 couleurs on va associer un entier à chaque couleur on superposera
// les pions
// 0=rouge
// 1=vert
// 2=bleu de France (clair)
// 3=rose
// 4=jaune
// 5=bleu paon (turquoise)
// 6=gris clair
// 7=Gris foncé

void creer_sauvegarde(int tour);

void affichejeu(int **colonnec, int **colonnep, int nb_manche, int manche_tt,
                int score_j1, int score_j2, char *j1, char *j2);

int convertcouleur(int couleur);
//  transforme le chiffre de la couleur passée en argumen en un
//  autre chiffre affichant la bonne couleur sur le terminal

void affichecouleurs(int **colonne);

void demande_jeu(char *nom_joueur, int num_joueur, int *abcisse, int *ordonnee);

void convert_couleur_ent_text(int couleur, char *rep);
//  transforme le chiffre de la couleur passée en argumen en son nom français

int parcours_couleur(int couleur, int **colonnec, int **colonnep,
                     int num_joueur);
// détermine le nombre de pions du joueur sur la couleur indiquée