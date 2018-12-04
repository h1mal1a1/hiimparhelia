#include <iostream>
#include "proxy.h"
#include "proxy_parse.h"
using namespace std;

void datafromclient(void* sockid)
{
    int MAX_BUFFER_SIZE = 5000;
    char buf[MAX_BUFFER_SIZE];
    int newsockfd = *((int*)sockid);
    char *request_message;//получаем сообщение из URL

    request_message = (char*) malloc(MAX_BUFFER_SIZE);//выделяет память под наш buffer и возвращает на неё указатель
    if (request_message == NULL)
    {
        fprintf(stderr, "Ошибка в выделении памяти \n");
        exit(1);
    }

    request_message[0] = '\0';
    int total_recieved_bits = 0;

    //определяем конец запроса
    while(strstr(request_message, "\r\n\r\n") == NULL)//Ищем до символа конца
    {
        int recvd = recv(newsockfd,buf,MAX_BUFFER_SIZE,0);
        if (recvd < 0)
        {
            fprintf(stderr, "Ошибка в цикле запроса \n");
            exit(1);
        }
        else
        if(recvd == 0)
            break;
        else
        {
            total_recieved_bits +=recvd;

            //Если наше сообщение оказалось больше чем наш буффер, то мы увеличиваем размер нашего буфера в 2 раза
            buf[recvd] = '\0';
            if(total_recieved_bits > MAX_BUFFER_SIZE)
            {
                MAX_BUFFER_SIZE *= 2;
                request_message = (char*) realloc(request_message, MAX_BUFFER_SIZE);
                if(request_message == NULL);
                {
                    fprintf(stderr, " Ошибка в перераспределении памяти \n");
                    exit(1);
                }
            }
        }

        strcat(request_message,buf);
    }

    struct ParsedRequest *req;

    req = ParsedRequest_create();

    if(ParsedRequest_parse(req,request_message,strlen(request_message)) < 0)
    {
        fprintf(stderr, "Ошибка в запросе сообщения. Разрешены только http и get заголовки. \n");
        exit(0);
    }
}

int main(int argc, char** argv)
{
    int sockfd,newsockfd;

    struct sockaddr_in serv_addr;
    struct sockaddr cli_addr;

    if (argc < 2)
    {
        fprintf(stderr, "Извините! Уточните Порт! \n");
    }

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0)
    {
        fprintf(stderr,"Извините, невозможно создать сокет \n");
        return 1;
    }

    memset(&serv_addr,0,sizeof(serv_addr));

    int portnumb=atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // используем для того, чтобы не связывать сокет с определённым IP-адресом.
    serv_addr.sin_port = htons(portnumb);

    int binded = bind(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));//Привязывает имя(serv_addr) к сокету(sockfd).
    if (binded < 0)
    {
        fprintf(stderr,"Ошибка в связывании! \n");
        return 1;
    }

    listen(sockfd,100);//слушает соединение на сокете(2ой параметр это очередь)

    int clilen = sizeof(struct sockaddr);

    //начинаем обрабатывать запрос веб-браузера
    while(1)
    {
        newsockfd = accept(sockfd, &cli_addr, (socklen_t*) &clilen);//соединяет новый сокет с извлечённым из очереди запросом
        if (newsockfd < 0)
            fprintf(stderr,"Ошибка в принятии запроса,то есть ");

        int pid = fork();//создаём дочерний процесс(идентичный процессу родителя

        if (pid == 0 )
        {
            datafromclient((void*)&newsockfd);//получаем данные от клиента
            close(newsockfd);
            _exit(0);
        }
        else
            close(newsockfd);
    }

    close(sockfd);

    return 0;
}