#pragma once

#ifndef WORKERS_HEADER
#define WORKERS_HEADER

#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>

#include "../../strategies/inc/strategies.hpp"
#include "../../indicators/inc/indicators.hpp"

using std::string, std::cout, std::endl, std::map;

/**
 * @brief All Workers which create Trader
 */
namespace Workers
{
    /**
     * @brief EMA Cross Strategy Worker
     */
    class EMA_Cross_Worker
    {
        private:
            /**
             * @brief API Key for taapi.io
             */
            string taapi_key;

            /**
             * @brief Symbol (pair) for trading
             */
            string symbol;

            /**
             * @brief Timeframe interval for trading
             */
            string interval;

            /**
             * @brief Data container for resolved signals
             */
            map<string, bool> signals{
                {"buy", false}, {"sell", false}
            };

            /**
             * @brief Period for Short EMA
             */
            int short_period;

            /**
             * @brief Period for Long EMA
             */
            int long_period;

            /**
             * @brief EMA Cross Strategy object
             */
            Strategies::EMA_Cross ema_strategy;

        public:
            /**
             * @brief Construct a new ema cross 15m object
             */
            EMA_Cross_Worker() { };

            /**
             * @brief Construct a new ema cross 15m object
             * 
             * @param taapi_key API Key for taapi.io
             * @param trade_symbol Symbol (pair) for trading
             * @param timeframe Timeframe interval for trading
             * @param short_ema_period Period for Short EMA
             * @param long_ema_period Period for Long EMA
             */
            EMA_Cross_Worker(
                const string &taapi_key, const string &trade_symbol,
                const string &timeframe, int short_ema_period,
                int long_ema_period
            ) : taapi_key(taapi_key), symbol(trade_symbol),
                interval(timeframe), short_period(short_ema_period),
                long_period(long_ema_period) { }

            /**
             * @brief Destroy the ema cross 15m object
             */
            ~EMA_Cross_Worker() { };

            /**
             * @brief Set the taapi key 
             * 
             * @param key API Key for taapi.io
             */
            void set_taapi_key(const string &key) { this->taapi_key = key; }

            /**
             * @brief Set the short period for EMA
             * 
             * @param short_ema_period 
             */
            void set_short_period(int short_ema_period) { this->short_period = short_ema_period; }

            /**
             * @brief Set the long period for EMA
             * 
             * @param long_ema_period 
             */
            void set_long_period(int long_ema_period) { this->long_period = long_ema_period; }

            /**
             * @brief Set the symbol for trading
             * 
             * @param trade_symbol Symbol (pair) for trading
             */
            void set_symbol(const string &trade_symbol) { this->symbol = trade_symbol; }

            /**
             * @brief Set the interval for trading
             * 
             * @param timeframe 
             */
            void set_interval(const string &timeframe) { this->interval = timeframe; }

            /**
             * @brief Get the buy signal 
             * 
             * @return true 
             * @return false 
             */
            bool get_buy_signal() { return signals["buy"]; }

            /**
             * @brief Get the sell signal 
             * 
             * @return true 
             * @return false 
             */
            bool get_sell_signal() { return signals["sell"]; }

            /**
             * @brief Describe parameters of the Worker
             * 
             * @param description Map for description
             */
            void describe_worker(map<string, string> &description)
            {
                description["short"] = std::to_string(this->short_period);
                description["long"] = std::to_string(this->long_period);
                description["symbol"] = this->symbol;
                description["interval"] = this->interval;
            }

            void resolve()
            {
                std::this_thread::sleep_for(std::chrono::seconds(15));

                double short_value = Indicators::TAAPI::EMA(
                    this->taapi_key, this->symbol,
                    this->interval, this->short_period
                );

                // TODO: Remake without delay
                std::this_thread::sleep_for(std::chrono::seconds(15));

                double long_value = Indicators::TAAPI::EMA(
                    this->taapi_key, this->symbol,
                    this->interval, this->long_period
                );

                // TODO: Remake with throw error
                if (short_value == 0.0 || long_value == 0.0)
                {
                    cout << "Limit of request to https://api.taapi.io/" << endl;
                    return;
                }

                this->ema_strategy.resolve(
                    short_value, long_value,
                    this->signals
                );
            }
    };
}


















#endif