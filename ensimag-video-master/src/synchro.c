#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>
#include <semaphore.h>


bool fini;

/* les variables pour la synchro, ici */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_taille_fenetre = PTHREAD_COND_INITIALIZER;

static texture_prod_cons synchro_texture =
{
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.cond_prod = PTHREAD_COND_INITIALIZER,
	.cond_cons = PTHREAD_COND_INITIALIZER,
	.nb_cases_pleines = 0,
	.NBMAX = NBTEX,
};

/* l'implantation des fonctions de synchro ici */
void debutConsommerTexture() {
	pthread_mutex_lock(&synchro_texture.mutex);
	while (synchro_texture.nb_cases_pleines == 0) // rien a consommer
		pthread_cond_wait(&synchro_texture.cond_cons, &synchro_texture.mutex);
	synchro_texture.nb_cases_pleines--;
}

void finConsommerTexture() {
	pthread_cond_signal(&synchro_texture.cond_prod); // tu peux ecrire
	pthread_mutex_unlock(&synchro_texture.mutex);
}

void debutDeposerTexture() {
	pthread_mutex_lock(&synchro_texture.mutex);
	while (synchro_texture.nb_cases_pleines == synchro_texture.NBMAX)
		pthread_cond_wait(&synchro_texture.cond_prod, &synchro_texture.mutex);
	synchro_texture.nb_cases_pleines++;
}

void finDeposerTexture() {
	pthread_cond_signal(&synchro_texture.cond_cons); // tu peux lire
	pthread_mutex_unlock(&synchro_texture.mutex);
}

void envoiTailleFenetre(th_ycbcr_buffer buffer) {
}

void attendreTailleFenetre() {
// 	cond_taille_fenetre.wait();
}

void signalerFenetreEtTexturePrete() {
}

void attendreFenetreTexture() {
// 	cond_fenetre_texture.wait();
}

