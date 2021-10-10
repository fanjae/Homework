#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char *message);
int set_data(int opnds[]);
int new_calculate(int result, int opnds[], char op, int i);
int calculate(int result, int opnum, int opnds[], char oprator);

int main(int argc, char *argv[])
{
	char opinfo[BUF_SIZE];
	int serv_sock, clnt_sock; // 서버 소켓 및 클라이언트 소켓
	int result, opnd_cnt, oper_cnt;
	int recv_cnt, recv_len;
	struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라이언트 주소
	socklen_t clnt_adr_sz;  
	
	if(argc != 2) { // Argument가 2가 아닌경우 (올바른 실행형태가 아니므로 오류 출력)
		printf("Usage a Port : <%s>\n",argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); // 소켓을 생성한다.
	// PF_INET : Ipv4 프로토콜, SOCK_STREAM : TCP Protocol
	// 프로토콜 체계가 PF_INET이므로 int protocol 부분에 IPPROTO_TCP 부분으로 명시한다.

	if(serv_sock == -1) // 소켓 생성이 실패한 경우
		error_handling("socket() error"); // 에러 처리를 한다.
	
	memset(&serv_adr,0,sizeof(serv_adr)); // serv_adr을 0으로 메모리 초기화 과정을 거친다.
	serv_adr.sin_family = AF_INET; // 주소 체계를 Ipv4로 설정한다.
	/* 의미상 AF_INET과 PF_INET은 같다. 하지만 socket 함수에는 PF_INET을 쓰는 것이 바람직하고,	
	struct sockaddr_in 구조체에 주소 체계를 넣을 때는 AF_INET을 넣는 것이 바람직 하다고 한다. */
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 현재 실행중인 서버 ip를 넣는다.	
	serv_adr.sin_port = htons(atoi(argv[1])); // port 주소는 1번째 argument로 한다. (0번째는 실행파일)
	

	if(bind(serv_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == - 1)  // serv_sock에 IP주소와 PORT 번호를 할당한다.  
		error_handling("bind() error"); 
	if(listen(serv_sock,5) == -1) // serv_sock에 Client 요청을 대기한다. backlog는 5개이다. 
		error_handling("listen() error");
	clnt_adr_sz = sizeof(clnt_adr); //  client의 주소 크기만큼 받는다.
	
	for(int i = 0; i < 5; i++) {
		opnd_cnt = 0;
		oper_cnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz); // binding 정보를 가지고 accept한다.
		// Client는 사용하지 않는다.
		read(clnt_sock, &opnd_cnt,1); // 클라이언트로 소켓으로부터 operand 개수를 읽어온다.
		read(clnt_sock, &oper_cnt,1); // 클라이언트로 소켓으로부터 operator 개수를 읽어온다.
		recv_len = 0;
		while((opnd_cnt * OPSZ + 2) > recv_len) { 
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE-1); // 클라이언트 소켓으로부터 나머지 메시지를 읽어온다.
			recv_len += recv_cnt;
		}
			
		result = set_data((int *) opinfo); // 첫번째 값을 받아온다.i
		for(int i = 1; i <= oper_cnt; i++) 
				result = new_calculate(result, (int *)opinfo, opinfo[recv_len-oper_cnt-2+i],i);
		write(clnt_sock, (char *)&result, sizeof(result)); // client에게 result된 결과값을 보내준다. client socket을 통해서 이를 보내준다.
		close(clnt_sock);
	}		
	close(serv_sock);
	return 0;
}

/* result 초기화를 하는 함수이다. */
int set_data(int opnds[]) 
{
	return opnds[0]; // 0번째 값을 넣어준다.
}
int new_calculate(int result, int opnds[], char op, int i) // 새로운 계산기. 기존 계산기는 항상 같은 연산자이기 때문에 함수 내부에서 반복이 일어나지만, 이 함수는 그렇지 않다.
{
	switch(op)
	{
		case '+': // 덧셈인 경우
			result += opnds[i]; // 결과 값에 opnds[i]를 더한다.
			break;
		case '-': // 뺄셈인 경우
			result -= opnds[i]; // 결과 값에 opnds[i]를 뺀다.
			break;
		case '*': // 곱셈인 경우
			result *= opnds[i]; // 결과 값에 opnds[i]를 곱한다.
			break;
		case '/': // 나눗셈인 경우
			if(opnds[i] == 0) { // 0인 경우에는 1로 나눴다고 처리한다.
				result /= 1; // 결과 값을 1로 나눈다.
			}
			else {
				result /= opnds[i]; // 결과 값에 opnds[i]로 나눈다.
			}
			break;
	}
	return result; // 결과값 리
}	

/* 기존 계산기의 계산 방식(여기에 나눗셈은 임의로 추가하였다.)
int calculate(int result,int opnum, int opnds[], char op)
{
	switch(op)
	{
		case '+':
			for(int i = 1; i < opnum; i++) result += opnds[i];
			break;
		case '-':
			for(int i = 1; i < opnum; i++) result -= opnds[i];
			break;
		case '*':
			for(int i = 1; i < opnum; i++) result *= opnds[i];
			break;
		case '/':
			for(int i = 1; i < opnum; i++) {
				if(opnds[i] == 0) {
					result /= 1;
				}
				else {
					result /= opnds[i];
				}
			}
			break;
	}
	return result;
}
*/

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
