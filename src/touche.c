#include "touche.h"
#include "main.h"

#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

#include <SDL2/SDL.h>

#define LONGUEUR_MAX_DESCRIPTIF 20 /* longueur max en caractere du descriptif de la touche */

typedef struct configTouches_s {

	uint32_t keyCode;
	char *descriptif;
} configTouches_t;

uint8_t *keyboardState;
configTouches_t *configuration;

static void SDL_touche_default ( void);	

void touche_destroy( void) {
	/* fermeture propre du module touches pour SDL */

	int i;

	if ( configuration == NULL) {

		if ( keyboardState != NULL) 
			free( keyboardState);

		return;
	}

	for ( i = 0 ; i < NB_TOUCHES ; i++ )
		if ( (configuration + i )->descriptif != NULL)
			free( (configuration + i )->descriptif);

	if ( keyboardState != NULL) 
		free( keyboardState);

	free( configuration);
}

extern void touche_init( void) {

	atexit( touche_destroy);

	/* permet d'allouer les zone memoire necessaire au bon fonctionnement du module touches */

	int i;

	/* allocation de la memoire necessaire au fonctionnement du module */

	keyboardState = malloc( sizeof( short) * NB_TOUCHES );
	configuration = malloc( sizeof( configTouches_t) * NB_TOUCHES);

	for ( i = 0 ; i < NB_TOUCHES ; i++ ) {

		(configuration + i)->descriptif = malloc( sizeof(char) * LONGUEUR_MAX_DESCRIPTIF);
		*(keyboardState + i ) = RELEASED;
	}

	*(keyboardState + SOURIS_ROUE_BAS ) = 0;
	*(keyboardState + SOURIS_ROUE_HAUT ) = 0;

	SDL_touche_default();

	memory_write8( 0xFF00, memory_special_service_read_joy() & 0xF0);
}

extern void touche_get( void) {
	/*	fonction a apeller a chaque image afin de recuperer l'etat ( appuyer ou relacher ) a chaque image
		x et y ne servent uniquement a recuperer les coordonees de la souris */

	SDL_Event event;

	short i;

	while ( SDL_PollEvent(&event) ) {

		i = 0;

		switch( event.type ) {

			case SDL_QUIT:

				*( keyboardState + QUITTER) = PRESSED;
				break;

			case SDL_KEYDOWN:

				while ( event.key.keysym.sym != ( configuration + i)->keyCode && i < NB_TOUCHES_REEL)
					i++;

				*( keyboardState + i) = PRESSED;
				break;

			case SDL_KEYUP:

				while ( event.key.keysym.sym != ( configuration + i)->keyCode && i < NB_TOUCHES_REEL)
					i++;

				*( keyboardState + i) = RELEASED;
				break;

			case SDL_MOUSEBUTTONDOWN:

				while ( event.button.button != ( configuration + i)->keyCode && i < NB_TOUCHES_REEL)
					i++;

				*( keyboardState + i) = PRESSED;
				break;

			case SDL_MOUSEBUTTONUP:

				while ( event.button.button != ( configuration + i)->keyCode && i < NB_TOUCHES_REEL)
					i++;

				*( keyboardState + i) = RELEASED;
				break;

			case SDL_MOUSEWHEEL:

				if ( event.wheel.y == 1)
					(*( keyboardState + SOURIS_ROUE_HAUT)) ++;
				else
					(*( keyboardState + SOURIS_ROUE_BAS)) ++;
				break;
		}
	}

	if ( (memory_special_service_read_joy() & 0b00100000) == 0) {

		if ( touche_appuyer( P))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00000001);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00000001);

		if ( touche_appuyer( L))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00000010);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00000010);

		if ( touche_appuyer( BACKSPACE))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00001000);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00001000);

		if ( touche_appuyer( ENTER))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00000100);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00000100);
	} else if ( (memory_special_service_read_joy() & 0b00010000) == 0) {

		if ( touche_appuyer( Z))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00000100);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00000100);

		if ( touche_appuyer( S))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00001000);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00001000);

		if ( touche_appuyer( Q))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00000010);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00000010);

		if ( touche_appuyer( D))
			memory_write8( 0xFF00, memory_special_service_read_joy() & ~0b00000001);
		else
			memory_write8( 0xFF00, memory_special_service_read_joy() | 0b00000001);
	}
}

