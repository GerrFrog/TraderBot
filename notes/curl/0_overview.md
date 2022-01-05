# Curl Library for C++

## Links
- [Link1](https://www.cyberforum.ru/blogs/131347/blog1151.html)
- [Link2](http://nullflow.blogspot.com/2011/07/c-curl.html)
- [Link3](https://curl.se/libcurl/c/libcurl-tutorial.html)

Curl interfaces:
1. Easy
2. Multi
3. Share

## Functions
- CURL *curl_easy_init(); - Начинает easy(простую) curl-сессию, возвращает её дескриптор, в случае неудачи NULL. Каждому вызову такой ф-ции должен соответствовать вызов ф-ции:
- void curl_easy_cleanup(CURL * curl); - exit 
- CURLcode curl_easy_setopt(CURL *curl, CURLoption option, parameter); - Задает соответствующего поведение через установку опций, возвращает CURLE_OK (0) если успешно, в противном случае код ошибки определенный константами в файле curl/curl.h. Опция CURLOPT_URL относится NETWORK-опциям и задает url- адрес. Этот параметр должен иметь вид согласно RFC 3986 формата: "scheme://host:port/path"
- CURLcode curl_easy_perform(CURL *curl); - Выполняет отправку запроса на сервер и возвращает CURLE_OK в случае успеха, в противном случае код ошибки

## Headers
```c
curl_easy_setopt(curl_handle, CURLOPT_USERAGENT,"Mozilla/5.0 (Windows NT 6.1; rv:16.0) Gecko/20100101 Firefox/16.0");
curl_easy_setopt(curl_handle, CURLOPT_ENCODING, "gzip,deflate"); // если curl скомпилина вместе с gzip 
// curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPALIVE , 1);  // не нашло такой опции в версии 7.19.3
// curl_easy_setopt(curl_handle, CURLOPT_REFERER,"http://some.com"); // yстанавливает referer
curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER,1);// автоматически заполняет поле referer
```
- curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1);Если установить - заголовки ответа сервера будут отображаться в месте с html-кодом страницы (заголовок + тело)

## Error Handler
```c
CURLcode res = curl_easy_perform(curl_handle);
if(res != CURLE_OK)
     printf( "curl_easy_perform() failed: %s\n", curl_easy_strerror(res) );
```
### Get Error from buffer
```c
static char ErrorBuffer[CURL_ERROR_SIZE]; // размер определяется константой curl
//...
curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errorBuffer); // указывает буфер ошибок
//...
CURLcode res = curl_easy_perform(curl_handle);
if(res != CURLE_OK)
     cout<<"Error!"<<ErrorBuffer<< endl;
```

## Redirect
- curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1); - Для автоматического перехода на перенаправляемую страницу необходимо установить опцию CURLOPT_FOLLOWLOCATION в 1
- curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 10); //  останавливаться после 10-ого редиректа - Для того что бы ограничить количество перенаправлений нужно установить опцию CURLOPT_MAXREDIRS её параметр указывает их максимальное количество

