/* Nickolas Schiffer ID #012279319, GID #37
   CMPE 142 Programming Project #3 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


int main() {
    char parent_string[BUFSIZ];
    char child_string[BUFSIZ];
    char pReadbuf[BUFSIZ];
    char cReadbuf[BUFSIZ];

    int fd[2];

    pid_t PID;

    pipe(fd);

    PID = fork();

    if (PID < 0){
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (PID == 0){ //Child
        read(fd[0], cReadbuf, sizeof(cReadbuf));                    //blocks on reading from pipe.
        
        printf(cReadbuf);                                           // print message from parent.

        sprintf(child_string, "Daddy, my name is %d\n", (int)PID); 
        
        write(fd[1], child_string, strlen(child_string));           //send message to parent through pipe.
        exit(EXIT_SUCCESS);                                         //exit status for parent.

    }
    else { //Parent
        sprintf(parent_string, "I am your daddy! and my name is %d\n", (int)PID);
        write(fd[1], parent_string, strlen(parent_string));         //write message to child through pipe.
        
        wait(NULL);                                                 //wait for child to exit.
        read(fd[0], pReadbuf, sizeof(pReadbuf));                    //read message from child.
        printf(pReadbuf);                                           //print message from child.

        close(fd[0]);                                               //close pipes.
        close(fd[1]);

        exit(EXIT_SUCCESS);

        

    }
}
