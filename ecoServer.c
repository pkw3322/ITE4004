#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]){
    int serv_sock,clnt_sock; //나한테 들어오는 sock, 내 sock
    char message[BUF_SIZE];
    int str_len;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    if (argc != 2){ // ./server 9091 내가 여는 내 port만 명시해주면 됨
        printf("Usage: %s <PORT> \n",argv[0]);
        exit(1);
    }

    //socket 열기
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); //TCPIP 통신 하겠다

    if (serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); //내가 쓸 수 있는 ip를 사용하겠다.
    serv_adr.sin_port = htons(atoi(argv[1])); //인자로 받은 port 저장

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }

    clnt_adr_sz = sizeof(clnt_adr);
    for (int i = 0; i < 5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        if(clnt_sock == -1){
            error_handling("clnt_sock error");
        }
        else {
            printf("Connected client %d \n", i + 1);
        }

        while((str_len=read(clnt_sock, message, BUF_SIZE)) != 0){
            write(clnt_sock,message,str_len);
        }
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

//message를 stderr 형태로 나타냄
void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}