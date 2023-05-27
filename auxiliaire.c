#include "plateau_couleurs.h"
#include "plateau_pions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// ce fichier est le garage des fonctions

int lancer_manche(int **colonnec, int **colonnep, int nb_manche, int manche_tt,
                  int temps, int *score_j1, int *score_j2, char *j1, char *j2,
                  int *dernier_coup, int **prom, int *existe_prom_j1,
                  int *existe_prom_j2) {
  time_t temps_debut, temps_fin; // structure adaptée aux éléments temporels
  time(&temps_debut);
  int tour = 1;
  //pour que le joueur qui commence soit celui qui est mené:
  if (*score_j1==*score_j2){
    
  }
  else if(*score_j1 == max(*score_j1, *score_j2)){
  tour+=1;
  }
  
  initialisation(colonnep, nb_manche);
  init_prom(prom);
  int estdeplace = 1;
  
  int enmanche = 1;
  int couleur_active = 0;
  char couleur_txt[25];
  char **joueur_actu;
  int num_j;
  char **joueur_pas_actu;
  while (enmanche) {
    int abcissepion, ordonneepion, abcissedeplace, ordonneedeplace;
    if (estdeplace) {
      num_j = (((tour - 1) % 2) + 1);
      switch (num_j) {
      case 1:
        joueur_actu = &j1;
        joueur_pas_actu = &j2;
        break;
      case 2:
        joueur_actu = &j2;
        joueur_pas_actu = &j1;
        break;
      }
    }
    affichejeu(colonnec, colonnep, nb_manche, manche_tt, *score_j1, *score_j2,
               j1, j2);
    printf("\n\n");
    if (tour < 2) {

      demande_choix_pion(*joueur_actu, num_j, &abcissepion, &ordonneepion,
                         colonnep, colonnec);
    } else {
      if (parcours_couleur(couleur_active, colonnec, colonnep, num_j) == 0) {
        printf("\n\n");
        printf("%s n'a pas de pion sur une case ", *joueur_actu);
        convert_couleur_ent_text(couleur_active, couleur_txt);
        int nvcoul = convertcouleur(couleur_active);
        printf("\e[%dm%s", nvcoul, couleur_txt);
        printf("\e[0m %s doit donc rejouer !", *joueur_pas_actu);
        printf("\n\n");
        tour += 1;
        num_j = (((tour - 1) % 2) + 1);
        switch (num_j) {
        case 1:
          joueur_actu = &j1;
          joueur_pas_actu = &j2;
          break;
        case 2:
          joueur_actu = &j2;
          joueur_pas_actu = &j1;
          break;
        }
        if (parcours_couleur(couleur_active, colonnec, colonnep, num_j) == 0) {
          printf("\n\n");

          time(&temps_fin);
          printf(
              "Les deux joueurs sont bloqués, c'est donc %s qui remporte la "
              "manche en %lf minutes car il a joué en dernier, félicitations !",
              *joueur_pas_actu, (difftime(temps_fin, temps_debut) / 60));
          printf("\n\n");
          if (num_j == 1) {
            if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j1 += 2;
            }
            else{*score_j1 += 1;}
          } else {
            if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j2 += 2;
            }
            else{*score_j2 += 1;}
          }
          return num_j;
        }
      }

      demande_choix_pion_2(*joueur_actu, num_j, &abcissepion, &ordonneepion,
                           colonnep, colonnec, couleur_active);
    }
    estdeplace = 1;
    demande_deplacement_pion(*joueur_actu, num_j, abcissepion, ordonneepion,
                             &abcissedeplace, &ordonneedeplace, colonnep,
                             &estdeplace);
    // printf("\nabcip%d, ordop%d abcid%d ordod%d estdplace%d\n", abcissepion,
    // ordonneepion, abcissedeplace, ordonneedeplace, estdeplace);
    if (estdeplace) {
      if (colonnep[ordonneepion][abcissepion] > 2) { // cas d'une pièce promue
        int vartemp = deplacement_possible_prom(abcissepion, ordonneepion,
                                                abcissedeplace, ordonneedeplace,
                                                colonnep, colonnec, num_j);
        // printf("\nvartemp%d\n", vartemp);
        switch (vartemp) {

        case 0:
          // deplacement impossible

          break;
        case 1:
          // deplacement classique
          printf("\n1\n");
          //(les lignes d'après sont répétées plus tard)
          couleur_active = colonnec[ordonneedeplace][abcissedeplace];
          int pion = colonnep[ordonneepion][abcissepion];
          colonnep[ordonneepion][abcissepion] = 0;
          colonnep[ordonneedeplace][abcissedeplace] = pion;
          // ici se fait le déplacement
          int pion_prom = prom[ordonneepion][abcissepion];
          prom[ordonneepion][abcissepion] = 0;
          prom[ordonneedeplace][abcissedeplace] = pion_prom;
          *dernier_coup = (ordonneedeplace * 10) + abcissedeplace;
          for (int i = 0; i < 8; i++) {
            if ((colonnep[7][i] == 2) || (colonnep[7][i] == 4)) {
              time(&temps_fin);

              affichejeu(colonnec, colonnep, nb_manche, manche_tt, *score_j1,
                         *score_j2, j1, j2);
              printf(
                  "\n\n\n\n\n\n%s a gagné la manche numéro %d en %lf minutes, "
                  "félicitations !",
                  j1, nb_manche, (difftime(temps_fin, temps_debut) / 60));
              sleep(1);
              enmanche = 0;
              if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j1 += 2;
            }
            else{*score_j1 += 1;}
              return num_j;
            }
            if ((colonnep[0][i] == 1) || (colonnep[0][i] == 3)) {
              affichejeu(colonnec, colonnep, nb_manche, manche_tt, *score_j1,
                         *score_j2, j1, j2);
              time(&temps_fin);

              printf(
                  "\n\n\n\n\n\n%s a gagné la manche numéro %d en %lf minutes, "
                  "félicitations !",
                  j2, nb_manche, (difftime(temps_fin, temps_debut) / 60));
              sleep(1);
              enmanche = 0;
              if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j2 += 2;
            }
            else{*score_j2 += 1;}
              return num_j;
            }
          }
          tour += 1;

          break;
        case 2: {
          // pousser une pièce
          int pion_prom = prom[ordonneepion][abcissepion];

          int pion = colonnep[ordonneepion][abcissepion];
          if (num_j == 1) {//joueur des x, pousse vers le bas
            int pion_prom_2 = prom[ordonneepion + 1][abcissepion];
            int pion_2 = colonnep[ordonneepion + 1][abcissepion];//pion poussé
            
    
            colonnep[ordonneedeplace + 1][abcissedeplace] = pion_2;
            prom[ordonneedeplace + 1][abcissedeplace] = pion_prom_2;
            couleur_active = colonnec[ordonneedeplace + 1][abcissedeplace];
          } else {//joueur des o, pousse vers le haut
            int pion_prom_2 = prom[ordonneepion - 1][abcissepion];
            int pion_2 = colonnep[ordonneepion - 1][abcissepion];//pion poussé
                
            colonnep[ordonneedeplace - 1][abcissedeplace] = pion_2;
            prom[ordonneedeplace - 1][abcissedeplace] = pion_prom_2;
            couleur_active = colonnec[ordonneedeplace - 1][abcissedeplace];
          }

          colonnep[ordonneepion][abcissepion] = 0;
          colonnep[ordonneedeplace][abcissedeplace] = pion;
          prom[ordonneepion][abcissepion] = 0;
          prom[ordonneedeplace][abcissedeplace] = pion_prom;
          *dernier_coup = (ordonneedeplace * 10) + abcissedeplace;
          for (int i = 0; i < 8; i++) {
            if ((colonnep[7][i] == 2) || (colonnep[7][i] == 4)) {
              time(&temps_fin);

              affichejeu(colonnec, colonnep, nb_manche, manche_tt, *score_j1,
                         *score_j2, j1, j2);
              printf(
                  "\n\n\n\n\n\n%s a gagné la manche numéro %d en %lf minutes, "
                  "félicitations !",
                  j1, nb_manche, (difftime(temps_fin, temps_debut) / 60));
              sleep(1);
              enmanche = 0;
              if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j1 += 2;
            }
            else{*score_j1 += 1;}
              return num_j;
            }
            if ((colonnep[0][i] == 1) || (colonnep[0][i] == 3)) {
              affichejeu(colonnec, colonnep, nb_manche, manche_tt, *score_j1,
                         *score_j2, j1, j2);
              time(&temps_fin);

              printf(
                  "\n\n\n\n\n\n%s a gagné la manche numéro %d en %lf minutes, "
                  "félicitations !",
                  j2, nb_manche, (difftime(temps_fin, temps_debut) / 60));
              sleep(1);
              enmanche = 0;
              if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j2 += 2;
            }
            else{*score_j2 += 1;}
              return num_j;
            }
          }
          tour += 1;
          break;
        }
        }
      }

      else if (deplacement_possible(abcissepion, ordonneepion, abcissedeplace,
                                    ordonneedeplace, colonnep, colonnec,
                                    num_j)) {
        couleur_active = colonnec[ordonneedeplace][abcissedeplace];

        // convert_couleur_ent_text(couleur_active, couleur_txt);
        // printf("\n%s\n\n", couleur_txt);
        int pion = colonnep[ordonneepion][abcissepion];
        colonnep[ordonneepion][abcissepion] = 0;
        colonnep[ordonneedeplace][abcissedeplace] = pion;
        // ici se fait le déplacement
        int pion_prom = prom[ordonneepion][abcissepion];
        prom[ordonneepion][abcissepion] = 0;
        prom[ordonneedeplace][abcissedeplace] = pion_prom;
        *dernier_coup = (ordonneedeplace * 10) + abcissedeplace;
        for (int i = 0; i < 8; i++) {
          if ((colonnep[7][i] == 2) || (colonnep[7][i] == 4)) {
            time(&temps_fin);

            affichejeu(colonnec, colonnep, nb_manche, manche_tt, *score_j1,
                       *score_j2, j1, j2);
            printf("\n\n\n\n\n\n%s a gagné la manche numéro %d en %lf minutes, "
                   "félicitations !",
                   j1, nb_manche, (difftime(temps_fin, temps_debut) / 60));
            sleep(1);
            enmanche = 0;
            if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j1 += 2;
            }
            else{*score_j1 += 1;}
            return num_j;
          }
          if ((colonnep[0][i] == 1) || (colonnep[0][i] == 3)) {
            affichejeu(colonnec, colonnep, nb_manche, manche_tt, *score_j1,
                       *score_j2, j1, j2);
            time(&temps_fin);

            printf("\n\n\n\n\n\n%s a gagné la manche numéro %d en %lf minutes, "
                   "félicitations !",
                   j2, nb_manche, (difftime(temps_fin, temps_debut) / 60));
            sleep(1);
            enmanche = 0;
            if (colonnep[(*dernier_coup)/10][(*dernier_coup)%10]>2){
              *score_j2 += 2;
            }
            else{*score_j2 += 1;}
            
            return num_j;
          }
        }
        tour += 1;
      }
    }
  }
}

