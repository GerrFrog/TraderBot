#pragma once

#ifndef INDICATORS_HEADER
#define INDICATORS_HEADER

#include <string>
#include <nlohmann/json.hpp>
#include <map>
#include <iostream>

#include "../../request/inc/request.hpp"

using std::string, std::map, std::cout, std::endl;

/**
 * @brief Requests to taapi.io 
 */
namespace Indicators::TAAPI
{
    /**
     * @brief Get the taapi url object
     * 
     * @return std::string 
     */
    std::string get_taapi_url();

    /**
     * @brief Get EMA value
     * 
     * @param key Key of taapi.io
     * @param symbol Symbol (pair)
     * @param interval Interval
     * @param period EMA Period
     * @return double 
     */
    double EMA(
        const std::string &key,
        const std::string &symbol, 
        const std::string &interval, 
        const int &period
    );
}





















#endif