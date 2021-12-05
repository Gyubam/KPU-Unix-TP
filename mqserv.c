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

pthread_t th[4];
pthread_mutex_t th_m[3];

int seat[9][9]={ 0, };
char rows[9] = { 'A','B','C','D','E','F','G','H','I' };
int remain_seat=81;

void *clnt1();
void *clnt2();
void *clnt3();
void *send_update();


// 메세지 형태 정의
typedef struct {
  long msgtype;
  int row;
  int column;
  char buf[BUFFER_SIZE];
  int data[9][9];
  int remain_seat;
} msgbuf;
msgbuf mybuf;
int main() {
	int key_id;
	int key_id2;
	int key_id3;


	printf("서버 실행중..\n");
	
	for(int i=0;i<3;i++){
		pthread_mutex_init(&th_m[i],NULL);
	}

 	pthread_create(&th[0],NULL,(void *)clnt1,NULL);
	pthread_create(&th[1],NULL,(void *)clnt2,NULL);
	pthread_create(&th[2],NULL,(void *)clnt3,NULL);
	pthread_create(&th[3],NULL,(void *)send_update,NULL);	
       while(1){



        printf("서버 실행중..\n");
	sleep(5);   


       }	       
    }
void *clnt1()
{
  int s_row;
  int s_column;
  int key_id;
  long msgtype = 1; // 수신받을 메세지 타입 미리 설정

  // 메세지 수신 
  key_id = msgget((key_t) 60011, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }

//////////////////////////////////////////////////////////////
  while (1) {
// while /////////////////////////////////////////////////
    // 메세지 타입이 1 일 때 수신
    if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      perror("msgrcv() error");
      exit(0); 
    }else {

	if(strcmp(mybuf.buf,"1")==0){
		printf("좌석 조회 요청\n");
	
        // 메세지에 배열 데이터 입력
		pthread_mutex_lock(&th_m[0]);
		for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                        mybuf.data[i][j]=seat[i][j];
                }
		mybuf.remain_seat=remain_seat;
        	}
		pthread_mutex_unlock(&th_m[0]);

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

		pthread_mutex_lock(&th_m[0]);
		seat[s_row][s_column]=1;
		remain_seat--;
		pthread_mutex_unlock(&th_m[0]);
	}

	if(strcmp(mybuf.buf,"3")==0){
		printf("좌석 변경 요청\n");
		s_row=mybuf.row;
                s_column=mybuf.column;
		
		pthread_mutex_lock(&th_m[0]);
		if(seat[s_row][s_column]==1){
                	seat[s_row][s_column]=0;
			remain_seat++;
		}else{
			seat[s_row][s_column]=1;
			remain_seat--;
		}
		pthread_mutex_unlock(&th_m[0]);
	}

	if(strcmp(mybuf.buf,"4")==0){
                printf("잔여 좌석 조회 요청\n");
                
		pthread_mutex_lock(&th_m[0]);
		mybuf.remain_seat=remain_seat;
		pthread_mutex_unlock(&th_m[0]);

		if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                        perror("msgsnd() error");
                        exit(0);
                }

        
	}
    
    }
// while///////////////////////////////////////////////////
  }
//////////////////////////////////////////////////////////////
}


void *clnt2()
{
  int s_row;
  int s_column;
  int key_id;
  long msgtype = 1; // 수신받을 메세지 타입 미리 설정

  // 메세지 수신
  key_id = msgget((key_t) 60012, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }

//////////////////////////////////////////////////////////////
  while (1) {
// while /////////////////////////////////////////////////
    // 메세지 타입이 1 일 때 수신
    if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      perror("msgrcv() error");
      exit(0);
    }else {

        if(strcmp(mybuf.buf,"1")==0){
                printf("좌석 조회 요청\n");

        // 메세지에 배열 데이터 입력
	
		pthread_mutex_lock(&th_m[1]);
                for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                        mybuf.data[i][j]=seat[i][j];
                }
                mybuf.remain_seat=remain_seat;
                }
		pthread_mutex_unlock(&th_m[1]);

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

		pthread_mutex_lock(&th_m[1]);
                seat[s_row][s_column]=1;
                remain_seat--;
		pthread_mutex_unlock(&th_m[1]);
        }

        if(strcmp(mybuf.buf,"3")==0){
                printf("좌석 변경 요청\n");
                s_row=mybuf.row;
                s_column=mybuf.column;

		pthread_mutex_lock(&th_m[1]);
                if(seat[s_row][s_column]==1){
                        seat[s_row][s_column]=0;
                        remain_seat++;
                }else{
                        seat[s_row][s_column]=1;
                        remain_seat--;
                }
		pthread_mutex_unlock(&th_m[1]);
        }

        if(strcmp(mybuf.buf,"4")==0){
                printf("잔여 좌석 조회 요청\n");

		pthread_mutex_lock(&th_m[1]);
                mybuf.remain_seat=remain_seat;
		pthread_mutex_lock(&th_m[1]);

                if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                        perror("msgsnd() error");
                        exit(0);
                }


        }

    }
// while///////////////////////////////////////////////////
  }
//////////////////////////////////////////////////////////////
}

void *clnt3()
{
  int s_row;
  int s_column;
  int key_id;
  long msgtype = 1; // 수신받을 메세지 타입 미리 설정

  // 메세지 수신
  key_id = msgget((key_t) 60013, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }

//////////////////////////////////////////////////////////////
  while (1) {
// while /////////////////////////////////////////////////
    // 메세지 타입이 1 일 때 수신
    if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      perror("msgrcv() error");
      exit(0);
    }else {

        if(strcmp(mybuf.buf,"1")==0){
                printf("좌석 조회 요청\n");

        // 메세지에 배열 데이터 입력
	
		pthread_mutex_lock(&th_m[2]);
                for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                        mybuf.data[i][j]=seat[i][j];
                }
                mybuf.remain_seat=remain_seat;
                }
		pthread_mutex_unlock(&th_m[2]);

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

		pthread_mutex_lock(&th_m[2]);
                seat[s_row][s_column]=1;
                remain_seat--;
		pthread_mutex_unlock(&th_m[2]);
        }

        if(strcmp(mybuf.buf,"3")==0){
                printf("좌석 변경 요청\n");
                s_row=mybuf.row;
                s_column=mybuf.column;

		pthread_mutex_lock(&th_m[2]);
                if(seat[s_row][s_column]==1){
                        seat[s_row][s_column]=0;
                        remain_seat++;
                }else{
                        seat[s_row][s_column]=1;
                        remain_seat--;
                }
		pthread_mutex_unlock(&th_m[2]);
        }

        if(strcmp(mybuf.buf,"4")==0){
                printf("잔여 좌석 조회 요청\n");


		pthread_mutex_lock(&th_m[2]);
                mybuf.remain_seat=remain_seat;
		pthread_mutex_unlock(&th_m[2]);

                if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                        perror("msgsnd() error");
                        exit(0);
                }


        }

    }
// while///////////////////////////////////////////////////
  }
//////////////////////////////////////////////////////////////
}

void *send_update(){
  int key_id;
  long msgtype = 1; // 수신받을 메세지 타입 미리 설정

  // 메세지 수신
  key_id = msgget((key_t) 60014, IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }
  mybuf.msgtype=1;
	
  while(1){
  	if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                        perror("msgsnd() error");
                        exit(0);
                }
	sleep(3);
  }

}
