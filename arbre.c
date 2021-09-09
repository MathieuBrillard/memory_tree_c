#include <stdio.h> /* pour la fonction printf */
#include <stdlib.h> /* function : srand & rand */
#include <time.h> /* function : time */
#include <string.h> /* function : strcmp (bonus) */

/* prototypage des fonction
Element *init(void);
void init_list(List **first_elem);
void push(List **pile, int valeur);
int pop(List **pile);
void tree_to_list(Element **racine, List **pile);
void suppr_node(Element **racine, Element **node, List **pile, Element **last_node, int value);
void add_node(Element **racine, int nb);
int print_tree(Element **racine, int profondeur);
void destruct(Element **racine);
void destruct_pile(List *pile);
int wanna_suppr(void); */


/* définition de la structure d'un noeud */
typedef struct structElement {
    int valeur;
    struct structElement *gauche;
    struct structElement *droite;
} Element;

/* fonction pour initialiser un nouveau noeud */
Element *init(void) {
    Element *racine;
    racine = malloc(sizeof(Element)); /* allocation mémoire */
    racine->gauche = NULL;
    racine->droite = NULL;

    return(racine);
}

/* définition de la structure pour la liste chainee */
typedef struct structList {
    int valeur;
    struct structList *precedent;
} List;

/* fonction pour initialiser la liste chainee */
void init_list(List **first_elem) {
    *first_elem = malloc(sizeof(List)); /* allocation mémoire */
    (*first_elem)->precedent = NULL; /* Sert de repère pour savoir que c'est le premier élément */
}

/* fonction pour libérer l'espace mémoires */
void destruct(Element **racine) {
    if ((*racine)->gauche != NULL) /* si il y a un noeud à gauche */
        destruct(&((*racine)->gauche)); /* récursivité avec le noeud suivant à gauche */
    if ((*racine)->droite != NULL) /* si il y a un noeud à droite */
        destruct(&((*racine)->droite)); /* récursivité avec le noeud suivant à droite */
    free(racine);
}

/* destruction des restes */
void destruct_pile(List *pile) {
    List *temp;
    while (pile->precedent != NULL) {
        temp = pile;
        pile = pile->precedent;
        free(temp);
    }
    free(pile);
}

/* fonction pour ajouter une valeur dans la list chainee */
void push(List **pile, int valeur) {
    (*pile)->valeur = valeur;
    List *nouveau;
    nouveau = malloc(sizeof(List));
    nouveau->precedent = *pile;
    *pile = nouveau;
}

/* fonction retournant une valeur de la liste chainee tout en libérant l'espace mémoire qui était utilisé par celle-ci */
int pop(List **pile) {
    if ((*pile)->precedent == NULL) /* pour éviter de dépiler des choses inexistante */
        return (-1);
    List *temp;
    temp = *pile;
    *pile = (*pile)->precedent;
    free(temp);
    int valeur = (*pile)->valeur;
    return valeur;
}

void tree_to_list(Element **racine, List **pile) {
    push(pile, (*racine)->valeur); /* on stock la valeur dans la liste chainee */
    if ((*racine)->gauche != NULL) /* si il y a un noeud à gauche */
        tree_to_list(&((*racine)->gauche), pile); /* récursivité avec le noeud suivant à gauche */
    if ((*racine)->droite != NULL) /* si il y a un noeud à droite */
        tree_to_list(&((*racine)->droite), pile); /* récursivité avec le noeud suivant à droite */
}

/* fonction pour entrer de nouvelles valeurs dans l'arbre */
void add_node(Element **racine, int nb) {
    if (nb <= (*racine)->valeur) { /* si la valeur est <= à la valeur du noeud actuel */
        if ((*racine)->gauche == NULL) { /* s'il n'existe pas de noeud à gauche */
            Element *nouveau = init(); /* on en créée un nouveau */
            nouveau->valeur = nb; /* on lui attribue sa valeur */
            (*racine)->gauche = nouveau; /* on stock ce nouvel élément à l'adresse gauche */
        }
        else /* s'il existe un noeud */
            add_node(&((*racine)->gauche), nb); /* on passe à celui-ci */
    }
    else { /* même chose pour la droite */
        if ((*racine)->droite == NULL) {
            Element *nouveau = init();
            nouveau->valeur = nb;
            (*racine)->droite = nouveau;
        }
        else
            add_node(&((*racine)->droite), nb);
    }
}

