#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdbool.h> // why ?!
#include "ensitheora.h"
#include <pthread.h>
#include <semaphore.h>

extern bool fini;

/* structure de synchronisation de la texture (producteur-consommateur) */
typedef struct 
{
	pthread_mutex_t mutex; // mutex d'acces au tampon
	pthread_cond_t cond_prod;
	pthread_cond_t cond_cons;
	int nb_cases_pleines;
	const int NBMAX; // nb de textures (=NBTEX) 
} texture_prod_cons;

typedef struct
{
	pthread_mutex_t mutex;
	pthread_cond_t cond_taille;
	pthread_cond_t cond_texture;
} taille_fenetre_texture;



/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
