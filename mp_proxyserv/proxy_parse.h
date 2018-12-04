#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <ctype.h>

#ifndef MP_PROXYSERV_PROXY_PARSE_H
#define MP_PROXYSERV_PROXY_PARSE_H

#define DEBUG 0

//ParsedRequest эта структура хранит в себе объекты HTTP запроса, которые уже отпарсили
struct ParsedRequest {
    char* method;
    char* protocol;
    char* host;
    char* port;
    char* path;
    char* version;
    char* buf;
    size_t buflen;
    struct ParsedHeader *headers;
    size_t headersused;
    size_t headerslen;
};

//Любые заголовки после строки запроса это пара ключ-значение в формате "key:value\r\n"
struct ParsedHeader {
    char* key;
    size_t keylen;
    char* value;
    size_t valuelen;
};

//Примеры: Строка запроса(Request Line) : GET /utilities/weatherfull/city/hyderabad HTTP/1.1, всё что ниже это тело

//Конструктор,который создаёт пустой объект
struct ParsedRequest* ParsedRequest_create();

/*Парсит запросы из строки запросов в буфер,buflen это его размер*/
int ParsedRequest_parse(struct ParsedRequest* parse, const char* buf, int buflen);

//Дескриптор
void ParsedRequest_destroy(struct ParsedRequest* pr);

//Возвращает полный буффер из объекта, который был распарсен. buf может быть имеет размер buflen с учётом строки запроса,заголовка и символами отделения тела от запроса,т.е. \r\n
int ParsedRequest_unparse(struct ParsedRequest* pr, char* buf, size_t buflen);

//Возвращает полный буфер , без строки запроса.Если это не заголовок, то вернёт /r/n.
int ParsedRequest_unparse_headers(struct ParsedRequest* pr,char* buf, size_t buflen);

//Длина строки запроса,заголовков и спец символов
size_t ParsedRequest_totalLen(struct ParsedRequest* pr);

//Длина заголовка, если его нету, то /r/n
size_t ParsedHeader_headersLen(struct ParsedRequest* pr);

//Set,Get,Remove это функции,которые работают с заголовками(устанавливают,удаляют)
int ParsedHeader_set(struct ParsedRequest* pr, const char* key, const char* value);

struct ParsedHeader* ParsedHeader_get(struct ParsedRequest* pr, const char* key);

int ParsedHeader_remove(struct ParsedRequest* pr, const char* key);

//Выводим отладочную информацию, если DEBUG установлен в 1
void debug(const char* format, ...);

#endif //MP_PROXYSERV_PROXY_PARSE_H
