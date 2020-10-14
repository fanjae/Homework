#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
int main(int argc, char *argv[])
{
	int serv_sock; // 서버 소켓
	int clnt_sock; // 클라이언트 소켓

	struct sockaddr_in serv_addr; // 서버 정보
	struct sockaddr_in clnt_addr; // 클라이언트 정보
	socklen_t clnt_addr_size;

	char message[]="Hello. 20132334!"; // 서버가 Client에게 보낼 메시지

	if(argc!=2) // Argument 오류 처리.
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET,SOCK_STREAM,0); // 소켓 생성
            // PF_INET : Ipv4 프로토콜 SOCK_STREAM : TCP 프로토콜
	if(serv_sock == -1) // 소켓 생성이 실패한 경우
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); // serv_addr 메모리 초기화
	serv_addr.sin_family=AF_INET; // 주소 체계 IPv4로 설정
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); // 현재 실행중인 서버 ip를 넣는다.
	serv_addr.sin_port=htons(atoi(argv[1])); // port 주소는 1번째 argument로 한다.

	if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) // serv_sock에 IP주소와 PORT 번호를 할당한다.
	{
		error_handling("bind() error");
	}

	if(listen(serv_sock, 5) == -1) // serv_sock에 Client 요청을 대기한다. backlog의 개수는 5개이다.
	{
		error_handling("listen() error");
	}

	clnt_addr_size=sizeof(clnt_addr); // client의 주소 크기 만큼 할당 받는다.
	clnt_sock=accept(serv_sock, (struct sockaddr *) &clnt_addr,&clnt_addr_size); // binding된 정보를 토대로 accept 받는다.
	if(clnt_sock == - 1) // accept 되지 않은 경우 오류 처리
	{
		error_handling("accept() error");
	}
	else // 연결된 경우 Client가 접속 했음을 알린다. 
	{
		printf("Client %s login.\n",inet_ntoa(clnt_addr.sin_addr)); // clnt_addr.sin_addr에 있는 주소 정보를 ipv4 주소 체계로 변경하여 출력한다.
	}

	write(clnt_sock, message, sizeof(message)); // Client 소켓 측으로 메시지를 쓴다.
	close(clnt_sock); // Client 소켓을 닫는다.
	close(serv_sock); // Server 소켓을 닫는다.
	return 0;
}