## Cookie
- curl_easy_setopt(curl_handle, CURLOPT_COOKIE, cookiestring); - Параметр -указатель на строку в стиле Си для установки cookies в http-запросе .
Формат строки должен быть вида name=contents, где name имя cookie, а contents- её содержание. Используется, когда вы хотите указать точное содержание cookie-заголовоков для отправки на сервер. Если нужно передать несколько cookie, то строка должна выглядеть как "name1 = content1; name2 = content2;" и т. д.
- curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, cookiefilename); - Принимает указатель на строку в стиле Си, которой хранится путь к файлу, который содержит cookies. Сookies должны хранится в формате куков Netscape/Mozilla или в обычном HTTP-стиле заголовков (Set-Cookie: ...) помещенных в файл. Если указать несуществующий файл или пустую строку (""), то это разрешит libcurl полученные использовать cookie в следующих запросах для данного дескриптора curl Можно несколько раз устанавливать эту опцию для загрузки нескольких файлов с куками.
- curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, cookiefilename); - Запишет все известные cookies в указанный файл. Если cookies нет файл не будет создан. Если указать "-" вместо имени файла cookies будут выведены в окно консоли ( stdout ), и будет разрешено использование cookies для этой сессии Если файл не может быть создан или сохранен libcurl не выдаст ошибку. Использование опций CURLOPT_VERBOSE или CURLOPT_DEBUGFUNCTION вызовет вывод предупреждения.
- curl_easy_setopt(curl_handle, CURLOPT_COOKIESESSION, 1); - Установка в 1 укажет текущему сеансу начать новую "сессию" cookies. Это заставит libcurl проигнорировать все "сессионные" cookies, которые она должна была бы загрузить, полученные из предыдущей сессии. По умолчанию, libcurl всегда сохраняет и загружает все cookies, вне зависимости от того, являются ли они "сессионными" или нет. "Сессионные" cookies - это cookies без срока истечения, которые должны существовать только для текущей "сессии". 
- curl_easy_setopt(curl_handle, CURLOPT_COOKIELIST, "ALL"); // Чистка cookies - Устанавливает cookies. Если задать параметр "ALL" то все cookies будут очищены , если "FLUSH" будут сохранены в файл указанный в опции CURLOPT_COOKIEJAR
### COUT cookie
```c
static void print_cookies(CURL *curl)
{
    CURLcode res;
    struct curl_slist *cookies;
    struct curl_slist *nc;
    int i;
 
    printf("Cookies, curl knows:\n");
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n",
                curl_easy_strerror(res ));
        exit(1);
    }
    nc= cookies, i = 1;
    while (nc)
    {
        printf("[%d]: %s\n", i, nc->data);
        nc = nc->next;
        i++;
    }
    if (i == 1)  printf("(none)\n");
 
    curl_slist_free_all(cookies);
}
```
## Convert to URL
Иногда возникает необходимость передавать GET/POST запросы параметры которые содержать в себе символы требующие "экранирования" ( кириллица, символ "@" ).
К примеру необходимо выполнить поиск в яндексе по слову "Программирование", если посмотреть в строку браузера, то там это будет выглядеть так:
- http://yandex.ua/yandsearch?text=Программирование

Экранировкани
- http://yandex.ua/yandsearch?text=%CF%F0%EE%E3%F0%E0%EC%EC%E8%F0%EE%E2%E0%ED%E8%E5

char* curl_easy_escape( CURL* curl , char* url , int length );

Для удобства использования с std::string можно написать такую функцию
```c
std::string escape(CURL *curl_handle, const std::string& text)
{
    std::string result;
    char* esc_text= curl_easy_escape(curl_handle, text.c_str(), text.length());
    if(!esc_text) throw std::runtime_error("Can not convert string to URL");
 
    result = esc_text;
    curl_free(esc_text);
 
    return result;
}
```