/* fonction pour supprimer un noeud de l'arbre */
void suppr_node(Element **racine, Element **node, List **pile, Element **last_node, int value) {
    if ((*node)->valeur == value) {
        printf("Valeur trouvee. En cours de suppression...\n");
        tree_to_list(node, pile); /* stock la liste de toute les valeurs à partir du noeud qu'on veut suppr */
        if ((*last_node)->gauche == *node) /* si le noeud à supprimer est sur la branche de gauche du noeud au dessus, on le retire */
            (*last_node)->gauche = NULL;
        else /* sinon il est à droite */
            (*last_node)->droite = NULL;
        destruct(node); /* libère la mémoire occupée par le noeud et tout ce qu'il y a en dessous */
        
        /* rajout des valeurs qui était en dessous dans l'arbre */ 
        int to_add;
        to_add = pop(pile);
        while (to_add != -1) {
            add_node(racine, to_add);
            to_add = pop(pile);
        }
        printf("Valeur correctement supprimee.\n");
    }
    else {
        if ((*node)->gauche != NULL) /* si il y a un noeud à gauche */
            suppr_node(racine, &((*node)->gauche), pile, node, value); /* récursivité avec le noeud suivant à gauche */
        if ((*node)->droite != NULL) /* si il y a un noeud à droite */
            suppr_node(racine, &((*node)->droite), pile, node, value); /* récursivité avec le noeud suivant à droite */
    }
}

/* fonction d'affichage de l'arbre */
int print_tree(Element **racine, int profondeur) {
    /* gestion de la profondeur avec des espaces */
    for(int i=0;i<=profondeur;i++)
        printf(" ");
    /* affiche de la valeur du noeud actuel */
    printf("- %d\n", (*racine)->valeur);
    profondeur++;

    if ((*racine)->gauche != NULL) { /* si il y a un noeud à gauche */
        print_tree(&((*racine)->gauche), profondeur); /* récursivité avec le noeud suivant à gauche */
        profondeur--;
    }
    else { /* sinon */
        for(int i=0;i<=profondeur;i++) /* gestion de la profondeur avec des espaces */
            printf(" ");
        printf("-\n"); /* pour les "fils" vide */
    }

    if ((*racine)->droite != NULL){ /*si il y a un noeud à droite */
        print_tree(&((*racine)->droite), profondeur); /* récursivité avec le noeud suivant à droite */
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

/* fonction pour sélectionner la valeur du noeud à supprimer dans l'arbre */
int wanna_suppr(void) {
    char reponse[1];
    int to_suppr;

    printf("Voulez vous supprimer une valeur ? (y/n) : ");
    scanf("%s", reponse);
    if (strcmp(reponse, "y") == 0) {
        printf("\nSaisissez la valeur a supprimer : ");
        scanf("%d", &to_suppr);
        return (to_suppr);
    }
    else if (strcmp(reponse, "n") == 0)
        printf("Fin du programme.\n");
    else {
        printf("Reponse incorrect.\n");
        wanna_suppr();
    }
    return (-1);
}

int main(void) {
    /* initialisation du noeud racine de l'arbre */
    Element *racine = init();
    /* initialisation de la liste chainee (pour le bonus) */
    List *pile;
    init_list(&pile);

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
        add_node(&racine, tab[i]);

    /* affichage de toutes les valeurs utilisées dans l'arbre */
    printf("Insertion des valeurs : ");
    for(int i=0;i<15;i++)
        printf("%d ", tab[i]);
    printf("\n");

    /* Affichage de l'arbre */
    print_tree(&racine, 0);

    /* demande de suppression d'une valeur */
    int value;
    value = wanna_suppr();
    if (value != -1) {
        printf("Suppression de la valeur en cours ...\n");
        suppr_node(&racine, &racine, &pile, &racine, value);
        printf("Affichage apres suppression :\n");
        print_tree(&racine, 0);
    }

    /* Libération de l'espace mémoire utilisé */
    destruct(&racine);
    destruct_pile(pile);

    return (0);
}
