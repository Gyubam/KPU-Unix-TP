#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define MSGSIZ		63

pthread_t th[5];
pthread_mutex_t th_m[5];

char *fifo = "fifo";
char *fifo2 = "fifo2";
int seat_availability[9][9] = { 0, };
int seat[9][9];
char rows[9] = { 'A','B','C','D','E','F','G','H','I' };
void seat_print();



int main (int argc, char **argv) {
 int fd, j, nwrite;
 int fd2;
 char msgbuf[MSGSIZ+1];
 char msgbuf2[MSGSIZ+1];
 char* select;
// if (argc < 2)
// {
// 	fprintf (stderr, "Usage: sendmessage msg ... \n");
// 	exit(1);
// }
/* O_NONBLOCK을 설정하여 fifo를 개방한다. */





  if (mkfifo(fifo, 60011) == -1)
 {
        if (errno != EEXIST)
                printf("receiver: mkfifo\n");
 }

  if ((fd2 = open(fifo2, O_RDWR | O_NONBLOCK)) < 0)
        printf("fifo2 open failed\n");


//  if (mknod("fifo2",S_IFIFO | 0644,0) == -1){
//  	perror("mknode");
//	
//  }

 /* fifo를 읽기와 쓰기용으로 개방한다. */
 if ((fd = open(fifo, O_RDWR | O_NONBLOCK)) < 0)
        printf("fifo open failed\n");


/* 메시지를 받는다 */
 for(;;)
 {
        if (read(fd, msgbuf, MSGSIZ+1) <0)
                printf("message read failed\n");
	else{
		select="send from serv";
        	nwrite = write(fd2,select,MSGSIZ+1);

	}


 /*
  * 메시지를 프린트한다 ; 실제로는 보다 흥미 있는 일이 수행된다.
  */

        printf ("message received:%s\n", msgbuf);
 }

 seat_print();
 exit (0);
}
void seat_print() {
	printf(" ");

	for (int i = 0; i < 9; i++) {
		printf("%5d  ", i + 1);
	}
	printf("\n");
	printf("-------------------------------------------------------------------------");
	printf("\n");

	for (int i = 0; i < 9; i++) {
		printf("%c  ", rows[i]);
		for (int j = 0; j < 9; j++) {
			if (seat_availability[i][j] == 1) {
				printf("| ■ |  ");
			}
			else {
				printf("| □ |  ");
			}


		}
		printf("\n");
		printf("-------------------------------------------------------------------------");
		printf("\n");
	}
}
void read_from_clnt(){}
void read_from_clnt2() {

     
}

