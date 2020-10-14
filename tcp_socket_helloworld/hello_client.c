#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr; // 서버 정보를 담는 구조체
	char message[30];
	int str_len;

	if(argc != 3) // argument 처리
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓을 생성한다.
	// PF_INET : IPv4 프로토콜, SOCK_STREAM : TCP 프로토콜
	if(sock == -1) // 소켓 생성에 실패한 경우 오류 출력
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); // serv_addr 메모리 초기화
	serv_addr.sin_family = AF_INET; // 주소 체계 IP_v4로 설정
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 32비트 ip 주소 값을 넣는다.
	serv_addr.sin_port = htons(atoi(argv[2])); // oprt 주소는 2번째 argument로 한다.

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) // 연결을 시도한다. 성공시 0, 실패시 -1
	{
		error_handling("connect() error!");
	}
	
	str_len = read(sock, message, sizeof(message)-1); // 소켓을 통해 받아온 메시지를 읽어온다.

	if(str_len == -1) 
	{
		error_handling("read() error!");
	}

	printf("Message from server: %s\n", message); // 메시지를 출력한다.
	close(sock); // 소켓을 닫는다.
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
