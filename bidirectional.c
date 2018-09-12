#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#define MSGSIZE 18

int main() {
    
    //Defining a pipe
    char inbuf[MSGSIZE];
    char outbuf[MSGSIZE];
    int p[2], i;
    if (pipe(p) < 0) {
        printf("Internal Pipe Error\n");
        exit(1);
    }

    //Defining a semaphore
    key_t mut = 1234;
    int msgflag = IPC_CREAT | 0666;
    int mutex = semget(mut, 2, msgflag);
    struct sembuf WAIT[1], SIGNAL[1];
    WAIT[0].sem_num = 1;
    WAIT[0].sem_op = -1;
    WAIT[0].sem_flg = SEM_UNDO;
    SIGNAL[0].sem_num = 1;
    SIGNAL[0].sem_op = 1;
    SIGNAL[0].sem_flg = SEM_UNDO;

    //Assigning two users
    int choice, secC, msgCount=0;
    do {
        printf("\nChoose a user [1/2, 0-stop] : ");
        scanf("%d",&choice);
        if (choice==1) {
            semop(mutex, WAIT, 1); //wait(mutex)
            printf("Select 1-read, 2-write : ");
            scanf("%d",&secC);
            if (secC==1) {
		        if(msgCount==0) {
			        printf("No messages\n");
		        } else {
                    printf("The messages sent from USER 2 :\n");
                    for(i=0;i<msgCount;i++) {
                        read(p[0], inbuf, MSGSIZE);
                        printf("%s\n", inbuf);
			        }
		        }
		        msgCount=0;
            } else {
                printf("Enter Message to be Sent : ");
                scanf("%s", outbuf);
                write(p[1], outbuf, MSGSIZE);
                printf("Message Sent\n");
		        msgCount++;
            }
            semop(mutex, SIGNAL, 1); //signal(mutex)
        } else if (choice==2) {
            semop(mutex, WAIT, 1); //wait(mutex)
            printf("Select 1-read, 2-write : ");
            scanf("%d",&secC);
            if (secC==1) {
		        if(msgCount==0) {
			        printf("No messages\n");
		        } else {
                    printf("The messages sent from USER 2 :\n");
                    for(i=0;i<msgCount;i++) {
                        read(p[0], inbuf, MSGSIZE);
                        printf("%s\n", inbuf);
			        }
		        }
		        msgCount=0;
            } else {
                printf("Enter Message to be Sent : ");
                scanf("%s", outbuf);
                write(p[1], outbuf, MSGSIZE);
                printf("Message Sent\n");
		        msgCount++;
            }
            semop(mutex, SIGNAL, 1); //signal(mutex)
        } else {
            printf("<< STOPPING >>\n\n");
            break;
        }
    } while (choice == 1 || choice == 2);
    semctl(mutex, 1, IPC_RMID | 0666);
    return 0;
}
