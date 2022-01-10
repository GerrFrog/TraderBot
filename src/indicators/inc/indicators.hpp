#pragma once

#ifndef INDICATORS_HEADER
#define INDICATORS_HEADER

#include <string>
#include <nlohmann/json.hpp>
#include <map>
#include <iostream>

#include "../../request/inc/request.hpp"

/**
 * @brief Requests to taapi.io 
 */
namespace Indicators::TAAPI
{
    /**
     * @brief URL of taapi.io
     */
    // std::string taapi_url = "https://api.taapi.io/";

    /**
     * @brief Get EMA value
     * 
     * @param key Key of taapi.io
     * @param symbol Symbol (pair)
     * @param interval Interval
     * @param period EMA Period
     * @param backtrack Candles from the data set and calculates the ema value X amount of candles back
     * @param backtracks The EMA value calculated on every candle for the past X candles
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