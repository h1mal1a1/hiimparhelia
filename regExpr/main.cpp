#include <iostream>
#include <string>
#include <regex>
#include <cstring>
#include <string.h>

using namespace std;

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

void explodeHeader(string str)
{
    regex symbSpace("[^\x20]+");//символ пробела
    cmatch meth;

    regex_search(str.c_str(),meth,symbSpace);
    string method = meth[0];
    regex tmpMeth(method);//удаляем из строки метод
    str = regex_replace(str,tmpMeth,"");
    cout << method << "-this is method" << endl;

    regex_search(str.c_str(),meth,symbSpace);
    string URI = meth[0];
    regex tmpURI(URI);
    str = regex_replace(str,tmpURI,"");
    cout << URI << " -this is URI" << endl;

    regex_search(str.c_str(),meth,symbSpace);
    string lastpart = meth[0];

    if(lastpart[lastpart.size()-1]=='\n')
        lastpart.pop_back();

    cout << lastpart << " -this is lastpart" << endl;
}

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

    cout << firstpath << " - " << secPath << endl;
    return secPath;
}

int main()
{
    string fullstr = "POST http://ocsp.digicert.com/ HTTP/1.1\n"
                    "Host: ocsp.digicert.com\n"
                    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:61.0) Gecko/20100101 Firefox/61.0\n"
                    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
                    "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\n"
                    "Accept-Encoding: gzip, deflate\n"
                    "Content-Length: 83\n"
                    "Content-Type: application/ocsp-request\n"
                    "Connection: keep-alive";

    string header = "POST http://ocsp.digicert.com/ HTTP/1.1\n";
    string request = "Host: ocsp.digicert.com\n";
    cmatch result;

    string* massStr = new string[10];
    massStr[0] = explodeStr(fullstr);
    massStr[1] = explodeStr(fullstr);
    cout << massStr[0] << " - massStr0" << endl;
    cout << massStr[1] << " - massStr1" << endl;

    string tmpHost = massStr[1];
    string onlyHost = explodeRequest(tmpHost);
    cout << onlyHost << "This is host " << endl;

    //explodeHeader(header);
    //explodeRequest(request);

    return 0;
}