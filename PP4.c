/* Nickolas Schiffer ID# 012279319, GID#37
    PP4 Threads */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>


pthread_cond_t cv;
pthread_mutex_t mutex;// = PTHREAD_MUTEX_INITIALIZER;
int ret1,ret2;

void *thread1(){
    //Print Ping
    pthread_mutex_lock(&mutex);
    printf("thread 1: ping thread 2\n");
    
    
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cv);
    

    
    //pthread_mutex_lock(&mutex);
    while(pthread_cond_wait(&cv, &mutex));
    printf("thread 1: pong! thread 2 ping received\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(EXIT_SUCCESS);
    
}

void *thread2(){
    //pthread_mutex_lock(&mutex);
    while(pthread_cond_wait(&cv, &mutex));
    pthread_mutex_unlock(&mutex);

    printf("thread 2: pong! thread 1 ping received\n");
    printf("thread 2: ping thread 1\n");
    
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mutex); 
    

    pthread_exit(EXIT_SUCCESS);



}

int main() {
    ret1 = pthread_cond_init(&cv, NULL);
    pthread_t t1,t2;
    while(1){
        pthread_create(&t1, NULL, thread1, (void*)&t1);
        pthread_create(&t2, NULL, thread2, (void*)&t2);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
    return EXIT_SUCCESS;
}

