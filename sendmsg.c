#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MSGSIZ		63

char *fifo = "fifo";

int main (int argc, char **argv) {
 int fd, j, nwrite;
 char msgbuf[MSGSIZ+1];

 if (argc < 2)
 {
 	fprintf (stderr, "Usage: sendmessage msg ... \n");
 	exit(1);
 }
/* O_NONBLOCK을 설정하여 fifo를 개방한다. */
 if ((fd = open(fifo, O_WRONLY | O_NONBLOCK)) < 0)
 	printf("fifo open failed\n");

/* 메시지를 보낸다. */
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
 exit (0);
}