extern uint8_t touche_appuyer( uint16_t touche) {
	/* renvoie PRESSED si la touche ( short touche ) est appuyé, RELEASED sinon */
	
	if ( *( keyboardState + touche ) == PRESSED )
		return PRESSED;
	return RELEASED;
}

extern void touche_reset_wheel_state( void) {
	/* doit etre apeller a chaque tour de la boucle pour remettre la roue a 0 */

	(*( keyboardState + SOURIS_ROUE_HAUT )) = 0;
	(*( keyboardState + SOURIS_ROUE_BAS )) = 0;
}

extern int8_t touche_wheel_state( void) {
	/* renvoie le TOTAL ( peut etre entre -127 et +128 ) du nombre de tour effectuer par l'utilasateur depuis le dernier apelle de SDL_reset_wheel_state */

	return (*( keyboardState + SOURIS_ROUE_HAUT )) - (*( keyboardState + SOURIS_ROUE_BAS ));
}

extern void touche_coord_souris ( int32_t *x, int32_t *y) {
	/* met les coordonees de la souris dans x et y */

	SDL_GetMouseState( x, y);
}

static void SDL_touche_default ( void) {

	strcpy((configuration + QUITTER )->descriptif, "quitter");
	(configuration + QUITTER )->keyCode = NULL_TOUCHE;

	strcpy((configuration + Z )->descriptif, "z");
	(configuration + Z )->keyCode = SDLK_z;

	strcpy((configuration + S )->descriptif, "s");
	(configuration + S )->keyCode = SDLK_s;

	strcpy((configuration + Q )->descriptif, "q");
	(configuration + Q )->keyCode = SDLK_q;

	strcpy((configuration + D )->descriptif, "d");
	(configuration + D )->keyCode = SDLK_d;

	strcpy((configuration + SPACE )->descriptif, "sauter");
	(configuration + SPACE )->keyCode = SDLK_SPACE;

	strcpy((configuration + P )->descriptif, "p");
	(configuration + P )->keyCode = SDLK_p;

	strcpy((configuration + L )->descriptif, "l");
	(configuration + L )->keyCode = SDLK_l;

	strcpy((configuration + SHIFT )->descriptif, "shift");
	(configuration + SHIFT )->keyCode = SDLK_LSHIFT;

	strcpy((configuration + BACKSPACE )->descriptif, "backspace");
	(configuration + BACKSPACE )->keyCode = SDLK_BACKSPACE;

	strcpy((configuration + ENTER )->descriptif, "enter");
	(configuration + ENTER )->keyCode = SDLK_RETURN;

	strcpy((configuration + ESCAPE )->descriptif, "escape");
	(configuration + ESCAPE )->keyCode = SDLK_ESCAPE;

	strcpy((configuration + SOURIS_BTN_1 )->descriptif, "souris_gauche");
	(configuration + SOURIS_BTN_1 )->keyCode = SDL_BUTTON_LEFT;

	strcpy((configuration + SOURIS_BTN_2 )->descriptif, "souris_droit");
	(configuration + SOURIS_BTN_2 )->keyCode = SDL_BUTTON_RIGHT;

	strcpy((configuration + SOURIS_ROUE_HAUT )->descriptif, "souris_roue_haut");
	(configuration + SOURIS_ROUE_HAUT )->keyCode = NULL_TOUCHE;

	strcpy((configuration + SOURIS_ROUE_BAS )->descriptif, "souris_roue_bas");
	(configuration + SOURIS_ROUE_BAS )->keyCode = NULL_TOUCHE;

	strcpy((configuration + AVOID_OUTWRITE )->descriptif, "NULL");
	(configuration + AVOID_OUTWRITE )->keyCode = NULL_TOUCHE;
}