#include "../inc/request.hpp"

nlohmann::json Request::JSON_Curl::request()
{
	curl_global_init(CURL_GLOBAL_ALL); // sets the program environment
	CURL *curl = curl_easy_init(); // initialize curl
	std::string response_string;
	struct curl_slist *headers = NULL; //linked-list string structure

	if (!curl)
	{
		// cout << "ERROR : Curl initialization\n" << endl;
		this->cleanup(curl, headers);
        this->req_url = "";
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
	curl_easy_setopt(curl, CURLOPT_URL, this->req_url.c_str()); // pass the url
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback); // pass our call back function to handle received data
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string); // pass the variable to hold the received data


	CURLcode status = curl_easy_perform(curl); 
	if (status != 0)
	{
		// cout << "Error: Request failed on URL : " << url << endl;
		// cout << "Error Code: " << status << " Error Detail : " << curl_easy_strerror(status) << endl;
		this->cleanup(curl, headers);
        this->req_url = "";
		return NULL;
	}

	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	curl_global_cleanup();

    this->req_url = "";

	return nlohmann::json::parse(response_string);
}

nlohmann::json Request::JSON_Curl::request(const std::string &url)
{
	curl_global_init(CURL_GLOBAL_ALL); // sets the program environment
	CURL *curl = curl_easy_init(); // initialize curl
	std::string response_string;
	struct curl_slist *headers = NULL; //linked-list string structure

	if (!curl)
	{
		// cout << "ERROR : Curl initialization\n" << endl;
		this->cleanup(curl, headers);
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


	CURLcode status = curl_easy_perform(curl); 
	if (status != 0)
	{
		// cout << "Error: Request failed on URL : " << url << endl;
		// cout << "Error Code: " << status << " Error Detail : " << curl_easy_strerror(status) << endl;
		this->cleanup(curl, headers);
		return NULL;
	}

	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	return nlohmann::json::parse(response_string);
}

void Request::JSON_Curl::construct_request(std::map<std::string, std::string> &params)
{
	std::string req_param = "";

    for ( const auto &[key, value]: params )
		req_param += key + '=' + value + '&';
	req_param.pop_back();

	this->req_url = this->base_url + '?' + req_param;
}



