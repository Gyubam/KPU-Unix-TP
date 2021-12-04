#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024

pthread_t th[5];
pthread_mutex_t th_m[5];

int seat[9][9];
char rows[9] = { 'A','B','C','D','E','F','G','H','I' };

void *clnt1();
void *clnt2();
void *clnt3();


// 메세지 형태 정의
typedef struct {
  long msgtype;
  int row;
  int column;
  char buf[BUFFER_SIZE];
  int data[9][9];
} msgbuf;
int main() {
 	pthread_create(&th[0],NULL,(void *)clnt1,NULL);
	pthread_create(&th[1],NULL,(void *)clnt2,NULL);
	pthread_create(&th[2],NULL,(void *)clnt3,NULL);

       while(1){
	   printf("서버 실행중..\n");
	   sleep(3);
       }	       
    }
void *clnt1()
{
  int s_row;
  int s_column;
  int key_id;
  msgbuf mybuf;
  long msgtype = 1; // 수신받을 메세지 타입 미리 설정

  // 메세지 수신 
  key_id = msgget((key_t) 60011, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }

//////////////////////////////////////////////////////////////
  while (1) {
    // 메세지 타입이 1 일 때 수신
    if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      perror("msgrcv() error");
      exit(0); 
    }else {

	if(strcmp(mybuf.buf,"1")==0){
		printf("좌석 조회 요청\n");
	
        // 메세지에 배열 데이터 입력
		for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                        mybuf.data[i][j]=seat[i][j];
                }
        }

                // 메시지 전송
        	if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                 	perror("msgsnd() error");
                 	exit(0);
          	}
	}
	
	if(strcmp(mybuf.buf,"2")==0){
		printf("좌석 예약 요청\n");

		s_row=mybuf.row;
		s_column=mybuf.column;

		seat[s_row][s_column]=1;
	}

	if(strcmp(mybuf.buf,"3")==0){
		printf("좌석 변경 요청\n");
		s_row=mybuf.row;
                s_column=mybuf.column;
		
		if(seat[s_row][s_column]==1){
                	seat[s_row][s_column]=0;
		}else{
			seat[s_row][s_column]=1;
		}
	}
    }
  }
//////////////////////////////////////////////////////////////
}
void *clnt2()
{
  int key_id;
  msgbuf mybuf;
  long msgtype = 1; // 수신받을 메세지 타입 미리 설정

  // 메세지 생성
  key_id = msgget((key_t) 60012, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }

  while (1) {
    // 메세지 타입이 1 일 때 수신
    if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      perror("msgrcv() error");
      exit(0);
    // 수신 받은 메세지에서 buf에 담긴 메시지  출력

    }
    printf("value : %s\n", mybuf.buf);

  }

}
void *clnt3()
{
  int key_id;
  msgbuf mybuf;
  long msgtype = 1; // 수신받을 메세지 타입 미리 설정

  // 메세지 생성
  key_id = msgget((key_t) 60013, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }

  while (1) {
    // 메세지 타입이 1 일 때 수신
    if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      perror("msgrcv() error");
      exit(0);
    // 수신 받은 메세지에서 buf에 담긴 메시지  출력

    }
    printf("value : %s\n", mybuf.buf);

  }

}