void demande_choix_pion_2(char *nom_joueur, int num_joueur, int *abcisse,
                          int *ordonnee, int **colonnep, int **colonnec,
                          int couleur_active) {
  int abcissereelle = 0;
  int nvlordonnee = 0;
  int boucle = 1;
  char couleur_txt[25];
  int nvcoul = convertcouleur(couleur_active);
  convert_couleur_ent_text(couleur_active, couleur_txt);
  while (boucle) {
    char typepion = 'e';
    if (num_joueur == 1) {
      typepion = 'x';
    } else {
      typepion = 'o';
    }
    char nvlabcisse = 'a';

    printf("C'est le tour de %s, quelle pièce ", nom_joueur);
    printf("\e[%dm%s", nvcoul, couleur_txt);
    printf("\e[0m souhaitez vous deplacer ?");
    printf("\n(vous en possedez %d actuellement) ",
           parcours_couleur(couleur_active, colonnec, colonnep, num_joueur));
    printf("\n\nPour "
           "rappel, vous controlez les %c.\nVeuillez écrire les "
           "coordonnées de "
           "la pièce sous le format : \n'LettreChiffre' exemple : C8\n",
           typepion);
    scanf("%c%d", &nvlabcisse, &nvlordonnee);
    while ('\n' != (getchar()))
      ;
    nvlordonnee -= 1;
    abcissereelle = (nvlabcisse - 97 + 32);
    int pion = (colonnep[nvlordonnee][abcissereelle]);
    if ((abcissereelle < 0) || (abcissereelle > 7) || (nvlordonnee < 0) ||
        (nvlordonnee > 7)) {
      printf("\n Code erreur #101 : format invalide.\n\n");

    }

    else if ((((pion != 1) && (pion != 3)) && (num_joueur == 2)) ||
             (((pion != 2) && (pion != 4)) && (num_joueur == 1))) {
      printf("\n%d %d %d\n", nvlordonnee, abcissereelle, pion);
      printf("\nCe pion n'est pas vôtre !\n");
    } else if (colonnec[nvlordonnee][abcissereelle] != couleur_active) {
      // printf("\n%d %d %d %d %d\n", nvlordonnee, abcissereelle, pion,
      // colonnec[nvlordonnee][abcissereelle], couleur_active);
      printf("\nCe pion n'est pas de la bonne couleur !\n");
    } else {
      boucle = 0;
    }
  }
  *abcisse = abcissereelle;
  *ordonnee = nvlordonnee;
}

