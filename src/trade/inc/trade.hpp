#pragma once

#ifndef TRADE_HEADER
#define TRADE_HEADER

#include <chrono>
#include <ctime> 
#include <map>
#include <string>
#include <iostream>

using std::map, std::cout, std::endl, std::string;

/**
 * @brief Contains all information about trade
 */
class Trade
{
    private:
        /**
         * @brief Unique Trade id
         */
        unsigned int id;

        /**
         * @brief The traded symbol
         */
        string symbol;

        /**
         * @brief Candles Interval
         */
        string interval;

        /**
         * @brief Open Time of Trade
         */
        std::chrono::_V2::system_clock::time_point open_time;

        /**
         * @brief Close Time of Trade
         */
        std::chrono::_V2::system_clock::time_point close_time;

        /**
         * @brief The Live time of trade (from buy to sell time)
         */
        double live_time;

        /**
         * @brief Stake for open trade (buy cryptocurrency with this stake)
         */
        double stake_amount;

        /**
         * @brief The price of cryptocurrency when trade was created
         */
        double open_price;

        /**
         * @brief The price of cryptocurrency when trade was closed
         */
        double close_price;

        /**
         * @brief Profit in USD
         */
        double profit;

        /**
         * @brief Profit in percentage
         */
        double per_profit;

    public:
        /**
         * @brief Construct a new Trade object
         */
        Trade()
        { } 

        /**
         * @brief Construct a new Trade object
         * 
         * @param stake_amount Stake amount to open a trade (USDT)
         * @param open_price Open Price when Trade was created
         * @param symbol The traded pair
         * @param interval Candles Interval
         */
        Trade(
            double stake_amount, 
            double open_price, 
            string &interval,
            string &symbol
        ) : stake_amount(stake_amount), open_price(open_price),
            symbol(symbol), interval(interval)
        { }

        /**
         * @brief Destroy the Trade object
         */
        ~Trade() 
        { }

        /**
         * @brief Set the Trade id
         * 
         * @param trade_id New Trade id
         */
        void set_id(unsigned int trade_id) { this->id = trade_id; }

        /**
         * @brief Set the stake amount 
         * 
         * @param stake Stake amount
         */
        void set_stake_amount(double stake) { this->stake_amount = stake; }

        /**
         * @brief Set the open price 
         * 
         * @param open Open price
         */
        void set_open_price(double open) { this->open_price = open; }

        /**
         * @brief Set the symbol 
         * 
         * @param sym The traded pair
         */
        void set_symbol(string &sym) { this->symbol = sym; }

        /**
         * @brief Set the open time 
         */
        void set_open_time() { this->open_time = std::chrono::system_clock::now(); }

        /**
         * @brief Set the interval 
         * 
         * @param intr Interval
         */
        void set_interval(string &intr) { this->interval = intr; }

        /**
         * @brief Set the close time. Behave as exit function for Trade
         * 
         * @param close_price Close price
         */
        void set_close_time(double close_p) 
        { 
            this->close_time = std::chrono::system_clock::now(); 
            this->close_price = close_p;
            this->live_time = (close_time - open_time).count();

            double percentage = close_p / this->open_price * 100;
            this->per_profit = percentage - 100;
            this->profit = this->stake_amount * percentage / 100 - this->stake_amount;
        }
};





















#endif