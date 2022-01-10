#pragma once

#ifndef TRADE_HEADER
#define TRADE_HEADER

#include <chrono>
#include <ctime> 

/**
 * @brief Contains all information about trade
 */
class Trade
{
    private:
        /**
         * @brief Open Time of Trade
         */
        std::chrono::_V2::system_clock::time_point open_time;

        /**
         * @brief Close Time of Trade
         */
        std::chrono::_V2::system_clock::time_point close_time;

    public:
        /**
         * @brief Construct a new Trade object
         */
        Trade() 
        { 
            this->open_time = std::chrono::system_clock::now();
        }

};





















#endif