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

#define SNAME "/mysem"


char producer_message[BUFSIZ + 1];
int depth_size = -1;
int producer_flag = -1, consumer_flag = -1, monitor_flag = -1;
int unix_socket_flag = -1, shared_memory_flag = -1;
int consumerance_flag = 0;

key_t key = 1234;
int consumerid;
int shm_id;

//producer for unix sockets
void producer_u(){
    //acquire shared semaphore from kernel memory
    sem_t *sem = sem_open(SNAME, 0);

    //acquire pointer to shared memory
   
    while(1){
        //Produce an item in next produced

        
    }
}

//producer for shared memory
void producer_s(){
    //acquire shared semaphore from kernel memory
    sem_t *sem = sem_open(SNAME, 0);

}

//consumer for unix sockets
void consumer_u(){
    //acquire shared semaphore from kernel memory
    sem_t *sem = sem_open(SNAME, 0);
}

//consumer for shared memory
void consumer_s(){
    //acquire shared semaphore from kernel memory
    sem_t *sem = sem_open(SNAME, 0);
}

void monitor(){
    //acquire shared semaphore from kernel memory
    sem_t *sem = sem_open(SNAME, 0);
}

void shared_mem_init(){
    // shared_buffer = mmap(0,depth_size*BUFSIZ, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shm_id = shmget(key, depth_size*BUFSIZ, IPC_CREAT | 0666);
}

void unix_socket_init(){

}

void semaphore_init(){
    sem_t *sem = sem_open(SNAME, O_CREAT, 0644, depth_size - 1);
}

int main(int argc, char *argv[]){
    int c;

    /*
    Handle -p -m, -c, and -e arguments. These handle producer (with string: message), consumer, and monitor. 
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
                break;
            case 'q':
                if (optarg == NULL){
                    fprintf(stderr, "Error, option q requires parameter [int: queue_depth].\n");
                    exit(EXIT_FAILURE);
                }
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
        }
        
    }
    if (unix_socket_flag == -1 || shared_memory_flag == -1){
        if (depth_size == -1){
            fprintf(stderr,"Error, must specify (-u or -s) and -q options first.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (depth_size == -1){
        fprintf(stderr,"Error,must specify queue depth with -q first.\n");
        exit(EXIT_FAILURE);
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
    else{
        semaphore_init();
        if (shared_memory_flag == 1){
            shared_mem_init();
        }
        if (unix_socket_flag == 1){
            unix_socket_init();
        }
        
    }
    while(1){

    }
}