## Examples
### Simple query
```c
#include <curl/curl.h>

int main()
{
    CURL *curl;//Объект CURL

    //объект в который будет записан результат вызова функции curl_easy_perform
    CURLcode res;

    //выполняем инициализацю
    curl = curl_easy_init();

    if(curl) { //проверяем все ли ОК
        //дальше работаем с опциями

        //задаем опцию получить страницу http://nullflow.blogspot.com
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.binance.com");

        //указываем использовать прокси сервер
        // curl_easy_setopt(curl, CURLOPT_PROXY, "ip_proxy:8080");

        //задаем опцию отображение заголовка страницы
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        //вызываем функцию curl_easy_perform которая обработает все наши заданные опции и вернет результат, если res будет равен 0 то начит всё прошло успешно, иначе возникла какая то ошибка.
        res = curl_easy_perform(curl);

        //завершаем нашу сессию
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            std::cout << res << std::endl;
        } else {
            std::cout << "Error " << res << std::endl;
        }
    }
}
```
### Simple query
```c
int main()
{
    CURL *  curl_handle = curl_easy_init();
    if(curl_handle)
    {
        // задаем  url адрес
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://www.cyberforum.ru");
        // выполняем запрос
        CURLcode res = curl_easy_perform(curl_handle);
        // закрываем дескриптор curl
        curl_easy_cleanup(curl_handle);
    }
 
    getchar();
    return 0;
}
```
### Save to buffer
```c
#include <string>
#include <iostream>
#include "curl/curl.h"
static size_t write_data(char *ptr, size_t size, size_t nmemb, string* data)
{
    if (data)
    {
        data->append(ptr, size*nmemb);
        return size*nmemb;
    }
    else 
    return 0;  // будет ошибка
}
int main()
{
    CURL *curl_handle;
    curl_handle = curl_easy_init();
 
    if (curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, "google.com");
 
    std::string content;
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &content);
 
        CURLcode res = curl_easy_perform(curl_handle);
        if (res)  std::cout << content << std::endl;
        else      std::cerr << curl_easy_strerror(res) << std::endl;
 
        curl_easy_cleanup(curl_handle);
    }
 
    getchar();
    return 0;
}
```
- char * ptr - указатель на принимаемые данные.
- size_t size - размер принимамого блока данных
- size_t nmemb - общее количество блоков данных.
- void* userdata - это параметр опции CURLOPT_WRITEDATA, в который производится запись - наш буфер.
### Save to file
```c
#include <string>
#include <iostream>
#include "curl/curl.h"
size_t write_data( char *ptr, size_t size, size_t nmemb, FILE* data)
{
    return fwrite(ptr, size, nmemb, data);
}
int main()
{
    // Открываем файлы для заголовка и тела
 
    const std::string header_filename= "head.txt";
    const std::string body_filename  = "body.html";
 
    FILE *header_file= fopen(header_filename.c_str(),"w");
    if (header_file == NULL) 
    return -1;
 
    FILE *body_file =  fopen(body_filename.c_str(),"w");
    if (body_file == NULL)  
    return -1;
 
    // Выполняем  запрос
    CURL *curl_handle = curl_easy_init();
    if(curl_handle)
    {
    const std::string url= "https://www.cyberforum.ru";
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
 
        // сохраняем тело
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, body_file);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
        // сохраняем заголовок
        curl_easy_setopt(curl_handle, CURLOPT_WRITEHEADER, header_file);
 
        CURLcode res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)
            std::cout<< "curl_easy_perform() failed: %s\n" << curl_easy_strerror(res) std::endl;
        curl_easy_cleanup(curl_handle);
    }
 
    std::cout<< "\nDone!"<<std::endl;
    getchar();
    return 0;
}
```
### POST query (Authorization)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "curl/curl.h"
std::string escape(CURL *curl_handle, const std::string& text)
{
    std::string result;
    char* esc_text= curl_easy_escape(curl_handle, text.c_str(), text.length());
    if(!esc_text) throw std::runtime_error("Can not convert string to URL");
 
    result = esc_text;
    curl_free(esc_text);
 
    return result;
}
static size_t write_data(char *ptr, size_t size, size_t nmemb, std::string* data)
{
    if (data)
    {
        data->append(ptr, size*nmemb);
        return size*nmemb;
    }
    else return 0;  // будет ошибка
}
static size_t write_head(char *ptr, size_t size, size_t nmemb, std::ostream* stream)
{
    (*stream)<< std::string(ptr, size*nmemb);
    return size*nmemb;
}
int main()
{
    /* Пользовательские данные */
    const std::string url_dologin= "https://www.cyberforum.ru/login.php?do=login";// страница авторизации
  const std::string url_user= "https://www.cyberforum.ru/usercp.php"; // Мой кабинет
  
    std::string user_name;  //  e-mail
    std::string password;   //  пароль
 
    std::cout<<"e-mail: ";
    getline(std::cin,user_name);
    std::cout<<password<<"password: ";
    getline(std::cin,password);
    std::cout<<std::endl;
 
    CURL *curl_handle = curl_easy_init();
    if(curl_handle)
    {
        /* Формирование  запроса на основе пользовательских данных */
        std::string post_data;
        post_data+= "vb_login_username="  + escape(curl_handle, user_name);
        post_data+= "&cookieuser=1";
        post_data+= "&vb_login_password=" + escape(curl_handle, password);
        post_data+= "&s=&securitytoken=guest";
        post_data+= "&do=login";
        post_data+= "&vb_login_md5password=";
        post_data+= "&vb_login_md5password_utf=";
 
        curl_easy_setopt(curl_handle, CURLOPT_URL, url_dologin.c_str() );
 
        //  сохраняем html код cтраницы в строку content
    std::string content;
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,     &content);
 
        // Загловок ответа сервера выводим в консоль
        curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, write_head);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEHEADER,    &std::cout);
 
        // авто перенаправление
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
        // max 5-ть перенаправлений
        curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5);
        // разрешаем использовать куки
        curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE,"");
 
        /* POST- запрос c авторизацией ( происходит получение кукисов ) */
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post_data.c_str() );
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, post_data.length() );
 
        CURLcode res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)
        {
            std::cout<< curl_easy_strerror(res) << std::endl;
            getchar();
            return 0;
        }
        // Проверяем успешно ли авторизировались
        if( content.find("Спасибо, что зашли") != std::string::npos )
            std::cout << "Authenticated!" <<std::endl<<std::endl;
        else
        {
            std::cout<< "Non-Authenticated!" <<std::endl;
            curl_easy_cleanup(curl_handle);
            getchar();
            return 0;
        }
        /* GET- запрос для перехода в "Мой кабинет" форума */
        content.clear();
        // меняем POST-режим на GET
        curl_easy_setopt(curl_handle, CURLOPT_POST, 0);
        curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);
        // меняем url
        curl_easy_setopt(curl_handle, CURLOPT_URL, url_user.c_str() );
 
        res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)  std::cout<< curl_easy_strerror(res) <<std::endl;
 
        // Проверяем получили то что ожидали
        if( content.find("Мой кабинет") != std::string::npos)
            std::cout << ""My cabinet" downloaded."  <<std::endl
                      << "------------- Content -----------------" <<std::endl
                      << content <<std::endl;
        else  std::cout << "Is not "My cabinet" page" <<std::endl;
 
        curl_easy_cleanup(curl_handle);
    }
 
    getchar();
    return 0;
}
```
- разрешить использовать полученные cookie;
- включить автоматический переход по перенаправлениям.

По умолчанию libcurl использует GET "режим" отправки запросов, после установки опции CURLOPT_POSTFIELDSIZE он переключается на POST, поэтому для осуществление последующего GET нужно переключить режим на GET:
```c
curl_easy_setopt(curl_handle, CURLOPT_POST, 0); // выключаем POST
curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);// включаем GET
```


### Retrieving compressed pages.
```c
#include <string>
#include <iostream>
#include "curl/curl.h"
int main()
{
    const std::string url= "http://www.google.ru/";
 
    CURL *curl_handle = curl_easy_init();
    if(curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str() );
        curl_easy_setopt(curl_handle, CURLOPT_ENCODING, "gzip,deflate"); // Принудительно ставим gzip
 
        CURLcode res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)
            std::cout<<"Error #"<<res<<" "<<curl_easy_strerror(res) <<std::endl;
 
        curl_easy_cleanup(curl_handle);
    }
 
    std::cout<<std::endl<<"Done!";
    getchar();
    return 0;
}
```
### Send with HTTPS
```c
#include <stdio.h>
#include <stdlib.h>
 
