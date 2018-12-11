#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <pthread.h>
#include <thread>
#include <regex>
#include <thread>
#include <arpa/inet.h>
#include <set>
#include <algorithm>
#include <fcntl.h>
using namespace std;


//1.Получить запрос и вытащить из него хост +
//2.На этот хост отправить запрос +
//3.Полученный ответ от хоста отправить клиенту.

int sockCli = 0;
char clientBuffer[1024];
char servBuffer[1024];

void* cliToServ(int sock);
void* servToCli(int sock,char* buffer);
void createNewSock(string host,int sock);

string explodeStr(string& str);
string explodeRequest(string str);
string getHost(char* buffer);

int main() {
    int listenfd = 0;
    struct sockaddr_in serv_addr;
    string host;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5001);

    if(bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(listenfd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in client_addr;
    socklen_t client_addr_size=sizeof(client_addr);

    while(1) {
        sockCli = accept(listenfd,(sockaddr *)&client_addr, &client_addr_size);//соединили слущающий сокет с клиентом

        cliToServ(sockCli);//получили сообщение от клиента

        string tmpHost = getHost(clientBuffer);// получили хост

        int newsock;
        thread th(createNewSock,tmpHost,newsock);//создали новый сокет для отправки запроса на хост
        //this_thread::sleep_for(chrono::seconds(3));
        th.detach();

        //cout << clientBuffer <<  " - THis IS clientBuff 2"<< endl;

        servToCli(newsock,servBuffer);//отправили запрос на хост

        cliToServ(newsock);//получили ответ от хоста на отправленный запрос

        servToCli(sockCli,servBuffer);//отправили полученный ответ от хоста в браузер
        //recv(newsock,servBuffer,sizeof(servBuffer)-1,0);
    }


    close(listenfd);
    return 0;
}


//получение сообщения от клиента к серверу
void* cliToServ(int sock) {
    recv(sock,clientBuffer,sizeof(clientBuffer)-1,0);
}

//отправка сообщения от сервера к клиенту
void* servToCli(int sock,char* buffer) {
    send(sock,buffer,sizeof(buffer)-1,0);
}

void createNewSock(string host,int sock) {
    struct sockaddr_in sock_addr;//структура сокета
    struct hostent* raw_host;
    raw_host = gethostbyname(host.c_str());


    timeval time_out;
    time_out.tv_sec = 0;
    time_out.tv_usec = 5000000;

    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        perror("new sock failed");
        exit(1);
    }
    sock_addr.sin_family=AF_INET;
    sock_addr.sin_port=htons(5000);

    in_addr* address = (in_addr * )raw_host->h_addr;
    string ip_address = inet_ntoa(* address);

    sock_addr.sin_addr.s_addr = *(in_addr_t*)raw_host->h_addr;

    if(connect(sock,(struct sockaddr *)&sock_addr,sizeof(sock_addr)) == -1) {
        perror("connect");
        exit(3);
    }
}


//парсер по \n
string explodeStr(string& str) {
    string desired_str;

    int i=0;

    while(str[i] != '\n') {
        desired_str += str[i];
        str.erase(str.begin());
    }
    str.erase(str.begin());

    return desired_str;
}

string getHost(char* buffer) {
    string request = buffer;//преобразуем char в string
    string* allRequest = new string[2]; //создаем временный массив для хранения элементов запросов

    allRequest[0]=explodeStr(request);//получили заголовок
    allRequest[1]=explodeStr(request);//получили host
    string tmpHost = allRequest[1];
    string host = explodeRequest(tmpHost);

    if(host[host.size()-1] == '\r')
        host.pop_back();

    cout << host << " - This is HOST" << endl;
    return host;
}

//парсит строку по :
string explodeRequest(string str) {
    regex key("[^:]+");
    cmatch request;

    regex_search(str.c_str(),request,key);
    string firstpath = request[0];
    regex tmpKey(firstpath);
    str = regex_replace(str,tmpKey,"");

    regex remGarbage(":\x20");
    str = regex_replace(str,remGarbage,"");
    string secPath = str;

    if(secPath[secPath.size()-1] == '\n')
        secPath.pop_back();

    return secPath;
}