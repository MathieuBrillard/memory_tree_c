#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* définition de la structure d'un noeud */
typedef struct structElement {
    int valeur;
    struct structElement *gauche;
    struct structElement *droite;
} Element;

/* fonction pour initialiser un nouveau noeud */
Element *init() {
    Element *racine;
    racine = malloc(sizeof(Element)); /* allocation mémoire */
    racine->gauche = NULL;
    racine->droite = NULL;

    return(racine);
}

/* fonction pour entrer de nouvelles valeurs */
void push(Element **racine, int nb) {
    if (nb <= (*racine)->valeur) { /* si la valeur est <= à la valeur du noeud actuel */
        if ((*racine)->gauche == NULL) { /* s'il n'existe pas de noeud à gauche */
            Element *nouveau = init(); /* on en créée un nouveau */
            nouveau->valeur = nb; /* on lui attribue sa valeur */
            (*racine)->gauche = nouveau; /* on stock ce nouvel élément à l'adresse gauche */
        }
        else /* s'il existe un noeud */
            push(&((*racine)->gauche), nb); /* on passe à celui-ci */
    }
    else { /* même chose pour la droite */
        if ((*racine)->droite == NULL) {
            Element *nouveau = init();
            nouveau->valeur = nb;
            (*racine)->droite = nouveau;
        }
        else
            push(&((*racine)->droite), nb);
    }
}

/* fonction d'affichage */
int pop(Element **racine, int profondeur) {
    /* gestion de la profondeur avec des espaces */
    for(int i=0;i<=profondeur;i++)
        printf(" ");
    /* affiche de la valeur du noeud actuel */
    printf("- %d\n", (*racine)->valeur);

    if ((*racine)->gauche != NULL) { /* si il y a un noeud à gauche */
        profondeur++;
        pop(&((*racine)->gauche), profondeur); /* récursivité avec le noeud suivant à gauche */
        profondeur--;
    }
    else { /* sinon */
        for(int i=0;i<=profondeur;i++) /* gestion de la profondeur avec des espaces */
            printf(" ");
        printf("-\n"); /* pour les "fils" vide */
    }

    if ((*racine)->droite != NULL){ /*si il y a un noeud à droite */
        profondeur++;
        pop(&((*racine)->droite), profondeur); /* récursivité avec le noeud suivant à droite */
        profondeur--;
    }
    else { /* sinon */
        for(int i=0;i<=profondeur;i++) /* gestion de la profondeur avec des espaces */
            printf(" ");
        printf("-\n"); /* pour les "fils" vide */
    }
    /* return (0); si on compile avec les tags -Wall -Werror -Wextra, gcc retourne une erreur :
        error: control reaches end of non-void function [-Werror=return-type]
        je ne comprends pas parce qu'une fonction void est censé ne rien renvoyé non ?
        En tout cas, en rajoutant ce return, il n'y a plus d'erreur. */
}

/* fonction pour libérer l'espace mémoires */
void destruct(Element **racine) {
    if ((*racine)->gauche != NULL) /* si il y a un noeud à gauche */
        destruct(&((*racine)->gauche)); /* récursivité avec le noeud suivant à gauche */
    if ((*racine)->droite != NULL) /* si il y a un noeud à droite */
        destruct(&((*racine)->droite)); /* récursivité avec le noeud suivant à droite */
    free(racine);
}

int main() {
    /* initialisation du noeud racine de l'arbre */
    Element *racine = init();

    /* pour tester le programme : */
    /* pour générer des nombres aléatoires */
    time_t t;
    srand((unsigned) time(&t));

    int tab[15]; /* création d'un tableau pour stocker ces valeurs */

    /* génération des nombres */
    for(int i=0;i<15;i++)
        tab[i] = rand() % 100;

    racine->valeur = tab[0]; /* on stock la première valeur dans le noeud racine */

    for(int i=1;i<15;i++) /* génération des noeuds et affectation des valeurs */
        push(&racine, tab[i]);

    /* affichage de toutes les valeurs utilisées dans l'arbre */
    printf("Insertion des valeurs : ");
    for(int i=0;i<15;i++)
        printf("%d ", tab[i]);
    printf("\n");

    /* Affichage de l'arbre */
    pop(&racine, 0);

    /* Libération de l'espace mémoire utilisé */
    destruct(&racine);

    return (0);
}