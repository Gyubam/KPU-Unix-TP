#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define MSGSIZ		63
char *fifo = "fifo";
int seat_availability[9][9] = { 0, };
int seat[9][9];
char rows[9] = { 'A','B','C','D','E','F','G','H','I' };
void seat_print();


int main (int argc, char **argv) {
 int fd, j, nwrite;
 char msgbuf[MSGSIZ+1];
 char* select;


 while(1) {
 	printf("좌석 확인 : 1 \n");
	//scanf("%c",argv);
	
	
	 if ((fd = open(fifo, O_WRONLY | O_NONBLOCK)) < 0)
        printf("fifo open failed\n");
	

	select="lakelknf";
	nwrite = write(fd,select,MSGSIZ+1);
	/*
	// 메시지를 보낸다
	 for ( j = 1; j < argc; j++)
 	{
        	if (strlen(argv[j]) > MSGSIZ)
        	{
               		fprintf (stderr, "message too long %s\n", argv[j]);
               		continue;
        	}

        	strcpy (msgbuf, argv[j]);

        	if ((nwrite = write (fd, msgbuf, MSGSIZ+1)) == -1)
               		printf("message write failed\n");

	}
 	*/

 	  }
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

