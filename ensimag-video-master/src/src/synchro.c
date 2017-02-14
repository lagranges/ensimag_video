#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>
#include <semaphore.h>


bool fini;

/* les variables pour la synchro, ici */

// structure non statique (extern) car
// utilisee dans (ensitheora.c?)
texture_prod_cons synchro_texture =
{
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.cond_prod = PTHREAD_COND_INITIALIZER,
	.cond_cons = PTHREAD_COND_INITIALIZER,
	.nb_cases_pleines = 0,
};

static taille_fenetre_texture synchro_fenetre = 
{
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.cond_taille = PTHREAD_COND_INITIALIZER,
	.cond_texture = PTHREAD_COND_INITIALIZER,
};

static  afficher_prete synchro_affichage = 
{
	.mutex = PTHREAD_MUTEX_INITIALIZER,
};

pthread_mutex_t mutex_consommer;

/* l'implantation des fonctions de synchro ici */
void debutConsommerTexture() {
	pthread_mutex_lock(&mutex_consommer);
	while (synchro_texture.nb_cases_pleines == 0) // rien a consommer
		pthread_cond_wait(&synchro_texture.cond_cons, &mutex_consommer);
	synchro_texture.nb_cases_pleines--;
}

void finConsommerTexture() {
	pthread_cond_signal(&synchro_texture.cond_prod); // tu peux ecrire
	pthread_mutex_unlock(&mutex_consommer);
}

void debutDeposerTexture() {
	pthread_mutex_lock(&synchro_texture.mutex);
	while (synchro_texture.nb_cases_pleines == NBTEX)
		pthread_cond_wait(&synchro_texture.cond_prod, &synchro_texture.mutex);
	synchro_texture.nb_cases_pleines++;
}

void finDeposerTexture() {
	pthread_cond_signal(&synchro_texture.cond_cons); // tu peux lire
	pthread_mutex_unlock(&synchro_texture.mutex);
}


void envoiTailleFenetre(th_ycbcr_buffer buffer) {
	pthread_mutex_lock(&synchro_fenetre.mutex);
	windowsx = buffer[0].width; // et pourquoi pas buffer[1], buffer[2] ??
	windowsy = buffer[0].height; // mystere ...
	pthread_cond_signal(&synchro_fenetre.cond_taille);
	pthread_mutex_unlock(&synchro_fenetre.mutex);
	pthread_mutex_lock(&synchro_affichage.mutex);
}

void attendreTailleFenetre() {
	pthread_mutex_lock(&synchro_fenetre.mutex);
	while ((windowsx == 0) && (windowsy == 0))
		pthread_cond_wait(&synchro_fenetre.cond_taille,
				&synchro_fenetre.mutex);
	pthread_mutex_unlock(&synchro_fenetre.mutex);
}

void signalerFenetreEtTexturePrete() {
	pthread_mutex_unlock(&synchro_affichage.mutex);
}

void attendreFenetreTexture() {
	pthread_mutex_lock(&synchro_affichage.mutex);
	pthread_mutex_unlock(&synchro_affichage.mutex);
    pthread_mutex_destroy(&synchro_affichage.mutex); 
}

