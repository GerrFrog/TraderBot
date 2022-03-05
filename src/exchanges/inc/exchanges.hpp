#pragma once

#ifndef BINAPI_HEADER
#define BINAPI_HEADER

#include <stdio.h>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <chrono>
#include <openssl/hmac.h>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <string.h>
#include <nlohmann/json.hpp>

using std::string, std::cout, std::endl, std::unordered_map, std::to_string;

/**
 * @brief All API for Exchanges
 */
namespace Exchanges::Binance
{
    /**
     * @brief Binance API 
     * 
     * @note https://github.com/binance/binance-signature-examples/blob/master/cpp/spot.cpp
     * @note https://docs.microsoft.com/en-us/answers/questions/590872/how-to-clear-data-downloaded-with-curl-from-the-pr.html
     */
    class Binance_API
    {
        private:
            /**
             * @brief Type of CURL object
             */
            typedef void (*cleanup)(CURL*);

            /**
             * @brief Typedef of CurlHandle
             */
            typedef std::unique_ptr<CURL, cleanup> CurlHandle;

            /**
             * @brief Request Header
             */
     		struct curl_slist *chunk = NULL;

            /**
             * @brief CurlHandle object
             */
            CurlHandle curlHandle;

            /**
             * @brief Received content from response
             */
            string content;

            /**
             * @brief Private key 
             */
            string private_key;

            /**
             * @brief Secret key
             */
            string secret_key;

            /**
             * @brief Base Binance API URL
             */
            const string baseUrl = "https://api.binance.com";

            /**
             * @brief Get the Timestamp object
             * 
             * @return string 
             */
            string getTimestamp() 
            {
                long long ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                return std::to_string(ms_since_epoch);
            }

            /**
             * @brief Encrypt string with HMAC
             * 
             * @param key Encryption Key
             * @param data Data
             * @return string 
             */
            string encryptWithHMAC(const char* key, const char* data) 
            {
                unsigned char *result;
                static char res_hexstring[64];
                int result_len = 32;
                string signature;

                result = HMAC(EVP_sha256(), key, strlen((char *)key), const_cast<unsigned char *>(reinterpret_cast<const unsigned char*>(data)), strlen((char *)data), NULL, NULL);
                for (int i = 0; i < result_len; i++) {
                    sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
                }

                for (int i = 0; i < 64; i++) {
                    signature += res_hexstring[i];
                }

                return signature;
            }

            /**
             * @brief Get the Signature object
             * 
             * @param query Query
             * @return string 
             */
            string getSignature(string &query) 
            {
                string signature = encryptWithHMAC(this->secret_key.c_str(), query.c_str());
                return "&signature=" + signature;
            }

            /**
             * @brief Setup Parameters for request
             * 
             * @param parameters 
             * @return string 
             */
            string joinQueryParameters(const std::unordered_map<string,string> &parameters) 
            {
                string queryString = "";
                for (auto it = parameters.cbegin(); it != parameters.cend(); ++it) {
                        if (it == parameters.cbegin()) {
                            queryString += it->first + '=' + it->second;
                        }

                        else {
                            queryString += '&' + it->first + '=' + it->second;
                        }
                }

                return queryString;
            }

            /**
             * @brief Write Callback Function for Curl
             * 
             * @param contents 
             * @param size 
             * @param nmemb 
             * @param userp 
             * @return size_t 
             */
            static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp) 
            {
                std::string *stuff = static_cast<std::string*>(userp);
                stuff->append(contents, size * nmemb);
                return size * nmemb;
            }

            /**
             * @brief Execute HTTP Request to URL
             */
            void executeHTTPRequest() {
                CURLcode res;

                this->content.clear();

                res = curl_easy_perform(curlHandle.get());

                if(res != CURLE_OK)
                    fprintf(
                        stderr, 
                        "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res)
                    );
            }