#include <string>
#include <iostream>
 
#include "curl/curl.h"
size_t write_data( char *ptr, size_t size, size_t nmemb, FILE* data)
{
    return fwrite(ptr, size, nmemb, data);
}
int main()
{
    // Открываем файлы для заголовка и тела
    std::string body_filename  = "body.html";
    FILE *body_file =  fopen(body_filename.c_str(),"w");
    if (body_file == NULL)  return -1;
 
    std::string url= "https://my.webmoney.ru/login.aspx";
 
    CURL *curl_handle = curl_easy_init();
    if(curl)
        {
            // сохранение в файл html-страницу
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, body_file);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
            // заголовки ответа выводим в консоль
            curl_easy_setopt(curl_handle, CURLOPT_WRITEHEADER, stdout);
 
               /* HTTPs  Запрос */
            curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str() );
 
             // не проверять SSL сертификат
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
            // не проверять Host SSL сертификата
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
 
            CURLcode res = curl_easy_perform(curl_handle);
            if(res != CURLE_OK)
                 std::cout<<"Error #"<<res<<" "<<curl_easy_strerror(res) <<std::endl;
 
             curl_easy_cleanup(curl_handle);
        }
 
    std::cout<<std::endl<<"Done!";
    getchar();
    return 0;
}
```




