void demande_choix_pion(char *nom_joueur, int num_joueur, int *abcisse,
                        int *ordonnee, int **colonnep, int **colonnec) {
  int abcissereelle = 0;
  int nvlordonnee = 0;
  int boucle = 1;
  while (boucle) {
    char typepion = 'e';
    if (num_joueur == 1) {
      typepion = 'x';
    } else {
      typepion = 'o';
    }
    char nvlabcisse = 'a';

    printf("C'est le tour de %s, quelle pièce souhaitez vous deplacer "
           "?\n\nPour "
           "rappel, vous controlez les %c.\nVeuillez écrire les "
           "coordonnées de "
           "la pièce sous le format : \n'LettreChiffre' exemple : C8\n",
           nom_joueur, typepion);
    scanf("%c%d", &nvlabcisse, &nvlordonnee);
    while ('\n' != (getchar()))
      ;
    nvlordonnee -= 1;
    abcissereelle = (nvlabcisse - 97 + 32);
    int pion = (colonnep[nvlordonnee][abcissereelle]);
    if ((abcissereelle < 0) || (abcissereelle > 7) || (nvlordonnee < 0) ||
        (nvlordonnee > 7)) {
      printf("\n Code erreur #101 : format invalide.\n\n");

    }

    else if ((((pion != 1) && (pion != 3)) && (num_joueur == 2)) ||
             (((pion != 2) && (pion != 4)) && (num_joueur == 1))) {
      // printf("\n%d %d %d\n", nvlordonnee, abcissereelle, pion);
      printf("\nCe pion n'est pas vôtre !\n");
    } else {
      boucle = 0;
    }
  }
  *abcisse = abcissereelle;
  *ordonnee = nvlordonnee;
}

