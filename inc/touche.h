#ifndef __TOUCHE_H__
#define __TOUCHE_H__

#include <inttypes.h>

#define NB_TOUCHES 17
#define NB_TOUCHES_REEL 16
#define NULL_TOUCHE 4199999


#define PRESSED 1
#define RELEASED 0

/****************************************************************************************
 * 			LES TOUCHES QUI PEUVENT ETRE UTILISER SONT TOUTES CI DESSOUS				*
 ****************************************************************************************/

#define QUITTER 0	/* Quand on appuye sur la croix en haut a droite */

#define Z 1
#define S 2
#define Q 3
#define D 4

#define SPACE 5
#define SAUTER 5

#define P 6 /* la touche P */
#define L 7 /* la touche L */

#define SHIFT 8

#define BACKSPACE 9
#define ENTER 10

#define ESCAPE 11	/* la touche escape en haut a gauche */

#define SOURIS_BTN_1 12 /* click gauche */
#define SOURIS_GAUCHE 12

#define SOURIS_BTN_2 13
#define SOURIS_DROIT 13

#define SOURIS_ROUE_HAUT 14

#define SOURIS_ROUE_BAS 15

#define AVOID_OUTWRITE 16 /* DOIT etre le dernier et ne doit pas etre touché */


/************************************************************************************************************************************
 * 														MODULE TOUCHE																*
 * 										Il vaut mieux aller voir le test du module touche pour										*
 * 												comprendre les fonctions si dessous													*
 ************************************************************************************************************************************/

/************************************************* FONCTION A APELLE UNIQUE *********************************************************/

/*	initialisation du module touche
	doit etre apeler une unique fois dans le programme */
extern void touche_init( void);





/******************** FONCTION A APLLER DANS UNE BOUCLE NECESSAIRE AU BON FONCTIONNEMENT DU MODULE TOUCHE ***************************/

/*	recupere l etat du clavier et de la souris
	doit etre apeler dans une boucle avant les fonctions suivantes*/
extern void touche_get( void);

/* doit etre apeller a chaque tour de la boucle pour remettre la roue a 0 */
extern void touche_reset_wheel_state( void);





/************************************* FONCTION INDIQUANT L'ETAT DU CLAVIER ET DE LA SOURIS ******************************************/

/* 	recupere l etat d une touche
	voir les define ci dessus pour les touches disponible */
extern uint8_t touche_appuyer( uint16_t touche);

/* renvoie le TOTAL ( peut etre entre -127 et +128 ) du nombre de tour effectuer par l'utilasateur depuis le dernier apelle de SDL_reset_wheel_state. si negatif la roue est tournée vers le bas si positif la roue est tournée vers le haut */
extern int8_t touche_wheel_state( void);

/*	met les coordonees de la souris dans x et y */
extern void touche_coord_souris( int32_t *x, int32_t *y );

#endif