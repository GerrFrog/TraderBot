#pragma once 

#ifndef REQUEST_HEADER
#define REQUEST_HEADER

#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <map>
#include <iostream>

/**
 * @brief All Simple Requests (GET, POST, JSON)
 */
namespace Request::Simple
{
    /**
     * @brief Object of JSON Request and Response.
     */
    class JSON_Curl
    {
        private:
            /**
             * @brief Base URL to request.
             */
            std::string base_url;

            /**
             * @brief The URL to request.
             */
            std::string req_url;

            /**
             * @brief This is our callback function 
             * which will be executed by curl on perform.
             * Here we will copy the data received to our struct
             * 
             * @param ptr Pointer to the data received by curl
             * @param size The count of data items received
             * @param nmemb Number of bytes
             * @param data Our pointer to hold the data
             * @return size_t size * nmemb
             */
            static size_t curl_callback(void *ptr, size_t size, size_t nmemb, std::string* data) {
                data->append((char*)ptr, size * nmemb);
                return size * nmemb;
            }

            /**
             * @brief Clean up all Curl objects
             * 
             * @param curl Curl object
             * @param headers Headers of request
             */
            void cleanup(CURL *curl, curl_slist *headers)
            {
                if (curl)
                    curl_easy_cleanup(curl);
                if (headers)
                    curl_slist_free_all(headers);
                curl_global_cleanup();
            }

        public:
            /**
             * @brief Construct a new json curl object
             */
            JSON_Curl() { };

            /**
             * @brief Construct a new json curl object
             * 
             * @param base_url The URL to request
             */
            JSON_Curl(std::string base_url) : base_url(base_url) { }

            /**
             * @brief Destroy the json curl object
             */
            ~JSON_Curl() { }

            /**
             * @brief Set the base URL
             * 
             * @param url 
             */
            void set_base_url(std::string url)
            {
                this->base_url = url;
            }

            /**
             * @brief Construct the Request
             * 
             * @param params Params for request
             */
            void construct_request(std::map<std::string, std::string> &params);

            /**
             * @brief Execute Request
             * 
             * @param url The URL to request
             * @return nlohmann::json The JSON Object
             */
            nlohmann::json request();

            /**
             * @brief Execute Request
             * 
             * @param url The URL to request
             * @return nlohmann::json The JSON Object
             */
            nlohmann::json request(const std::string &url);
    };
}




















#endif