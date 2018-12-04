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

using namespace std;

int sockCli = 0;
char clientBuff[1024];
void* serviceToClient(void* client_socket);
string explodeStr(string& str);
string explodeRequest(string str);
string getHost();
/*void* writeSck(void*);
void* readSck(void*);*/

int main()
{
    int listenfd = 0;//mainsocket
    struct sockaddr_in serv_addr;
    string host;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5003);

    if(bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(listenfd, 10) <0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in client_addr;
    socklen_t client_addr_size=sizeof(client_addr);

    while(1)
    {
        sockCli = accept(listenfd,(sockaddr *)&client_addr, &client_addr_size);
        pthread_t* thId = new pthread_t;
        pthread_create(&thId[0],NULL,serviceToClient,NULL);
        pthread_join(thId[0],NULL);
        getHost();
        //pthread_create(&thId[1],NULL,writeSck,NULL);
        //pthread_join(thId[1],NULL);
    }


    return 0;
}

string getHost()
{
    string request = clientBuff;//преобразуем char в string
    string* allRequest = new string[2]; //создаем временный массив для хранения элементов запросов

    allRequest[0]=explodeStr(request);//получили заголовок
    allRequest[1]=explodeStr(request);//получили host
    string tmpHost = allRequest[1];
    string host = explodeRequest(tmpHost);

    if(host[host.size()-1] == '\r')
        host.pop_back();

    string host2 = host;
    cout << host << "-This is Host" << endl;
    return host;
}

//Потоковая функция
void* serviceToClient(void* )
{
    recv(sockCli,clientBuff,sizeof(clientBuff)-1,0);
    send(sockCli,clientBuff,sizeof(clientBuff)-1,0);
}

//парсер по \n
string explodeStr(string& str)
{
    regex key("[^\n]+");
    cmatch meth;

    regex_search(str.c_str(),meth,key);
    string tmpStr=meth[0];
    regex tmpReg(tmpStr);
    str=regex_replace(str.c_str(),tmpReg,"");

    return tmpStr;
}


//парсит строку по :
string explodeRequest(string str)
{
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

/*void* writeSck(void *)
{
    for(;;)
    {
        char s[1024];

        cout << "<---";
        bzero(s,BUF_SIZE+1);
        cin.getline(s,BUF_SIZE);

        send(sockCli,s,strlen(s),0);
    }
    close(sockCli);
}

void* readSck(void* )
{
    char test[BUF_SIZE];
    bzero(test,BUF_SIZE+1);
    bool loop = false;

    while(!loop)
    {
        bzero(test,BUF_SIZE+1);

        int rc = read(sockCli,test,BUF_SIZE);
        if(rc>0)
        {
            string tester(test);
            cout << ": " << tester << endl;

            if(tester == "exit_server")
                break;
        }
    }
    cout << "\nClosing thread and conn" << endl;
    close(sockCli);
}*/

