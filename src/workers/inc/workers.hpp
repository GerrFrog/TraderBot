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
    template <class Strategy>
    class Worker
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
             * @brief Params for strategy
             */
            nlohmann::json strategy_params;

            /**
             * @brief EMA Cross Strategy object
             */
            Strategy strategy;

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
            Worker(
                const string &taapi_key, const string &trade_symbol,
                const string &timeframe, const string &name,
                nlohmann::json &params
            ) : taapi_key(taapi_key), symbol(trade_symbol),
                interval(timeframe), strategy_name(name),
                strategy_params(params)
            { }

            /**
             * @brief Destroy the ema cross 15m object
             */
            ~Worker() { };

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
                description["symbol"] = this->symbol;
                description["interval"] = this->interval;
                description["strategy"] = this->strategy_name;
            }

            /**
             * @brief Resolve the strategy in worker
             */
            void resolve()
            {
                map<string, double> params;

                // TODO: Create instance from config. Without if-else
                for (auto& [key, value] : this->strategy_params.items()) {
                    if (value["indicator"] == "EMA")
                    {
                        if (value["type"] == "TAAPI")
                        {
                        again:
                            try {
                                params[key] = Indicators::TAAPI::EMA(
                                    this->taapi_key, this->symbol,
                                    this->interval, value["indicator_params"]
                                );
                            } catch (Exceptions::TAAPI::Rate_Limit& exp) {
                                cout << exp.what() << endl;
                                // TODO: Pass delay from config
                                std::this_thread::sleep_for(
                                    std::chrono::seconds(
                                        15
                                    )
                                );
                                goto again;
                            }
                        }
                    } // else if () // Other indicator
                }
                this->strategy.resolve(
                    params,
                    this->signals
                );
            }
    };
}


















#endif