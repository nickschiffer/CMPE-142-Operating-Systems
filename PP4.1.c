/* Nickolas Schiffer ID# 012279319, GID #37
    PP4 Threads */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


pthread_cond_t cv;
pthread_mutex_t cl, tl;
int ret;

void *thread(void *param){
    int ttype = *((int *)param);

    switch(ttype){

        case 1: {
            pthread_mutex_lock(&cl);    //Lock cond lock, which thread 2 will block on
            pthread_mutex_unlock(&tl);  //Unlock thread lock
            printf("thread 1: ping thread 2\n");
            fflush(stdout);
            pthread_cond_wait(&cv, &cl);//Sleep until thread 2 is done with lock

            printf("thread 1: pong! thread 2 ping received\n");
            fflush(stdout);
            pthread_mutex_unlock(&cl);
            pthread_exit((void*)EXIT_SUCCESS);
        }
        case 2: {
            pthread_mutex_lock(&tl);    //Block on thread lock until thread 1 is done with it.
            pthread_mutex_lock(&cl);    //Aquire cond lock that thread 1 is waiting on.
            pthread_mutex_unlock(&tl);  //Release thread lock.
            printf("thread 2: pong! thread 1 ping received\n");
            fflush(stdout);
            printf("thread 2: ping thread 1\n");
            fflush(stdout);
            pthread_mutex_unlock(&cl);  //Release cond lock.
            pthread_cond_signal(&cv);   //Wake up thread 1.
            pthread_exit((void*)EXIT_SUCCESS);
        }

        default:
            pthread_exit((void*)EXIT_FAILURE);

    }
}


int main() {
    pthread_t t1, t2;
    ret = pthread_cond_init(&cv, NULL);
    int p1 = 1;
    int p2 = 2;
    
    while(1){

        pthread_mutex_lock(&tl); //Need to ensure that thread 1 executes before thread 2.
        pthread_create(&t1, NULL, thread, (void*)&p1);
        pthread_create(&t2, NULL, thread, (void*)&p2);

        pthread_join(t1, EXIT_SUCCESS);
        pthread_join(t2, EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
