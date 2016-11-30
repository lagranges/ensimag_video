#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>
#include <semaphore.h>


bool fini;

/* les variables pour la synchro, ici */

int nb_cases_pleines = 0;

pthread_cond_t cond_producteur_texture =   PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consommateur_texture = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_producteur_fenetre =   PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consommateur_fenetre = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_texture = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_fenetre = PTHREAD_MUTEX_INITIALIZER;





void debutConsommerTexture() {
	pthread_mutex_lock(&mutex_texture);
	while (nb_cases_pleines == 0) // rien a consommer
		pthread_cond_wait(&cond_consommateur_texture, &mutex_texture);
	nb_cases_pleines--;
}

void finConsommerTexture() {
	pthread_cond_signal(&cond_producteur_texture); // tu peux ecrire
	pthread_mutex_unlock(&mutex_texture);
}

void debutDeposerTexture() {
	pthread_mutex_lock(&mutex_texture);
	while (nb_cases_pleines == NBTEX)
		pthread_cond_wait(&cond_producteur_texture, &mutex_texture);
	nb_cases_pleines++;
}

void finDeposerTexture() {
	pthread_cond_signal(&cond_consommateur_texture); // tu peux lire
	pthread_mutex_unlock(&mutex_texture);
}



void envoiTailleFenetre(th_ycbcr_buffer buffer) {
	pthread_mutex_lock(&mutex_fenetre);
	windowsx = buffer[0].width; // et pourquoi pas buffer[1], buffer[2] ??
	windowsy = buffer[0].height;
	pthread_cond_signal(&cond_consommateur_taille);
// 	printf("[ENVOI TAILLE FENETRE] signalé!");
	pthread_mutex_unlock(&mutex_fenetre);
}

void attendreTailleFenetre() {
	pthread_mutex_lock(&mutex_fenetre);
	while ((windowsx == 0) && (windowsy == 0))
		pthread_cond_wait(&cond_consommateur_fenetre, &mutex_fenetre);
	pthread_mutex_unlock(&mutex_fenetre);
}

void signalerFenetreEtTexturePrete() {
	pthread_mutex_lock(&mutex_fenetre);
	fini = true;
	pthread_mutex_unlocl(&mutex_fenetre);
}

void attendreFenetreTexture() {
	pthread_mutex_lock(&mutex_fenetre);
	while (!fini)
		pthread_cond_wait(&cond_producteur_fenetre, &mutex_fenetre);
	pthread_mutex_unlock(&mutex_fenetre);
}
























// static texture_prod_cons synchro_texture =
// {
// 	.mutex = PTHREAD_MUTEX_INITIALIZER,
// 	.cond_prod = PTHREAD_COND_INITIALIZER,
// 	.cond_cons = PTHREAD_COND_INITIALIZER,
// 	.nb_cases_pleines = 0,
// 	.NBTEX = NBTEX,
// };
// static taille_fenetre_texture synchro_fenetre = 
// {
// 	.mutex = PTHREAD_MUTEX_INITIALIZER,
// 	.cond_taille = PTHREAD_COND_INITIALIZER,
// 	.cond_texture = PTHREAD_COND_INITIALIZER,
// };
