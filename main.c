#include "plateau_couleurs.h"
#include "plateau_pions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  // creer une sructure? partie qui comprend plateau pions
  // plateau couleurs, nombre de manche, nombre de tour?

  // ici on crée les côtés TABLEAU[cote_numero_en_partant_du_bas][colonne]
  int cotep1[ARETE];
  int cotep2[ARETE];
  int cotep3[ARETE];
  int cotep4[ARETE];
  int cotep5[ARETE];
  int cotep6[ARETE];
  int cotep7[ARETE];
  int cotep8[ARETE];

  int *colonnesp[ARETE] = {cotep1, cotep2, cotep3, cotep4,
                           cotep5, cotep6, cotep7, cotep8};

  int cotec1[ARETE] = {0, 1, 2, 3, 4, 5, 6, 7}; // ici on crée les côtés
  int cotec2[ARETE] = {5, 0, 3, 6, 1, 4, 7, 2};
  int cotec3[ARETE] = {6, 3, 0, 5, 2, 7, 4, 1};
  int cotec4[ARETE] = {3, 2, 1, 0, 7, 6, 5, 4};
  int cotec5[ARETE] = {4, 5, 6, 7, 0, 1, 2, 3};
  int cotec6[ARETE] = {1, 4, 7, 2, 5, 0, 3, 6};
  int cotec7[ARETE] = {2, 7, 4, 1, 6, 3, 0, 5};
  int cotec8[ARETE] = {7, 6, 5, 4, 3, 2, 1, 0};
  int *colonnesc[ARETE] = {cotec1, cotec2, cotec3, cotec4,
                           cotec5, cotec6, cotec7, cotec8};
  // les tableaux d'après sont ceux de la sauvegarde
  int scotep1[ARETE];
  int scotep2[ARETE];
  int scotep3[ARETE];
  int scotep4[ARETE];
  int scotep5[ARETE];
  int scotep6[ARETE];
  int scotep7[ARETE];
  int scotep8[ARETE];

  int *scolonnesp[ARETE] = {cotep1, cotep2, cotep3, cotep4,
                            cotep5, cotep6, cotep7, cotep8};

  int scotec1[ARETE] = {0, 1, 2, 3, 4, 5, 6, 7}; // ici on crée les côtés
  int scotec2[ARETE] = {5, 0, 3, 6, 1, 4, 7, 2};
  int scotec3[ARETE] = {6, 3, 0, 5, 2, 7, 4, 1};
  int scotec4[ARETE] = {3, 2, 1, 0, 7, 6, 5, 4};
  int scotec5[ARETE] = {4, 5, 6, 7, 0, 1, 2, 3};
  int scotec6[ARETE] = {1, 4, 7, 2, 5, 0, 3, 6};
  int scotec7[ARETE] = {2, 7, 4, 1, 6, 3, 0, 5};
  int scotec8[ARETE] = {7, 6, 5, 4, 3, 2, 1, 0};
  int *scolonnesc[ARETE] = {cotec1, cotec2, cotec3, cotec4,
                            cotec5, cotec6, cotec7, cotec8};

  // on fait le tableau de promotions ou  chaque pièce a sa propre valeur
  int prom1[ARETE];
  int prom2[ARETE];
  int prom3[ARETE];
  int prom4[ARETE];
  int prom5[ARETE];
  int prom6[ARETE];
  int prom7[ARETE];
  int prom8[ARETE];
  int *prom[ARETE] = {prom1, prom2, prom3, prom4, prom5, prom6, prom7, prom8};
  // on remplit les case du haut et du bas avec des entiers de 1à8 en haut, de
  // 9à 15 en bas, celles restantes contiennent 0.

  int nb_manche = 4;
  int num_manche = 1;
  int temps = 0;
  int encoursdepartie = 1;  // indique si on est en cours de partie
  int jeu = 1;              // indique si on a lancé le jeu
  int existesauvegarde = 0; // indique s'il existe une sauvegarde
  int score_j1 = 0, score_j2 = 0;
  int dernier_coup = 0; // utile pour trouver quelle pièce promouvoir
  // se compose de 10*ordonnee + abcisse exemple pour e3 = 24
  int existe_prom_j1 = 0;
  int existe_prom_j2 = 0;

  int ordo_promx;
  int abci_promx;
  int ordo_promo;
  int abci_promo;


  time_t temps_début, temps_dif,
      temps_fin; // structure adaptée aux éléments temporels

  time(&temps_dif);

  while (jeu) {
    printf("\n\n\n");
    // printf("\n%f\n\n", difftime(temps_dif, temps_actu));
    int reponse = (menuprincipal());

    if (reponse == 1) {
      printf("\n");
      char joueur1[21] = {"alex"};
      char joueur2[21] = {"michel"};
      
      nb_manche = demande_manche();

      selecjoueur1(joueur1);
      selecjoueur2(joueur2);
      while (num_manche <= nb_manche) {
        lancer_manche(colonnesc, colonnesp, num_manche, nb_manche, 0, &score_j1,
                      &score_j2, joueur1, joueur2, &dernier_coup, prom,
                      &existe_prom_j1, &existe_prom_j2);
        num_manche += 1;
        if ((existe_prom_j1 == 0) || (existe_prom_j2 == 0)) {

          if ((score_j1 == max(score_j1, score_j2)) && (existe_prom_j1 == 0)) {
            existe_prom_j1 = 1;
            //initialisation(colonnesp, num_manche+1);
            //if (existe_prom_j2){
              //colonnesp[ordo_promo][abci_promo]+=2;//pour garder la promotion
            //}
            promouvoir(colonnesp, prom, dernier_coup,&ordo_promx,&abci_promx);
          
          } else if (existe_prom_j2 == 0) {
            existe_prom_j2 = 1;
            //initialisation(colonnesp, num_manche+1);
            //if (existe_prom_j1){
              //colonnesp[ordo_promx][abci_promx]+=2;//pour garder la promotion
            //}
            promouvoir(colonnesp, prom, dernier_coup,&ordo_promo,&abci_promo);
          }
          initialisation(colonnesp,3);
          
          
        }
        //printf("%d %d %d %d", ordo_promx,abci_promx,ordo_promo,abci_promo);
        initialisation(colonnesp,3);
        if (existe_prom_j1){
              colonnesp[ordo_promx][abci_promx]+=2;//pour garder la promotion
            }
          if (existe_prom_j2){
              colonnesp[ordo_promo][abci_promo]+=2;//pour garder la promotion
            }
      }
      if (score_j1 == score_j2) {
        printf("\n\nIl y a égalité, une nouvelle manche décisive va démarrer "
               "!\n\n");
        sleep(2);
        num_manche += 1;
        lancer_manche(colonnesc, colonnesp, num_manche += 1, nb_manche, 0,
                      &score_j1, &score_j2, joueur1, joueur2, &dernier_coup,
                      prom, &existe_prom_j1, &existe_prom_j2);
      }
      if (score_j1 == max(score_j1, score_j2)) {
        printf("\n\n\n%s Gagne la partie, bien joué !\n\n\n", joueur1);
      } else {
        printf("\n\n\n%s Gagne la partie, bien joué !\n\n\n", joueur2);
      }

    } else if (reponse == 2) {
      if (existesauvegarde) {

        existesauvegarde = 0;
      } else {
        printf(" Code erreur #8 : il n'existe pas de sauvegarde\n");
      }
    }

    else {
      printf("\n Code erreur #621 : chiffre invalide.\n\n");
    }
    sleep(2);
  }
  return 0;
}
