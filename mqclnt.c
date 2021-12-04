#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define BUFFER_SIZE 1024

int seat[9][9];
char rows[9] = { 'A','B','C','D','E','F','G','H','I' };
int seat_availability[9][9] = { 0, };

void seat_print();

// 메세지 형태 정의
typedef struct {
  long msgtype;
  int row;
  int column;
  char buf[BUFFER_SIZE];
  int data[9][9];
}msgbuf;

int main() {
  int key_id;
  msgbuf mybuf;
  int count = 0;
  int num=0;
 
  // 메세지 생성
  key_id = msgget((key_t) 60011, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }
  
  // 메세지 타입 설정
  mybuf.msgtype = 1;
  while (1) {
	  printf("좌석 조회 : 1\n");
	  printf("좌석 예약 : 2\n");
	  printf("좌석 변경 : 3\n");
	  scanf("%d",&num);
	  if(num==1)
	  {
		  mybuf.msgtype=1;
		  strcpy(mybuf.buf,"1");
	// 메시지 전송
	    if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
	         perror("msgsnd() error");
     		 exit(0);
   	  }
	// 수신 설정
		 key_id = msgget((key_t) 60011, IPC_CREAT|0666);
 		 if (key_id == -1) {
    			perror("msgget() error");
    			exit(0);
  		}
	// 메시지 수신
		if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      			perror("msgrcv() error");
      			exit(0);
		}
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				seat[i][j]=mybuf.data[i][j];
			}
		}
		seat_print();

	  }
	  else if(num==2)
	  {
		char c;
		printf("행을 선택하세요. ex)A\n");
		getchar();
		scanf("%c",&c);
		if(c=='A')
			mybuf.row=0;
		else if(c=='B')
			mybuf.row=1;
		else if(c=='C')
                        mybuf.row=2;
		else if(c=='D')
                        mybuf.row=3;
		else if(c=='E')
                        mybuf.row=4;
		else if(c=='F')
                        mybuf.row=5;
		else if(c=='G')
                        mybuf.row=6;
		else if(c=='H')
                        mybuf.row=7;
		else if(c=='I')
                        mybuf.row=8;

		printf("열을 선택하세요. ex)3\n");
		scanf("%d",&mybuf.column);

	  	mybuf.msgtype=1;
		strcpy(mybuf.buf,"2");
		

		 // 메시지 전송
            if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                 perror("msgsnd() error");
                 exit(0);
	  	}
	  }
	  else if(num==3)
	  {
		char c;
                printf("행을 선택하세요. ex)A\n");
                getchar();
                scanf("%c",&c);
                if(c=='A')
                        mybuf.row=0;
                else if(c=='B')
                        mybuf.row=1;
                else if(c=='C')
                        mybuf.row=2;
                else if(c=='D')
                        mybuf.row=3;
                else if(c=='E')
                        mybuf.row=4;
                else if(c=='F')
                        mybuf.row=5;
                else if(c=='G')
                        mybuf.row=6;
                else if(c=='H')
                        mybuf.row=7;
                else if(c=='I')
                        mybuf.row=8;

                printf("열을 선택하세요. ex)3\n");
                scanf("%d",&mybuf.column);

                mybuf.msgtype=1;
                strcpy(mybuf.buf,"3");
		if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                perror("msgsnd() error");
                exit(0);
                }

	  }
	  else{
		  printf("입력 에러입니다.");
	  }
    // value 값 1 증가시켜서 메세지 전송
    
  }
}

void seat_print() {
        printf(" ");

        for (int i = 0; i < 9; i++) {
                printf("%5d  ", i);
        }
        printf("\n");
        printf("-------------------------------------------------------------------------");
        printf("\n");

        for (int i = 0; i < 9; i++) {
                printf("%c  ", rows[i]);
                for (int j = 0; j < 9; j++) {
                        if (seat[i][j] == 1) {
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

