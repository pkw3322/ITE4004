#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]){
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_adr;
    int str_len;

    // ./ gcc client.c -0 client
    // ./client 127.0.0.1 9091 <- 실행 시킬 때 어떤 ip로 어떤 port를 통해 실행 시킬지
    //인자가 3개가 아니라면
    if (argc != 3){
        printf("Usage: %s <IP> <PORT> \n",argv[0]);
    }

    //socket은 file discripter 형태로 반환 됨
    sock = socket(PF_INET, SOCK_STREAM,0);

    //socket 생성 시 에러가 나오면 결과 값 -1로 나옴
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET; //tcpip 통신 하겠다.
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]); //ip 저장
    serv_adr.sin_port = htons(atoi(argv[2])); //port 저장

    //connet 시도 
    if(connect(sock, (struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1){
        //실패시 -1 반환 하기 때문에 error handling
        error_handling("connect() error");
    }
    else {
        //connect 성공 시 연결
        puts("Connected ----->");
    }
    while(1){
        fputs("Input message(Q,q to quit):",stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n")){
            break;
        }
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE-1);
        message[str_len] = 0;
        
        printf("Message from server: %s",message);

    }
    close(sock);//socket 닫기 닫지 않게 되면 socket이 계속 열려 있는 상태이기 때문에 해킹에 취약할 수도 있음
    return 0;
}

//message를 stderr 형태로 나타냄
void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}