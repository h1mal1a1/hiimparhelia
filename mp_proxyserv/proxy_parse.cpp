#include <cstdlib>
#include "proxy_parse.h"

#define DEFAULT_NHDRS   8

//использую va_list, для того, чтобы передавать неизвестное кол-во параметров
void debug(const char* format, ...)
{
    va_list args;
    if(DEBUG)
    {
        va_start(args,format);
        vfprintf(stderr,format,args);
        va_end(args);
    }
}

int ParsedHeader_set(struct ParsedRequest* pr,const char* key, const char* value)
{
    struct ParsedHeader* ph;
    ParsedHeader_remove(pr,key);

    if(pr->headerslen <= pr->headersused+1)
    {
        pr->headerslen=pr->headerslen*2;
        pr->headers=(struct ParsedHeader *)realloc(pr->headers,pr->headerslen*sizeof(struct ParsedHeader));

        if(!pr->headers)
            return -1;

        ph = pr->headers+pr->headersused;
        pr->headersused+=1;

        ph->key=(char*)malloc(strlen(key)+1);
        memcpy(ph->key,key,strlen(key));
        ph->key[strlen(key)]='\0';

        ph->value=(char*)malloc(strlen(value)+1);
        memcpy(ph->value,value,strlen(value));
        ph->value[strlen(value)] = '\0';

        ph->keylen = strlen(key)+1;
        ph->valuelen = strlen(value)+1;
        return 0;
    }
}
/* Создаёт структуру заголовка */
void ParsedHeader_create(struct ParsedRequest* pr)
{
    pr->headers = (struct ParsedHeader*)malloc(sizeof(struct ParsedHeader)*DEFAULT_NHDRS);//Выделяем память размером struct ParsedHeader * дефолтные значения заголовка
    pr->headerslen = DEFAULT_NHDRS;
    pr->headersused = 0;
}

struct ParsedHeader* ParsedHeader_get(struct ParsedRequest* pr, const char* key)
{

}

//удаляет указанный ключ из parsedHeader
int ParsedHeader_remove(struct ParsedRequest* pr, const char* key)//2
{
    struct ParsedHeader* tmp;
    tmp = ParsedHeader_get(pr,key);
    if(tmp == NULL)
        return -1;

    free(tmp->key);
    free(tmp->value);
    tmp->key=NULL;
    return 0;
}

/*

size_t ParsedHeader_headersLen(struct ParsedRequest* pr)
{

}

void ParsedRequest_destroy(struct ParsedRequest* pr)
{

}
*/
//создаём структуру PR
struct ParsedRequest* ParsedRequest_create()
{
    struct ParsedRequest *pr;
    pr = (struct ParsedRequest*)malloc(sizeof(struct ParsedRequest));
    if(pr != NULL)
    {
        ParsedHeader_create(pr);
        pr->buf = NULL;
        pr->method = NULL;
        pr->protocol = NULL;
        pr->host = NULL;
        pr->path = NULL;
        pr->version = NULL;
        pr->buf = NULL;
        pr->buflen = 0;
    }
    return pr;
}

int ParsedHeader_parse(struct ParsedRequest* pr,char* line)
{
    char* key;
    char* value;
    char* index1;
    char* index2;

    index1=index(line,':');
    if(index1==NULL)
    {
        debug("No colon found\n");
        return -1;
    }

    key=(char*)malloc((index1-line+1)*sizeof(char));
    memcpy(key,line,index1-line);
    key[index1-line]='\0';

    index1+=2;
    index2=strstr(index1,"\r\n");
    memcpy(value,index1,(index2-index1));
    value[index2-index1]='\0';

    ParsedHeader_set(pr,key,value);
    free(key);
    free(value);
    return 0;
}

int ParsedRequest_unparse(struct ParsedRequest* pr,char* buf, size_t buflen)
{

}

int ParsedRequest_unparse_headers(struct ParsedRequest* pr,char* buf, size_t buflen)
{

}

size_t ParsedRequest_totalLen(struct ParsedRequest* pr)
{

}

int ParsedRequest_parse(struct ParsedRequest* parse, const char* buf, int buflen)
{
}
