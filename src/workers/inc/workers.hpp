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
namespace Workers::TAAPI
{
    /**
     * @brief EMA Cross Strategy Worker
     */
    class EMA_Cross_Worker
    {
        private:
            /**
             * @brief Strategy name worker works
             */
            string strategy_name;

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
             * @brief The delay between requests
             */
            int delay;

            /**
             * @brief EMA Cross Strategy object
             */
            Strategies::EMA_Cross ema_strategy;

        public:
            /**
             * @brief Construct a new ema cross 15m object
             * 
             * @param taapi_key API Key for taapi.io
             * @param trade_symbol Symbol (pair) for trading
             * @param timeframe Timeframe interval for trading
             * @param name Strategy name Worker works
             * @param short_ema_period Period for Short EMA
             * @param long_ema_period Period for Long EMA
             * @param delay Delay between request
             */
            EMA_Cross_Worker(
                const string &taapi_key, const string &trade_symbol,
                const string &timeframe, const string &name,
                int short_ema_period, int long_ema_period,
                int delay
            ) : taapi_key(taapi_key), symbol(trade_symbol),
                interval(timeframe), strategy_name(name),
                short_period(short_ema_period), long_period(long_ema_period),
                delay(delay)
            { }

            /**
             * @brief Destroy the ema cross 15m object
             */
            ~EMA_Cross_Worker() { };

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
             * @brief Get Worker description
             * 
             * @param description Map for description
             */
            void get_worker_description(map<string, string> &description)
            {
                description["short"] = std::to_string(this->short_period);
                description["long"] = std::to_string(this->long_period);
                description["symbol"] = this->symbol;
                description["interval"] = this->interval;
                description["strategy"] = this->strategy_name;
            }

            /**
             * @brief Resolve the strategy in worker
             */
            void resolve()
            {
                double short_value, long_value;
                // std::this_thread::sleep_for(std::chrono::seconds(15));

            again_short:
                try {
                    short_value = Indicators::TAAPI::EMA(
                        this->taapi_key, this->symbol,
                        this->interval, this->short_period
                    );
                } catch (Exceptions::TAAPI::Rate_Limit& exp) {
                    cout << exp.what() << endl;
                    std::this_thread::sleep_for(
                        std::chrono::seconds(
                            this->delay
                        )
                    );
                    goto again_short;
                }

            again_long:
                try {
                    long_value = Indicators::TAAPI::EMA(
                        this->taapi_key, this->symbol,
                        this->interval, this->long_period
                    );
                } catch (Exceptions::TAAPI::Rate_Limit& exp) {
                    cout << exp.what() << endl;
                    std::this_thread::sleep_for(
                        std::chrono::seconds(
                            this->delay
                        )
                    );
                    goto again_long;
                }

                this->ema_strategy.resolve(
                    short_value, long_value,
                    this->signals
                );
            }
    };
}


















#endif