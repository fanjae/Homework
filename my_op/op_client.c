#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt, oper_cnt;
	struct sockaddr_in serv_address;

	if(argc <= 2) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);   // 소켓을 생성한다. PF_INET : IPv4 프로토콜, SOCK_STREAM : TCP 프로토콜
	// 프로토콜 체계가 PF_INET 이므로 int protocol 부분에 구체적으로 프로토콜 체계를 IPPROTO_TCP로 변경하였다.

	if(sock==-1) // 소켓 생성에 실패한 경우
		error_handling("socket() error");

	memset(&serv_address, 0, sizeof(serv_address)); // serv_adr를 0으로 메모리 초기화 과정을 거친다.
	serv_address.sin_family=AF_INET; // 주소 체계를 Ipv4로 설정한다.
	/* 의미상 AF_INET과 PF_INET는 같으나 socket 함수에는 PF_INET으로 쓰는 것이 바람직하고
	struct sockaddr_in 구조체에 주소 체계를 넣을 때에는 AF_INET을 넣는 것이 바람직하다고 한다. */

	serv_address.sin_addr.s_addr=inet_addr(argv[1]); // 32비트 ip 주소는 1번째 argument로 한다. (0번째는 실행파일)
	serv_address.sin_port=htons(atoi(argv[2])); // port 주소는 2번째 argument로 한다.

	if(connect(sock, (struct sockaddr *)&serv_address, sizeof(serv_address))==-1) // 연결을 시도한다. 성공시 0, 실패시 -1을 반환한다.
		error_handling("connect() error!"); // 연결에 실패한 경우 출력.
	// exit(1) // QnA : 서버는 클라이언트와 binding 오류가 떠도 종료되면 안되더라도 Client는 괜찮지 않을까?
	else
		puts("Connected Success!"); // 연결 성공시.

	fputs("Operand count: ", stdout); 
	scanf("%d", &opnd_cnt); // 피연산자를 개수를 입력 받는다.
	opmsg[0] = (char)opnd_cnt;	
	fputs("Operator count: ", stdout);
	scanf("%d", &oper_cnt); // 연산자 개수를 입력 받는다.
	opmsg[1] = (char)oper_cnt;


	if(opnd_cnt <= 1) {  // 피연산자의 개수가 1개 이하인 경우
		fprintf(stderr,"Error : Operand it's more than 1.\n");	
		exit(1);
	}
	
	if(oper_cnt != opnd_cnt-1) { // 본 계산기는 피연산자 사이에 연산자 1개씩 들어간다고 가정함에 따라 그렇지 않은 경우 오류.
		fprintf(stderr,"Error : Operator and Operand it's not matching\n"); // matching 연산자랑 연산자 개수가 매칭 되지 않으면 오류.
		exit(1);
	}

	
	for(int i=0; i<opnd_cnt; i++) {
		printf("Operand %d : ", i+1);
		scanf("%d", (int *)&opmsg[i*OPSZ+2]); /* 맨 앞에서 0번째 1번째는 피연산자 수와 연산자 수가 들어간다. 따라서 그 다음
		부터 피연산자를 넣을 수 있도록 계산하였다. */
	}


	fgetc(stdin);
//	fputs("Operator: ",stdout);
//	scanf("%c",&opmsg[opnd_cnt*OPSZ+2]);

	for(int i=0; i<oper_cnt; i++) {
		printf("Operator %d : ", i+1);
		scanf("%c", &opmsg[opnd_cnt*OPSZ+2+i]); /* 맨 뒤부터 연산자를 채워준다.	
		연산자를 채워줄때 가장 끝부분에 넣기 시작한다. */
		fgetc(stdin);
	}

	write(sock, opmsg, opnd_cnt*OPSZ+2+oper_cnt+1); // socket에 Message를 보낸다.
	read(sock, &result, RLT_SIZE); // socket으로 결과 데이터를 읽어온다. 

	
	printf("Operation result: %d \n", result);
	close(sock); // 소켓을 닫는다.
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


