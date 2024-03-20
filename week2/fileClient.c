#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 50

void error_handling(char *message);

int main(int argc,char *argv[]){
    FILE *fp;

    char buf[BUF_SIZE];
    int read_cnt;
    int sd;
    struct sockaddr_in serv_adr;

    if(argc != 3){// ./client 0.0.0.0 9091
        printf("Usage %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }

    fp = fopen("receive.txt","wb"); //쓰기 형식으로 파일을 생성, 열기
    sd = socket(PF_INET,SOCK_STREAM,0);
    //PF_INET: tcp/ip 통신을 하겠다는 약속, ipv4
    //if ipv6 사용 시 PF_INET6
    //SOCK_STREAM: TCP 통신을 하겠다는 뜻 If UDP 사용 시 SOCK_DGRAM : UDP
    //0: IPPROTO_TCP

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET; //tcp통신에서 ipv4형식으로 나타내겠다는 뜻
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]); // 서버 ip
    serv_adr.sin_port = htons(atoi(argv[2])); //서버 port
    //127.0.0.1, 9091 (2진수 또는 16진수)
    //atoi: 문자열을 int로 바꾸기 위해
    //htons 서버와 클라이언트의 OS를 고려하고 환경을 맞추기 위한 약속 정하는 과정

    if(connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        //실패시 -1 반환 하기 때문에 error handling
        error_handling("connect() error");
    }
    else {
        //connect 성공 시 연결
        puts("Connected ----->");
    }
    //(struct sockaddr*)&serv_adr => typecasting 
    //serv_adr은 sockaddr_in이기 때문에 이를 sockaddr로 type casting 중이다.
    
    while((read_cnt = read(sd,buf,BUF_SIZE)) != 0)
        fwrite((void *)buf, 1, read_cnt, fp);

    puts("complete!");
    write(sd,"bye",5);
    fclose(fp);
    close(sd);
    return 0;
}


//message를 stderr 형태로 나타냄
void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}