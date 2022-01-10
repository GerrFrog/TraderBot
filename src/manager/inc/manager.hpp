#pragma once

#ifndef MANAGER_HEADER
#define MANAGER_HEADER

#include <string>
#include <iostream>
#include <map>

#include "../../indicators/inc/indicators.hpp"

using std::map, std::string, std::cout, std::endl;

/**
 * @brief Trading Manager
 */
class Trade_Manager
{
    private:
        /**
         * @brief taapi.io key
         */
        string taapi_key;

    public:
        /**
         * @brief Construct a new Trade_Manager object
         * 
         * @param key taapi.io key
         */
        Trade_Manager(const string &key) : taapi_key(key) { }

        void start()
        {
            Indicators::TAAPI::EMA("", "", "", 5);
        }
};




















#endif