int deplacement_possible(int abcissepion, int ordonneepion, int abcissedeplace,
                         int ordonneedeplace, int **colonnep, int **colonnec,
                         int num_joueur) {
  // cette fonction renvoi 1 si le deplacement est possible, 0 sinon
  int possible = 0, boucle = 1;
  if (num_joueur == 1) { // joueur des x
    if (ordonneepion > ordonneedeplace) {
      printf("\nIl est impossible de reculer !\n");
      return 0;
    }

    if ((abcissepion == abcissedeplace) &&
        (ordonneepion != ordonneedeplace)) { // déplacement purement vertical
      for (int i = (ordonneepion + 1); i <= ordonneedeplace; i = i + 1) {
        if ((colonnep[i][abcissepion]) != 0) {
          printf("\nIl y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
  } else { // joueur des o
    if (ordonneepion < ordonneedeplace) {
      printf("\nIl est impossible de reculer !\n");
      return 0;
    }
    if ((abcissepion == abcissedeplace) &&
        (ordonneepion != ordonneedeplace)) { // déplacement purement vertical
      for (int i = (ordonneepion - 1); i >= ordonneedeplace; i = i - 1) {
        if ((colonnep[i][abcissepion]) != 0) {
          printf("\nIl y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
  }

  if ((abcissepion != abcissedeplace) &&
      (ordonneepion == ordonneedeplace)) { // déplacement purement horizontal
    if ((abcissepion - abcissedeplace) > 0) { // déplacement de gauche à droite
      for (int i = (abcissepion + 1); i <= abcissedeplace; i = i + 1) {
        if ((colonnep[ordonneepion][i]) != 0) {
          printf("\nIl y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    } else { // déplacement de droite à gauche
      for (int i = (abcissepion - 1); i >= abcissedeplace; i = i - 1) {
        if ((colonnep[ordonneepion][i]) != 0) {
          printf("\nIl y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
  } else if ((abcissepion != abcissedeplace) ||
             (ordonneepion == ordonneedeplace)) { // déplacement diagonal
    int dif_abs = (abcissepion - abcissedeplace),
        dif_ord = (ordonneepion - ordonneedeplace);
    if (abs(dif_abs) != abs(dif_ord)) {
      printf("\nDéplacement illicite ! \n\n");
      return 0;
    }
    if ((dif_abs < 0) && (dif_ord < 0)) // deplacement haut gauche > bas droit
    {
      for (int i = abcissepion + 1; i <= abcissedeplace; i++) {
        if (colonnep[ordonneepion + i - abcissepion][i] != 0) {
          printf("\n1Il y a une pièce en travers de la route!\n");
          return 0;
        }
      }
    }
    if ((dif_abs < 0) && (dif_ord > 0)) // deplacement bas gauche > haut droit
    {
      for (int i = abcissepion + 1, b = ordonneepion - 1; i <= abcissedeplace;
           i++, b = b - 1) {
        if (colonnep[b][i] != 0) {
          printf("\n2Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
    if ((dif_abs > 0) && (dif_ord > 0)) // deplacement bas DROIT > haut gauche
    {
      for (int i = abcissepion - 1, b = ordonneepion - 1; i >= abcissedeplace;
           i = i - 1, b = b - 1) {
        if (colonnep[b][i] != 0) {
          printf("\n3Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
    if ((dif_abs > 0) && (dif_ord < 0)) // deplacement haut DROIT > bas gauche
    {
      for (int i = abcissepion - 1, b = ordonneepion + 1; i >= abcissedeplace;
           i = i - 1, b = b - 1) {
        if (colonnep[b][i] != 0) {
          printf("\n4Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
  }

  return 1;
}

void demande_deplacement_pion(char *nom_joueur, int num_joueur,
                              int abcisse_debut, int ordonnee_debut,
                              int *abcisse, int *ordonnee, int **colonnep,
                              int *estdeplace) {
  int abcissereelle = 0;
  int nvlordonnee = 0;
  int boucle = 1;
  while (boucle) {
    char typepion = 'e';
    if (num_joueur == 1) {
      typepion = 'x';
    } else {
      typepion = 'o';
    }
    char nvlabcisse = 'a';
    //printf("\n%d\n", colonnep[ordonnee_debut][abcisse_debut]);
    if (colonnep[ordonnee_debut][abcisse_debut] > 2) {
      printf("Où desirez vous emmener la pièce promue en %c%d ?\n\n"
             "Vous pouvez vous la déplacer au maximum de 5 cases\n"
             "mais vous disposez également de la possibilité de pousser\n"
             "une pièce (non promue) située juste devant, \n"
             "pour cela indiquez les coordonnées de la pièce à bousculer."
             "\n\nVeuillez écrire les "
             "coordonnées de la case cible sous le format : \n'LettreChiffre'"
             "exemple : C8\n",
             (abcisse_debut + 97 - 32), (ordonnee_debut + 1));
    } else {
      printf("Où desirez vous emmener le pion en %c%d ?\n\nVeuillez écrire "
             "les "
             "coordonnées de la case cible sous le format : \n'LettreChiffre' "
             "exemple : C8\n",
             (abcisse_debut + 97 - 32), (ordonnee_debut + 1));
    }
    scanf("%c%d", &nvlabcisse, &nvlordonnee);
    while ('\n' != (getchar()))
      ;
    nvlordonnee -= 1;
    abcissereelle = (nvlabcisse - 97 + 32);
    if ((abcissereelle == abcisse_debut) && (ordonnee_debut == nvlordonnee)) {
      printf("\nDéplacement annulé\n\n");
      *estdeplace = 0;
      boucle = 0;
    } else if ((abcissereelle < 0) || (abcissereelle > 7) ||
               (nvlordonnee < 0) || (nvlordonnee > 7)) {
      printf("\n Code erreur #101 : format invalide.\n\n");
    } else {

      boucle = 0;
    }
  }
  *abcisse = abcissereelle;
  *ordonnee = nvlordonnee;
}

int menuprincipal() {
  int rep = 0;
  printf(" Pour commencer une nouvelle partie : écrivez 1.\n Pour "
         "reprendre la "
         "partie sauvegardée : écrivez 2.\n");
  scanf("%d", &rep);
  while ('\n' != (getchar()))
    ; // règle les problemes de chaîne de caractères
  return rep;
}

void selecjoueur1(char *tab) {
  printf("Veuillez écrire le nom du joueur des croix,\nce dernier aura le "
         "privilège de commencer:\n(maximum 20 caractères)\nAppuyez deux fois "
         "sur la touche entrée\n\n");
  fgets(tab, 20, stdin);
  tab[(strlen(tab) - 1)] = '\0';
  while ('\n' != (getchar()))
    ;
}

void selecjoueur2(char *tab) {
  printf("Veuillez écrire le nom du joueur des ronds:\n(maximum 20 "
         "caractères)\nAppuyez deux fois sur la touche entrée\n\n");
  fgets(tab, 20, stdin);
  tab[(strlen(tab) - 1)] = '\0';
  while ('\n' != (getchar()))
    ;
}

int convertcouleur(int couleur)
// transforme le chiffre de la couleur passée en argumen en un
// autre chiffre affichant la bonne couleur sur le terminal
{
  int nouvcoul = 0;
  switch (couleur) {
  case 0:
    nouvcoul = 41;
    break;
  case 1:
    nouvcoul = 42;
    break;
  case 2:
    nouvcoul = 44;
    break;
  case 3:
    nouvcoul = 45;
    break;
  case 4:
    nouvcoul = 103;
    break;
  case 5:
    nouvcoul = 46;
    break;
  case 6:
    nouvcoul = 107;
    break;
  case 7:
    nouvcoul = 100;
    break;
  }
  return (nouvcoul);
}

void affichecouleurs(int **colonne) {
  for (int col = 0; col < ARETE; col++) {
    for (int cot = 0; cot < ARETE; cot++) {
      int nvlcoul = (convertcouleur(colonne[col][cot]));
      printf("\e[%dm %d", nvlcoul, colonne[col][cot]);
    }
    printf("\n");
  }
}

void affichejeu(int **colonnec, int **colonnep, int nb_manche, int manche_tt,
                int score_j1, int score_j2, char *j1, char *j2) {
  // erreur ici lati est plutrot la longitude
  printf("\n");
  for (int lati = 0; lati < ARETE; lati++) {
    for (int cot = 0; cot < ARETE; cot++) {
      int nvlcoul = (convertcouleur(colonnec[lati][cot]));
      char signepion = renvoi_signepion(colonnep[lati][cot]);
      int coulpion = renvoi_coul_pion(
          colonnep[lati][cot]); // les couleurs fonctionnent pas trop
      printf("\e[%d;%dm %c ", coulpion, nvlcoul, signepion);
    }
    printf("\e[0m %d ", (lati + 1));
    if (lati == 2) {
      printf("   score de %s = %d", j1, score_j1);
    }
    if (lati == 3) {
      printf("   score de %s = %d", j2, score_j2);
    }
    if (lati == 4) {
      printf("   manche numéro %d/%d ", nb_manche, manche_tt);
    }
    printf("\n");
  }
  for (int lati = 0; lati < ARETE; lati++) {
    printf("\e[0m %c ", (lati + 97 - 32));
  }
}

int *initialisation(int **colonne, int num_manche) {
  for (int lati = 0; lati < ARETE; lati++) { // lati pour latitude
    if (colonne[0][lati] != 4) {
      colonne[0][lati] = 2; // on remplit les cases du haut par des croix
    }
    if (num_manche == 1) {
      colonne[0][lati] = 2;
    }
  }
  for (int lati = 0; lati < ARETE; lati++) {
    if (colonne[7][lati] != 3) {
      colonne[7][lati] = 1; // on remplit les cases du bas par des ronds
    }
    if (num_manche == 1) {
      colonne[7][lati] = 1;
    }
  }
  for (int col = 1; col < (ARETE - 1); col++) {
    for (int cot = 0; cot < ARETE; cot++) {
      colonne[col][cot] = 0;
      // on vide toutes les autres cases
    }
  }
}

int demande_manche() {
  int nb_manche = 0, boucle = 1;

  while (boucle) {
    printf("\nDe combien de manches desirez vous structurer votre partie "
           "?\nVeuillez entrer un nombre écrit en chiffres.\n\n");
    scanf("%d", &nb_manche);
    while ('\n' != (getchar()))
      ;
    if (nb_manche <= 0) {
      printf("Le nombre de manches doit être supérieur à 0!\n\n");
    }

    else {
      boucle = 0;
    }
  }
  return nb_manche;
}

char renvoi_signepion(int pion) {
  // renvoi le signe du pion
  // de pion
  char renvoi = 32;
  switch (pion) {
  case 0:
    renvoi = 32;
    break;

  case 1:
    renvoi = 111;
    break;
  case 2:
    renvoi = 120;
    break;
  case 3:
    renvoi = 79;
    break;
  case 4:
    renvoi = 88;
    break;
  }
  return (renvoi);
}
int renvoi_coul_pion(
    int pion) { // permet d'afficher la couleur d' un pion partir de l'indic
  // pas utile
  int renvoi = 0;
  switch (pion) {
  case 0:
    renvoi = 30;
    break;

  case 1:
    renvoi = 97;
    break;
  case 2:
    renvoi = 30;
    break;
  case 3:
    renvoi = 97;
    break;
  case 4:
    renvoi = 30;
    break;
  }
  return renvoi;
}

void convert_couleur_ent_text(int couleur, char *rep) {
  switch (couleur) {
  case 0:
    strcpy(rep, "rouge");
    break;
  case 1:
    strcpy(rep, "vert");
    break;
  case 2:
    strcpy(rep, "bleu de France(clair)");
    break;
  case 3:
    strcpy(rep, "violet");
    break;
  case 4:
    strcpy(rep, "Jaune");
    break;
  case 5:
    strcpy(rep, "bleu paon (turquoise)");
    break;
  case 6:
    strcpy(rep, "blanc");
    break;
  case 7:
    strcpy(rep, "noir");
    break;
  }
}

int max(int ent1, int ent2) {
  if (ent1 >= ent2) {
    return ent1;
  } else {
    return ent2;
  }
}

int parcours_couleur(int couleur, int **colonnec, int **colonnep,
                     int num_joueur) {

  int compteur = 0;
  for (int ordonnee = 0; ordonnee < 8; ordonnee++) {
    for (int abcisse = 0; abcisse < 8; abcisse++) {
      if (colonnec[ordonnee][abcisse] == couleur) {
        if (num_joueur == 1) {
          if ((colonnep[ordonnee][abcisse] == 2) ||
              (colonnep[ordonnee][abcisse] == 4)) {
            compteur += 1;
          }
        } else {
          if ((colonnep[ordonnee][abcisse] == 1) ||
              (colonnep[ordonnee][abcisse] == 3)) {
            compteur += 1;
          }
        }
      }
    }
  }
  // printf("%d %d", couleur, compteur);
  return compteur;
}

void init_prom(int **prom) {
  for (int abcisse = 0; abcisse < 8; abcisse++) {
    prom[0][abcisse] = (abcisse + 1);
    prom[7][abcisse] = (abcisse + 9);
  } // on remplit les case du haut et du bas avec des entiers de 1à8 en
    // haut, de 9à 15 en bas

  for (int ordonnee = 1; ordonnee < 7; ordonnee++) {
    for (int abcisse = 0; abcisse < 8; abcisse++) {
      prom[ordonnee][abcisse] = 0;
    }
  }
}

// la procédure promouvoir promeut la pièce ayant offert la victoire
void promouvoir(int **colonnep, int **prom, int dernier_coup,int* ordo_prom, int* abci_prom) {

  int ordo = dernier_coup / 10;
  int abci = dernier_coup % 10;

  int indice = prom[ordo][abci];
  initialisation(colonnep, 3);
  init_prom(prom);
  if (indice < 9) {
    for (int abcisse = 1; abcisse < 9; abcisse++) {
      if (indice == abcisse) {
        //colonnep[0][(abcisse - 1)] += 2;
        *ordo_prom=0;
        *abci_prom=abcisse-1;
        printf("\n\nLa pièce en %c1 a été promue !\n\n", abcisse - 1 + 89 - 24);
      }
    }
  } else {
    for (int abcisse = 9; abcisse < 17; abcisse++) {
      if (indice == abcisse) {
        //colonnep[7][(abcisse - 9)] += 2;
        *ordo_prom=7;
        *abci_prom=abcisse-9;
        printf("\n\nLa pièce en %c8 a été promue !\n\n", abcisse - 1 + 89 - 32);
      }
    }
  }
}

// la fonction ci-dessous est une variation de deplacement possible adaptée
// aux actions des pièces promues, elle renvoie 0 si le deplacement est
// impossible, 1 si c'est un déplacement possible et 2 si il s'agit de
// pousser une pièce.
int deplacement_possible_prom(int abcissepion, int ordonneepion,
                              int abcissedeplace, int ordonneedeplace,
                              int **colonnep, int **colonnec, int num_joueur) {
  int possible = 0, boucle = 1;
  if (num_joueur == 1) { // joueur des x
    if (ordonneepion > ordonneedeplace) {
      printf("\nIl est impossible de reculer !\n");
      return 0;
    }
    if ((abs(ordonneepion - ordonneedeplace)) > 5) {
      printf("\nDéplacement trop long !\n");
      return 0;
    }

    if (((ordonneedeplace - ordonneepion) == 1) &&
        (colonnep[ordonneedeplace][abcissedeplace] != 0)) { // pousser une pièce
      if (colonnep[ordonneedeplace][abcissedeplace] == 3) {
        printf("\nIl est impossible de pousser une pièce promue !\n");
        return 0;
      }
      if (colonnep[ordonneedeplace][abcissedeplace] == 2) {
        printf("\nIl est impossible de pousser une de vos pièces !\n");
        return 0;
      }
      if (ordonneedeplace == 7) {
        printf("\nIl est impossible de pousser une pièce hors du plateau!\n");
        return 0;
      }
      if (colonnep[ordonneedeplace + 1][abcissedeplace] != 0) {
        printf("\nIl est impossible de pousser plus d'une pièce!\n");
        return 0;
      }

      return 2;
    }

    if ((abcissepion == abcissedeplace) &&
        (ordonneepion != ordonneedeplace)) { // déplacement purement vertical
      for (int i = (ordonneepion + 1); i <= ordonneedeplace; i = i + 1) {
        if ((colonnep[i][abcissepion]) != 0) {
          printf("\n1Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
  } else { // joueur des o
    if (ordonneepion < ordonneedeplace) {
      printf("\nIl est impossible de reculer !\n");
      return 0;
      }
      //printf("\nABCI%d ORDO%d PION%d NUMJ%d difordo%d\n",abcissedeplace,ordonneedeplace,colonnep[abcissepion][ordonneedeplace],num_joueur,(ordonneepion - ordonneedeplace));
      if (((ordonneepion - ordonneedeplace) == 1) &&
          (colonnep[ordonneedeplace][abcissedeplace] != 0)) { // pousser une pièce
        //printf("\npousser1\n");
        if (colonnep[ordonneedeplace][abcissedeplace] == 4) {
          printf("\n2Il est impossible de pousser une pièce promue !\n");
          return 0;
        }
        if ((colonnep[ordonneedeplace][abcissedeplace]) == 1) {
          printf("\n3Il est impossible de pousser une de vos pièces !\n");
          return 0;
        }
        if (ordonneedeplace == 0) {
          printf("\n4Il est impossible de pousser une pièce hors du "
                 "plateau!\n");
          return 0;
        }
        if (colonnep[ordonneedeplace - 1] [abcissedeplace] != 0) {
          printf("\n5Il est impossible de pousser plus d'une pièce!\n");
          return 0;
        }
        return 2;
      }
    
    if ((abcissepion == abcissedeplace) &&
        (ordonneepion != ordonneedeplace)) { // déplacement purement vertical
      for (int i = (ordonneepion - 1); i >= ordonneedeplace; i = i - 1) {
        if ((colonnep[i][abcissepion]) != 0) {
          printf("\n6Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
    if ((abs(ordonneepion - ordonneedeplace)) > 5) {
      printf("\nDéplacement trop long !\n");
      return 0;
    }
  }

  if ((abcissepion != abcissedeplace) &&
      (ordonneepion == ordonneedeplace)) { // déplacement purement horizontal
    if ((abs(abcissepion - abcissedeplace)) > 5) {
      printf("\nDéplacement trop long !\n");
      return 0;
      if ((abcissepion - abcissedeplace) >
          0) { // déplacement de gauche à droite
        for (int i = (abcissepion + 1); i <= abcissedeplace; i = i + 1) {
          if ((colonnep[ordonneepion][i]) != 0) {
            printf("\n7Il y a une pièce en travers de la route !\n");
            return 0;
          }
        }
      } else { // déplacement de droite à gauche
        for (int i = (abcissepion - 1); i >= abcissedeplace; i = i - 1) {
          if ((colonnep[ordonneepion][i]) != 0) {
            printf("\n8Il y a une pièce en travers de la route !\n");
            return 0;
          }
        }
      }
    }
  } else if ((abcissepion != abcissedeplace) ||
             (ordonneepion == ordonneedeplace)) { // déplacement diagonal
    int dif_abs = (abcissepion - abcissedeplace),
        dif_ord = (ordonneepion - ordonneedeplace);
    if (abs(dif_abs) > 5) {
      printf("\nDéplacement trop long !\n");
      return 0;
    }
    if (abs(dif_abs) != abs(dif_ord)) {
      printf("\nDéplacement illicite ! \n\n");
      return 0;
    }
    if ((dif_abs < 0) && (dif_ord < 0)) // deplacement haut gauche > bas droit
    {
      for (int i = abcissepion + 1; i <= abcissedeplace; i++) {
        if (colonnep[ordonneepion + i - abcissepion] != 0) {
          printf("\n9Il y a une pièce en travers de la route!\n");
          return 0;
        }
      }
    }
    if ((dif_abs < 0) && (dif_ord > 0)) // deplacement bas gauche > haut droit
    {
      for (int i = abcissepion + 1, b = ordonneepion - 1; i <= abcissedeplace;
           i++, b = b - 1) {
        if (colonnep[b][i] != 0) {
          printf("\n10Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
    if ((dif_abs > 0) && (dif_ord > 0)) // deplacement bas DROIT > haut gauche
    {
      for (int i = abcissepion - 1, b = ordonneepion - 1; i >= abcissedeplace;
           i = i - 1, b = b - 1) {
        if (colonnep[b][i] != 0) {
          printf("\n11Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
    if ((dif_abs > 0) && (dif_ord < 0)) // deplacement haut DROIT > bas gauche
    {
      for (int i = abcissepion - 1, b = ordonneepion + 1; i >= abcissedeplace;
           i = i - 1, b = b - 1) {
        if (colonnep[b][i] != 0) {
          printf("\n12Il y a une pièce en travers de la route !\n");
          return 0;
        }
      }
    }
  }

  return 1;
}





