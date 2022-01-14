#pragma once

#ifndef TRADERS_HEADER
#define TRADERS_HEADER

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include "../../trade/inc/trade.hpp"
#include "../../request/inc/request.hpp"
#include "../../exceptions/inc/exceptions.hpp"

using std::string, std::map, std::cout, std::endl;

/**
 * @brief All Traders which trader
 */
namespace Traders::TAAPI
{
    /**
     * @brief Trader decides when open and close order
     */
    class Trader
    {
        private:
            /**
             * @brief Active Trade for Worker
             */
            Trade trade;

            /**
             * @brief The name of the Strategy the Trader trades
             */
            string strategy_name;

            /**
             * @brief The traded pair
             */
            string symbol;

            /**
             * @brief 
             */
            string interval;

            /**
             * @brief Work Trade Status
             */
            bool work = false;

            /**
             * @brief Stake amount to trade (USDT)
             */
            double stake_amount;

            /**
             * @brief Get the current price of cryptocurrency
             * 
             * @return double 
             */
            double get_current_price()
            {
                map<string, string> params;
                string sym;

                std::remove_copy(
                    this->symbol.begin(),
                    this->symbol.end(),
                    std::back_inserter(sym),
                    '/'
                );

                params["symbol"] = sym;

                Request::Simple::JSON_Curl json_curl("https://api.binance.com/api/v3/ticker/price");

                json_curl.construct_request(params);

                nlohmann::json res = json_curl.request();

                if (res.contains("price"))
                {
                    string price = res["price"];
                    return std::stod(price);
                }
                
                throw Exceptions::Panic::Panic_Exception("Something went wront in trader!", 1, 0);
            }

            /**
             * @brief Initialize new Trade
             */
            void initialize_trade()
            {
                this->trade.set_open_time();
                // TODO: Unique ID for each Trade
                this->trade.set_id(1);
                this->trade.set_symbol(
                    this->symbol
                );
                this->trade.set_stake_amount(
                    this->stake_amount
                );
                this->trade.set_open_price(
                    this->get_current_price()
                );
                this->trade.set_interval(
                    this->interval
                );
            }

            /**
             * @brief Clear current Trade to open new one
             */
            void clear_trade()
            {
                // TODO: Clear Trade object
            }

            /**
             * @brief Open new Trade
             */
            void open_trade()
            {
                this->initialize_trade();
                this->work = true;
                // TODO: Open trade in binance
            }

            /**
             * @brief Close current Trade
             */
            void close_trade()
            {
                this->trade.set_close_time(
                    this->get_current_price()
                );
                this->work = false;
                // TODO: Close trade in binance
            }

        public:
            /**
             * @brief Construct a new ema cross trader object
             * 
             * @param symbol Symbol (pair) to trade
             * @param interval Candles interval
             * @param name The name of the Strategy the Trader trades
             * @param stake Stake Amount to open Trade
             */
            Trader(
                const string &symbol,
                const string &interval,
                const string &name,
                double stake
            ) : symbol(symbol), interval(interval),
                strategy_name(name), stake_amount(stake)
            { }

            /**
             * @brief Destroy the Trader object
             */
            ~Trader()
            { }

            /**
             * @brief Get the symbol
             * 
             * @return string 
             */
            string get_symbol() { return this->symbol; }

            /**
             * @brief Get the interval 
             * 
             * @return string 
             */
            string get_interval() { return this->interval; }

            /**
             * @brief Get the strategy name Trader trades
             * 
             * @return string 
             */
            string get_strategy_name() { return this->strategy_name; }

            /**
             * @brief Is Trade working?
             * 
             * @return true 
             * @return false 
             */
            bool is_work() { return this->work; }

            /**
             * @brief Get the stake amount
             * 
             * @return double 
             */
            double get_stake_amount() { return this->stake_amount; }

            /**
             * @brief Get Trader description
             * 
             * @param description Map for description
             */
            void get_trader_description(map<string, string> &description)
            {
                description["symbol"] = this->symbol;
                description["interval"] = this->interval;
                description["strategy"] = this->strategy_name;
                description["is_work"] = this->work ? "true" : "false";
                description["stake_amount"] = std::to_string(this->stake_amount);
            }

            /**
             * @brief Decide what to do with trade
             * 
             * @param buy_signal Buy signal
             * @param sell_signal Sell signal
             */
            void resolve(bool buy_signal, bool sell_signal)
            {
                if (this->work)
                {
                    if (sell_signal) 
                    {
                        this->close_trade();
                        // Do something with Trade
                        cout << "[+] Trade is closed" << endl;
                        this->clear_trade(); // Reset options
                    }
                    return;
                } else {
                    if (buy_signal)
                    {
                        this->open_trade();
                        // Do something with Trade
                        cout << "[+] Trade is opened" << endl;
                    }
                    return;
                }
            }
    };
}

























#endif