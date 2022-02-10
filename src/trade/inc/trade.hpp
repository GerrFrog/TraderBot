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
    // TODO: Get open time
    // TODO: Get close time
    private:
        /**
         * @brief Unique Trade id
         */
        unsigned int id;

        /**
         * @brief Activity state of trade
         */
        bool active = false;

        /**
         * @brief Completed status of trade
         */
        bool completed = false;

        /**
         * @brief The traded symbol
         */
        string symbol;

        /**
         * @brief Candles Interval
         */
        string interval;

        /**
         * @brief Trade position (long/short)
         */
        string position;

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
         * @brief Stake for open "long" trade (buy cryptocurrency with this stake)
         */
        double stake_amount;

        /**
         * @brief Stake for open "short" trade
         */
        double symbol_amount;

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
        Trade() = default;

        /**
         * @brief Construct a new Trade object
         * 
         * @param stake_amount Stake amount to open a "long" trade (USDT)
         * @param symbol_amount Stake amount of symbol to open a "short" trade
         * @param open_price Open Price when Trade was created
         * @param symbol The traded pair
         * @param interval Candles Interval
         */
        Trade(
            double stake_amount, 
            double symbol_amount,
            double open_price, 
            const string &interval,
            const string &symbol,
            const string &position
        ) : stake_amount(stake_amount), 
            symbol_amount(symbol_amount),
            open_price(open_price), symbol(symbol), 
            interval(interval), position(position)
        { }

        /**
         * @brief Destroy the Trade object
         */
        ~Trade() = default;

        /**
         * @brief Overload of "==" operator
         * 
         * @param trade Trade object
         * @return true 
         * @return false 
         */
        bool operator ==(const Trade& trade)
        {
            if (
                trade.open_price == this->open_price &&
                trade.position == this->position &&
                trade.open_time == this->open_time &&
                trade.stake_amount == this->stake_amount &&
                trade.symbol_amount == this->symbol_amount &&
                trade.symbol == this->symbol 
            ) return true;
            return false;
        }

        /**
         * @brief Get the symbol of trade
         * 
         * @return string 
         */
        string get_symbol() { return this->symbol; }

        /**
         * @brief Get the position 
         * 
         * @return string 
         */
        string get_position() { return this->position; }

        /**
         * @brief Get the interval 
         * 
         * @return string 
         */
        string get_interval() { return this->interval; }

        /**
         * @brief Get the live time 
         * 
         * @return double 
         */
        double get_live_time() { return this->live_time; }

        /**
         * @brief Get the stake amount 
         * 
         * @return double 
         */
        double get_stake_amount() { return this->stake_amount; }

        /**
         * @brief Get the symbol amount
         * 
         * @return double 
         */
        double get_symbol_amount() { return this->symbol_amount; }

        /**
         * @brief Get the open price 
         * 
         * @return double 
         */
        double get_open_price() { return this->open_price; }

        /**
         * @brief Get the close price
         * 
         * @return double 
         */
        double get_close_price() { return this->close_price; }

        /**
         * @brief Get the abs profit 
         * 
         * @return double 
         */
        double get_abs_profit() { return this->profit; }

        /**
         * @brief Get the per profit 
         * 
         * @return double 
         */
        double get_per_profit() { return this->per_profit; }

        /**
         * @brief Set the Trade id
         * 
         * @param trade_id New Trade id
         */
        void set_id(unsigned int trade_id) { this->id = trade_id; }

        /**
         * @brief Set the position 
         * 
         * @param pos Position (long/short)
         */
        void set_position(const string &pos) { this->position = pos; }

        /**
         * @brief Set the stake amount 
         * 
         * @param amount Stake amount
         */
        void set_stake_amount(double amount) { this->stake_amount = amount; }

        /**
         * @brief Set the symbol amount
         * 
         * @param amount 
         */
        void set_symbol_amount(double amount) { this->symbol_amount = amount; }

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
        void set_open_time(
            unsigned int id,
            const string& symbol,
            const string& position,
            const string& interval,
            double stake_amount,
            double symbol_amount,
            double open_p
        ) 
        { 
            this->active = true;
            this->completed = false;

            this->open_time = std::chrono::system_clock::now(); 

            this->id = id;
            this->symbol = symbol,
            this->position = position;
            this->interval = interval;
            this->stake_amount = stake_amount;
            this->symbol_amount = symbol_amount;
            this->open_price = open_p;
        }

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
         * @param short_goal Goal of short trade (USDT or symbol)
         */
        void set_close_time(
            double close_p,
            const string& short_goal = ""
        ) 
        { 
            this->active = false;
            this->completed = true;

            this->close_time = std::chrono::system_clock::now(); 
            this->close_price = close_p;
            this->live_time = (close_time - open_time).count();

            double percentage = close_p / this->open_price * 100;
            double total_monets;

            if (this->position == "long")
            {
                this->per_profit = percentage - 100;
                this->profit = this->stake_amount * percentage / 100 - this->stake_amount;
            } else if (this->position == "short") {
                if (short_goal == "usdt") 
                {
                    this->per_profit = 100 - percentage;
                    this->profit = this->symbol_amount * this->open_price -
                                this->symbol_amount * close_p;
                } else if (short_goal == "symbol") {
                    total_monets = (this->symbol_amount * this->open_price) /
                        close_p;
                    this->profit = total_monets - this->symbol_amount;
                    this->per_profit = (total_monets / this->symbol_amount - 1) * 100;
                }
            }
        }

        /**
         * @brief Is trade active?
         * 
         * @return true 
         * @return false 
         */
        bool is_active() { return this->active; }

        /**
         * @brief Is trade completed?
         * 
         * @return true 
         * @return false 
         */
        bool is_completed() { return this->completed; }

        /**
         * @brief Describe the trade
         */
        void describe_trade()
        {
            cout << "ID: " << this->id << endl
                 << "Symbol: " << this->symbol << endl
                 << "Interval: " << this->interval << endl
                 << "Position: " << this->position << endl
                 << "Open price: " << this->open_price << endl
                 << "Stake amount: " << this->stake_amount << endl
                 << "Symbol amount: " << this->symbol_amount << endl
            ;
        }
};






















#endif