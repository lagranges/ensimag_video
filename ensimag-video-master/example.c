#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#define NBTHR 10

void *hello(void *a) {
      printf ("hello world! de %ld\n", (intptr_t)a);
        fflush(stdout);
          sleep(5);
            return 0;
}

int main(int argc, char **argv) {
      (void)argc;
        (void) argv;
          pthread_t tid[NBTHR];
            
            for (int i=0; i < NBTHR; i++) {
                    pthread_create(& tid[i], NULL, hello, (void *)(intptr_t)i);
                      }
              
              for (int i=0; i < NBTHR; i++) {
                      void *status;
                          pthread_join(tid[i], & status);
                            }
                
                return 0;
}
