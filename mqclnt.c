#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#define BILLION 1000000000L;
#define BUFFER_SIZE 1024

pthread_t th[3];
pthread_mutex_t th_m[3];

int seat[9][9]={ 0, };
char rows[9] = { 'A','B','C','D','E','F','G','H','I' };
int remain_seat=81;

void seat_print();
void print_time();
void check_update();
void remain_print();
// 메세지 형태 정의
typedef struct {
  long msgtype;
  int row;
  int column;
  char buf[BUFFER_SIZE];
  int data[9][9];
  int remain_seat;
}msgbuf;
msgbuf mybuf;

int main(int argc, char **argv) {
  int key_id;
  int count = 0;
  int num=0;

  int i;
  struct timespec start, stop;
  double accum;


  if(argc != 2){
	printf("key 번호를 입력해주세요\n");
	exit(1);
  }

 
  // 메세지 생성
  key_id = msgget((key_t) atoi(argv[1]), IPC_CREAT|0666);
  if (key_id == -1) {
    perror("msgget() error");
    exit(0);
  }
  
	pthread_create(&th[0],NULL,(void *)check_update,NULL);
	pthread_create(&th[0],NULL,(void *)print_time,NULL);

  // 메세지 타입 설정
  mybuf.msgtype = 1;
  while (1) {
	  printf("좌석 조회 : 1\n");
	  printf("좌석 예약 : 2\n");
	  printf("좌석 변경 : 3\n");
	  printf("잔여 좌석 조회: 4\n");
	  printf("종료 : 0\n");
	  scanf("%d",&num);
	  if(num==1)
	  {
		  mybuf.msgtype=1;
		  strcpy(mybuf.buf,"1");

	// 타이머 시작
		  if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
			perror( "clock gettime" );
			return EXIT_FAILURE;
			}

	// 메시지 전송
	    if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
	         perror("msgsnd() error");
     		 exit(0);
   	  }
	// 수신 설정
		 key_id = msgget((key_t) atoi(argv[1]), IPC_CREAT|0666);
 		 if (key_id == -1) {
    			perror("msgget() error");
    			exit(0);
  		}
	// 메시지 수신
		if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
      			perror("msgrcv() error");
      			exit(0);
		}

		if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
			perror( "clock gettime" );
			return EXIT_FAILURE;
			}
		accum = ( stop.tv_sec - start.tv_sec )
		+ (double)( stop.tv_nsec - start.tv_nsec )
		/ (double)BILLION;
		printf("수행시간 : %.9f\n", accum);
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
		
		if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
			perror( "clock gettime" );
			return EXIT_FAILURE;
		}

		 // 메시지 전송
            if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                 perror("msgsnd() error");
                 exit(0);
	  	}

	     printf("------------예약 완료------------\n\n");

		if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
			perror( "clock gettime" );
			return EXIT_FAILURE;
		}
		accum = ( stop.tv_sec - start.tv_sec )
		+ (double)( stop.tv_nsec - start.tv_nsec )
		/ (double)BILLION;
		printf("수행시간 : %.9f\n", accum);

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

		if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
			perror( "clock gettime" );
			return EXIT_FAILURE;
		}

		if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                perror("msgsnd() error");
                exit(0);
                }

		if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
			perror( "clock gettime" );
			return EXIT_FAILURE;
		}
		accum = ( stop.tv_sec - start.tv_sec )
		+ (double)( stop.tv_nsec - start.tv_nsec )
		/ (double)BILLION;
		printf("수행시간 : %.9f\n", accum);

		printf("------------변경 완료------------\n\n");

	  }else if(num==4){

		mybuf.msgtype=1;
                strcpy(mybuf.buf,"4");

		if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
                        perror( "clock gettime" );
                        return EXIT_FAILURE;
                }

                if (msgsnd(key_id, &mybuf, sizeof(msgbuf), IPC_NOWAIT) == -1) {
                perror("msgsnd() error");
                exit(0);
		}
		if (msgrcv(key_id, &mybuf, sizeof(msgbuf), 1, 0) == -1) {
                        perror("msgrcv() error");
                        exit(0);
                }

		if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
                        perror( "clock gettime" );
                        return EXIT_FAILURE;
                }
                accum = ( stop.tv_sec - start.tv_sec )
                + (double)( stop.tv_nsec - start.tv_nsec )
                / (double)BILLION;
                printf("수행시간 : %.9f\n", accum);

		remain_seat=mybuf.remain_seat;
                remain_print();

	  }else if(num==0){
		exit(0);
	  }
	  else{
		  printf("입력 에러입니다.");
	  }
    
  }
}
void remain_print(){
	
	printf("\n\n잔여 좌석  : %d\n\n",remain_seat);
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
void check_update(){
	msgbuf checkbuf;
	int key_id;
	
	key_id = msgget((key_t) 60014, IPC_CREAT|0666);
  	if (key_id == -1) {
    		perror("msgget() error");
    		exit(0);
  	}
	while(1){
		
		if (msgrcv(key_id, &checkbuf, sizeof(msgbuf), 1, 0) == -1) {
                        perror("msgrcv() error");
                        exit(0);
                }else{
			for(int i=0;i<9;i++){
				for(int j=0;j<9;j++){
					if(seat[i][j]!=checkbuf.data[i][j]){
						printf("\n\n변경 사항 발생\n\n");
					}
				}
			}
		}




	}

}

void print_time(){
	time_t tcurrent;
	tcurrent = time(NULL);
	printf("\nThe current time = %s\n", ctime(&tcurrent));
	sleep(60);
}
