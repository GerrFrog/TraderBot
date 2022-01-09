#include "main.hpp"

// using namespace std;
using std::cout, std::string, std::endl;

void Cleanup(CURL *curl, curl_slist *headers)
{
	if (curl)
		curl_easy_cleanup(curl);
	if (headers)
		curl_slist_free_all(headers);
	curl_global_cleanup();
}

/*This is our callback function which will be executed by curl on perform.
here we will copy the data received to our struct
ptr : points to the data received by curl.
size : is the count of data items received.
nmemb : number of bytes
data : our pointer to hold the data.
*/
size_t curl_callback(void *ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

/*
Send a url from which you want to get the json data
*/
string GetRequest(const string url)
{
	curl_global_init(CURL_GLOBAL_ALL); // sets the program environment
	CURL *curl = curl_easy_init(); // initialize curl
	std::string response_string;
	struct curl_slist *headers = NULL; //linked-list string structure
	if (!curl)
	{
		cout << "ERROR : Curl initialization\n" << endl;
		Cleanup(curl, headers);
		return NULL;
	}

	headers = curl_slist_append(headers, "User-Agent: libcurl-agent/1.0");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Cache-Control: no-cache");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	//ignore ssl verification if you don't have the certificate or don't want secure communication
	// you don't need these two lines if you are dealing with HTTP url
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); //maximum time allowed for operation to finish, its in seconds
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // pass the url
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback); // pass our call back function to handle received data
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string); // pass the variable to hold the received data


	CURLcode status = curl_easy_perform(curl); // execute request
	if (status != 0)
	{
		cout << "Error: Request failed on URL : " << url << endl;
		cout << "Error Code: " << status << " Error Detail : " << curl_easy_strerror(status) << endl;
		Cleanup(curl, headers);
		return NULL;
	}

	// do the clean up
	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	return response_string;
}

int main(int argc, char *argv[]) {
    dotenv::load("../.env");

    const std::string pk = dotenv::get("PRIVATE_KEY");
    const std::string sk = dotenv::get("SECRET_KEY");
    const std::string taapi_key = dotenv::get("TAAPI_KEY");

    boost::asio::io_context ioctx;
    binapi::rest::api api(
        ioctx
        ,"api.binance.com"
        ,"443"
        ,pk
        ,sk
        ,10000 // recvWindow
    );

    auto account = api.account_info();
    if ( !account ) {
        std::cerr << "account info error: " << account.errmsg << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "account info: " << account.v << std::endl << std::endl;

    // CURL *curl;//Объект CURL

    // //объект в который будет записан результат вызова функции curl_easy_perform
    // CURLcode res;

    // //выполняем инициализацю
    // curl = curl_easy_init();

    // if(curl) { //проверяем все ли ОК
    //     //дальше работаем с опциями

    //     //задаем опцию получить страницу http://nullflow.blogspot.com
    //     // curl_easy_setopt(curl, CURLOPT_URL, "https://api.binance.com");
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://api.taapi.io/ema?secret=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlbWFpbCI6ImdoYWdoYWw5M0BnbWFpbC5jb20iLCJpYXQiOjE2NDEwMzQ2NjAsImV4cCI6Nzk0ODIzNDY2MH0.RQt0ocdRR_RwF5V3NbxxafvTSHactzx2MJ_qBWSq_fY&exchange=binance&symbol=BTC/USDT&interval=1h");

    //     //указываем использовать прокси сервер
    //     // curl_easy_setopt(curl, CURLOPT_PROXY, "ip_proxy:8080");

    //     //задаем опцию отображение заголовка страницы
    //     curl_easy_setopt(curl, CURLOPT_HEADER, 1);

    //     //вызываем функцию curl_easy_perform которая обработает все наши заданные опции и вернет результат, если res будет равен 0 то начит всё прошло успешно, иначе возникла какая то ошибка.
    //     res = curl_easy_perform(curl);

    //     //завершаем нашу сессию
    //     curl_easy_cleanup(curl);

    //     if (res == CURLE_OK)
    //     {
    //         std::cout << res << std::endl;
    //     } else {
    //         std::cout << "Error " << res << std::endl;
    //     }
    // }

    cout << Indicators::TAAPI::EMA(taapi_key, "BTC/USDT", "1h", 30) << endl;

    return EXIT_SUCCESS;
}