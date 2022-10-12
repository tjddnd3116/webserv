#include <cstdio>
#include <iostream>
#include <cstring>
#include <sys/_types/_socklen_t.h>
#include <sys/fcntl.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/event.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <string.h>



# define BUFF_SIZE 1024

int main(void)
{
	socklen_t client_addr_size;

	int server_socket;
	int client_socket;
	int htmlFD;

	// struct sockaddr_in {
	//     __uint8_t       sin_len;
	//     sa_family_t     sin_family;
	//     in_port_t       sin_port;
	//     struct  in_addr sin_addr;
	//     char            sin_zero[8];
	// };

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	char buff_rcv[BUFF_SIZE + 5];
	char buff_snd[BUFF_SIZE + 5];

	htmlFD = open("./index.html", O_RDONLY);
	read(htmlFD, buff_snd, BUFF_SIZE);
	close(htmlFD);

	// 서버 소켓 생성
	// AF_INET : IPv4 인터넷 프로토콜
	// SOCK_STREAM : 순차적, 신뢰할수 있는 양방향 바이트 스트림 연결
	// 0 : 소켓과 함께 사용될 특정 프로토콜을 지정
	// 		0으로 지정하면 운영체제는 가장 적절한 프로토콜을 선택함
	// 		스트림 소켓에는 TCP를 선택하고 데이터그램 소켓은 UDP를 선택함
	// 		비정상적 상황을 제외하고는 항상 0으로 지정
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		std::cout << "socket() 실행 에러\n" << std::endl;


	// server_addr 설정
	// sockaddr_in 에는 4개의 필드가 존재
	// sin_family : address family 설정
	// sin_port	: 포트 번호 설정
	// sin_addr.s_addr : 호스트의 ip주소가 포함됨
	// 					서버 코드의 경우 이것은 항상 서버가 실행되는 시스템의 ip주소
	// 					이 주소를 가져오는 INADDR_ANY라는 메크로가 있음
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 소켓을 address에 바인딩함
	// 소켓 파일 디스크립터, 바인딩되는 주소, 바인딩되는 주소의 크기를 인자로 넣어줌
	if (bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		std::cout << "bind() 실행 에러\n" << std::endl;

	// 시스템 콜을 통해 프로세스는 listen의 소켓으로 연결을 수신대기 할수 있음
	// 첫번째 인자는 소켓 파일디스크립터
	// 두번째 인자는 백로그 큐의 크기, 즉 프로세스가 특정 연결을 처리하는 동안 대기할 수 있는 연결 수
	// 대부분의 시스템에서 허용하는 최대 크기인 5로 설정해야함
	if (listen(server_socket, 2) == -1)
		std::cout << "listen() 실행 에러\n" << std::endl;

	std::cout << "------ accept waiting" << std::endl;
	// accept 시스템콜로 인해 클라이어트가 서버에 연결할 때까지 프로세스가 blocking 됨
	// 클라이언트에서 성공적으로 연결되면 프로세스를 깨움
	// 새 파일 디스크립터를 리턴하고 이 연결에 대한 모든 통신은 새 파일 디스크립터를 사용하여 수행해야함
	// 두번째 인자는 연결의 다른 쪽 끝에 있는 클라이어늩 주소에 대한 참조 포인터
	// 세번째 인자는 이 구조의 크기
	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
	if (client_socket < 0)
		std::cout << "accept error" << std::endl;

	std::cout << "------ accept client" << std::endl;
	while (1)
	{
		// int flag;
		int read_ret;
		std::cout <<"----- read wait" << std::endl;

		memset(buff_rcv, 0, sizeof(buff_snd));
		read_ret = read(client_socket, buff_rcv, BUFF_SIZE);
		// flag = fcntl(client_socket, F_GETFL, 0);
		// fcntl(client_socket, F_SETFL, flag | O_NONBLOCK);

		if (read_ret < 0)
		{
			std::cout << "----- read error" << std::endl;
			std::cout << "----- socket close" << std::endl;
			close(client_socket);
		}
		else if (read_ret == 0)
		{
			std::cout << "----- socket close" << std::endl;
			close(client_socket);
			break;
		}
		else
		{
			std::cout << "----- read :" << buff_rcv << std::endl;
			send(client_socket, buff_snd, strlen(buff_snd) +1, 0);
		}
		std::cout << "----- other routine processing" << std::endl;
		// close(client_socket);
	}
	return (0);
}
