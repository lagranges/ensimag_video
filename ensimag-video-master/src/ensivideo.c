#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#include "stream_common.h"
#include "oggstream.h"

pthread_t thread_theora_decode,
	  thread_vorbis;

extern pthread_t thread_theora_affiche;

int main(int argc, char *argv[]) {
    int res;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);
    
    // start the two stream readers
    pthread_create(&thread_theora_decode, NULL, theoraStreamReader, argv[1]);
    pthread_create(&thread_vorbis, NULL, vorbisStreamReader, argv[1]);
    
    // wait audio thread
    pthread_join(thread_vorbis, NULL);

    // 1 seconde de garde pour le son,
    sleep(1);
   
    // tuer les deux threads videos si ils sont bloqués
    pthread_cancel(thread_theoa_affiche);
    pthread_cancel(thread_theora_decode);

    // attendre les 2 threads videos
    pthread_join(thread_theora_decode, NULL);
    pthread_join(thread_theora_affiche, NULL);
    
    exit(EXIT_SUCCESS);    
}
	
