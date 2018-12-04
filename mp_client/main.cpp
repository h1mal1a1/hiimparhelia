#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

void* writeSck(void *);
void* readSck(void* );

int BUF_SIZE = 1024;

int main()
{
    setlocale(LC_ALL,"Russian");
    char message[1024];
    char buff[sizeof(message)];

    int sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    if(sock < 0)
    {
        perror("socketCli error");
        exit(1);
    }

    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(5003);

    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
        perror("ConnectCli failed");
        exit(2);
    }

    ssize_t nsize;
    while((nsize = recv(sock,buff,sizeof(buff)-1,0)) != -1 )
    {
        buff[nsize] = 0;
        printf("S=>C:%s",buff);
        printf("S<=C:");
        fgets(&buff[0],sizeof(buff)-1,stdin);

        send(sock,&buff[0],strlen(&buff[0]),0);


    }
    close(sock);
    return -1;
}

/*void writeSck()
{
    for(;;)
    {
        char s[100];
        cin.getline(s,BUF_SIZE);
        send(sock,s,strlen(s),0);
    }
    close(sock);
}

void readSck()
{
    char test[BUF_SIZE];
    bzero(test,BUF_SIZE+1);
    bool loop = false;

    while(!loop)
    {
        bzero(test,BUF_SIZE+1);

        int rc = read(sock,test,BUF_SIZE);
        if(rc>0)
        {
            string tester(test);
            cout << ": " << tester << endl;

            if(tester == "exit_server")
                break;
        }
    }
    cout << "\nClosing thread and conn" << endl;
    close(sock);
}
*/
/*
 *      printf("Введите сообщение серверу:");
        if(!strcmp(fgets(message,sizeof(message),stdin),"exit"))
        {
            close(sock);
            exit(2);
        }
        printf("отправка сообщения на сервер...\n");
        send(sock,message,sizeof(message),0);
        int bytes_read=0;
        printf("Ожидание сообщения\n");
        bytes_read = recv(sock,buff,sizeof(message),0);
        printf("Получено %d bytes\tСообщение: %s\n",bytes_read,buff);
 *
 */