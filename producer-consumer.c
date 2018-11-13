/* SJSU CMPE 142-02: Operating Systems
*  Nickolas Schiffer #012279319
*  PP #5: Producer-Consumer
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/signal.h>
#include <string.h>

#define SMUTEX "/mutex"
#define SFULL  "/full"
#define SEMPTY "/empty"




char producer_message[BUFSIZ + 1];
int depth_size = -1;
int producer_flag = -1, consumer_flag = -1, monitor_flag = -1;
int unix_socket_flag = -1, shared_memory_flag = -1;
int consumerance_flag = 0;





key_t key = 1234, key_count = 12345;
sem_t *sem_mutex, *sem_full, *sem_empty;
int consumerid;
int shm_id,shm_count_id;

//producer for unix sockets
void producer_u(){
    //acquire shared semaphore from kernel memory
    

    //acquire pointer to shared memory
   
    while(1){
        //Produce an item in next produced

        
    }
}

//producer for shared memory
void producer_s(){
    //acquire shared semaphore from kernel memory
    sem_t *sem_mutex = sem_open(SMUTEX, 0);
    sem_t *sem_empty = sem_open(SEMPTY, 0);
    sem_t *sem_full  = sem_open(SFULL,  0);

    int a,b,c;
    char (*mem)[BUFSIZ];
    int (*count);
    mem = shmat(shm_id, NULL, 0);
    count = shmat(shm_count_id, NULL, 0);
    
    
    /* For Testing, Remove Later */

        *count = 0;
        (*count)++;
    
        for (int i = 0; i < 10; i++){
            char str[BUFSIZ] = "";
            sprintf(str,"Hello there %d", i);
            memcpy(mem[i], str, BUFSIZ);
        }
        for (int i = 0; i < 10; i++){
            printf("%s\n",mem[i]);
        }
        
        // get semaphore values
        sem_getvalue(sem_mutex, &a);
        sem_getvalue(sem_empty, &b);
        sem_getvalue(sem_full, &c);
        printf("sem_mutex: %d, sem_empty: %d, sem_full: %d\n", a, b, c);
    

}

//consumer for unix sockets
void consumer_u(){
    //acquire shared semaphore from kernel memory
    
}

//consumer for shared memory
void consumer_s(){
    //acquire shared semaphore from kernel memory
    
}

void monitor(){
    //acquire shared semaphore from kernel memory
    
}

void shared_mem_init(){
    // shared_buffer = mmap(0,depth_size*BUFSIZ, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shm_id = shmget(key, depth_size*sizeof(char[BUFSIZ]), IPC_CREAT | 0666);
    shm_count_id = shmget(key_count, sizeof(int), IPC_CREAT | 0666);
}

void unix_socket_init(){

}

void semaphore_init(){
    sem_mutex = sem_open(SMUTEX, O_CREAT, 0666, 1);
    sem_empty = sem_open(SEMPTY, O_CREAT, 0666, 0);
    sem_full  = sem_open(SFULL,  O_CREAT, 0666, depth_size);
    // sem_mutex = sem_init(SMUTEX, O_CREAT, 0666, 1);
    // sem_empty = sem_init(SEMPTY, O_CREAT, 0666, 0);
    // sem_full  = sem_init(SFULL,  O_CREAT, 0666, depth_size);

    //printf("semaphore: %s", sem);
}

void buffer_init(int size){

}

void cleanup(int sig){
    shmctl(shm_id, IPC_RMID, NULL);
    shmctl(shm_count_id, IPC_RMID, NULL);
    sem_destroy(sem_mutex);
    sem_destroy(sem_empty);
    sem_destroy(sem_full);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    int c;
    signal(SIGINT, cleanup);

    /*
    Handle -p -m, -c, u, s, q and -e arguments. 
    */
    while ((c = getopt(argc, argv, "usq:pcem:")) != -1){
        switch(c){
            case 'p':
                if (producer_flag == 0){
                    fprintf(stderr, "Error, another role is already selected.\n");
                    exit(EXIT_FAILURE);
                }
                producer_flag = 1;
                consumer_flag = 0;
                monitor_flag  = 0;
                break;
            case 'm':
                if (optarg == NULL){
                    fprintf(stderr, "Error: Option m requires parameter [string: message].\n");
                    exit(EXIT_FAILURE);
                }
                snprintf(producer_message, BUFSIZ, "%s", optarg);
                printf("producer argument: %s\n", producer_message);
                break;
            case 'c':
                if (consumer_flag == 0){
                    fprintf(stderr, "Error, another role is already selected.\n");
                    exit(EXIT_FAILURE);
                }
                consumer_flag = 1;
                producer_flag = 0;
                monitor_flag  = 0;
                break;
            case 'e':
                if (monitor_flag == 0){
                    fprintf(stderr, "Error, another role is already selected.\n");
                    exit(EXIT_FAILURE);
                }
                monitor_flag  = 1;
                producer_flag = 0;
                consumer_flag = 0;
                break;
            case 'u':
                if (shared_memory_flag == 1){
                    fprintf(stderr, "Error, -s (Shared Memory) already selected.\n");
                    exit(EXIT_FAILURE);
                }
                unix_socket_flag   = 1;
                shared_memory_flag = 0;
                break;
            case 's':
                if (unix_socket_flag == 1){
                    fprintf(stderr, "Error, -u (Unix Socket) already selected.\n");
                    exit(EXIT_FAILURE);
                }
                unix_socket_flag   = 0;
                shared_memory_flag = 1;
                printf("shared mem.\n");
                break;
            case 'q':
                if (optarg == NULL){
                    fprintf(stderr, "Error, option q requires parameter [int: queue_depth].\n");
                    exit(EXIT_FAILURE);
                }
                printf("optarg: %s.\n",optarg);
                if ((depth_size = atoi(optarg)) <= 0){
                    fprintf(stderr, "Error, Queue depth must be an int > 0\n");
                    exit(EXIT_FAILURE);
                }
                break;
                
            case '?':
                if (optopt == 'm' || optopt == 'q')
                    //fprintf(stderr, "Option -%c needs an argument.\n", optopt);
                break;
            default:
                fprintf(stderr, "Error");
                break;
        }
        
    }
    if (unix_socket_flag == -1 || shared_memory_flag == -1){
        if (depth_size == -1){
            fprintf(stderr,"Error, must specify (-u or -s) and -q options first.\n");
            exit(EXIT_FAILURE);
        }
    }

    else if (producer_flag == 1){
        if (shared_memory_flag == 1){
            producer_s();
        }
        else if (unix_socket_flag == 1){
            producer_u();
        }
        else {
            fprintf(stderr, "Error, must specify (-u or -s) and -q options first.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (consumer_flag == 1){
        if (shared_memory_flag == 1){
            consumer_s();
        }
        else if (unix_socket_flag == 1){
            consumer_u();
        }
        else {
            fprintf(stderr, "Error, must specify (-u or -s) and -q options first.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (monitor_flag == 1){
        monitor();
    }
    else if (depth_size == -1){
        fprintf(stderr,"Error,must specify queue depth with -q first.\n");
        exit(EXIT_FAILURE);
    }
    
    else{
        semaphore_init();
        if (shared_memory_flag == 1){
            shared_mem_init();
        }
        else if (unix_socket_flag == 1){
            unix_socket_init();
        }
        
    }
    producer_s();
    
    while(1){
    }
}