            /**
             * @brief Send Public Request to Binance (do not use private/secret keys)
             * 
             * @param urlPath Request URL
             * @param parameters Parameters for Request
             */
            void sendPublicRequest(const string &urlPath, unordered_map<string,string> &parameters) 
            {
                string url = baseUrl + urlPath + '?';
                if (!parameters.empty()) {
                    url += joinQueryParameters(parameters);
                }
                curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEDATA, &content);
                curl_easy_setopt(curlHandle.get(), CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(curlHandle.get(), CURLOPT_URL, url.c_str());
                curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEFUNCTION, &Binance_API::WriteCallback);
                executeHTTPRequest();
            }

            /**
             * @brief Send Signed Request to Binance (use private/secret keys)
             * 
             * @param httpMethod HTTP Method
             * @param urlPath URL Path
             * @param parameters Parameters for Request
             */
            void sendSignedRequest(const string &httpMethod, const string &urlPath, unordered_map<string,string> &parameters) 
            {
                string url = baseUrl + urlPath + '?';
                string queryString = "";
                string timeStamp = "timestamp=" + getTimestamp();
                if (!parameters.empty()) {
                    string res = joinQueryParameters(parameters) + '&' + timeStamp;
                    url += res;
                    queryString += res; 
                }

                else {
                    url += timeStamp;
                    queryString += timeStamp;
                }

                string signature = getSignature(queryString);
                url += signature;
                queryString += signature;

                curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEDATA, &content);
                curl_easy_setopt(curlHandle.get(), CURLOPT_FOLLOWLOCATION, 1L);

                if (httpMethod == "POST") {
                    curl_easy_setopt(curlHandle.get(), CURLOPT_URL, (baseUrl + urlPath).c_str());
                    curl_easy_setopt(curlHandle.get(), CURLOPT_POSTFIELDS, queryString.c_str());
                    curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEFUNCTION, &Binance_API::WriteCallback);
                    executeHTTPRequest();
                }

                else {
                    curl_easy_setopt(curlHandle.get(), CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curlHandle.get(), CURLOPT_WRITEFUNCTION, &Binance_API::WriteCallback);
                    executeHTTPRequest();
                }
            }

        public:
            /**
             * @brief Construct a new Bin_API object
             * 
             * @param pk Private API key
             * @param sc Secret API key
             */
            Binance_API(
                const string &pk,
                const string &sk
            ) : private_key(pk), 
                secret_key(sk),
                curlHandle(curl_easy_init(), &curl_easy_cleanup)
            { 
        		this->chunk = curl_slist_append(this->chunk, ("X-MBX-APIKEY:" + this->private_key).c_str());
                curl_easy_setopt(curlHandle.get(), CURLOPT_HTTPHEADER, this->chunk);
            }

            /**
             * @brief Destroy the Bin_API object
             */
            // ~Binance_API() = default;
            ~Binance_API()
            {
                curl_slist_free_all(this->chunk);
            }

            /**
             * @brief Set the keys 
             * 
             * @param pk Private Binance API key
             * @param sk Secret Binance API key
             */
            void set_keys(
                const string &pk,
                const string &sk
            ) { this->private_key = pk; this->secret_key = sk; }

            /**
             * @brief Get information about exchange
             * 
             * @param symbol Symbol (pair)
             * @return nlohmann::json 
             */
            nlohmann::json exchange_info(const string &symbol)
            {
                std::unordered_map<string,string> parameters;

                parameters.insert({
                    {"symbol", symbol}
                });

                sendPublicRequest("/api/v3/exchangeInfo", parameters);

                return nlohmann::json::parse(this->content);
            }

            /**
             * @brief Get the current price of cryptocurrency
             * 
             * @param pair Pair
             * @return nlohmann::json 
             */
            nlohmann::json get_current_price(const string& pair)
            {
                std::unordered_map<string, string> parameters;

                parameters.insert({
                    {"symbol", pair}
                });

                sendPublicRequest("/api/v3/ticker/price", parameters);

                return nlohmann::json::parse(this->content);
            }

            /**
             * @brief Get account info
             * 
             * @return nlohmann::json
             */
            nlohmann::json account_info()
            {
                std::unordered_map<string,string> parameters;

        		sendSignedRequest("GET", "/api/v3/account", parameters);

                return nlohmann::json::parse(this->content);
            }

            /**
             * @brief Get the balance of account
             * 
             * @param symbol Symbol (pair)
             * @return nlohmann::json 
             */
            nlohmann::json get_balance(const string &symbol)
            {
                std::unordered_map<string,string> parameters;

        		sendSignedRequest("GET", "/api/v3/account", parameters);

                nlohmann::json response = nlohmann::json::parse(this->content)["balances"];
                for (int i = 0; i < response.size(); i++)
                {
                    if (response[i]["asset"] == symbol)
                        return response[i];
                }
                return NULL;
            }

            /**
             * @brief Open new Order
             * 
             * @param symbol Symbol (pair)
             * @param side Side (BUY/SELL)
             * @param quantity Quantity
             * @return nlohmann::json 
             */
            nlohmann::json open_new_order(
                const string &symbol,
                const string &side,
                const string &quantity
            )
            {
                std::unordered_map<string,string> parameters;

                if (side == "BUY")
                    parameters.insert({
                        {"symbol", symbol},
                        {"side", side},
                        {"type", "MARKET"},
                        {"quoteOrderQty", quantity}
                    });
                if (side == "SELL")
                    parameters.insert({
                        {"symbol", symbol},
                        {"side", side},
                        {"type", "MARKET"},
                        {"quantity", quantity}
                    });

                sendSignedRequest("POST", "/api/v3/order", parameters);

                return nlohmann::json::parse(this->content);
            }
    };
}
























#endif