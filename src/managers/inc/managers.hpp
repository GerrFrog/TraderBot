#pragma once

#ifndef MANAGER_HEADER
#define MANAGER_HEADER

#include <string>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "../../workers/inc/workers.hpp"

using std::map, std::string, std::cout, std::endl, std::vector;

// TODO: Parallel execution of Trading

/**
 * @brief All Managers for working
 */
namespace Managers
{
    /**
     * @brief Workers Manager. Responsable for all Workers
     */
    class Workers_Manager
    {
        private:
            /**
             * @brief taapi.io key
             */
            string taapi_key;

            /**
             * @brief Configuration for strategies
             */
            nlohmann::json config;

            /**
             * @brief Vector for EMA Cross Strategy Workers
             */
            vector<Workers::EMA_Cross_Worker> ema_cross_workers;

            /**
             * @brief Initialize EMA Cross Strategy Workers
             */
            void initial_ema_cross()
            {
                vector<string> timeframes;
                vector<string> symbols;

                for (auto& elem : this->config["ema_cross"]["timeframes"])
                    timeframes.push_back(elem);

                for (auto& elem : this->config["ema_cross"]["symbols"])
                    symbols.push_back(elem);

                for (string& timeframe : timeframes)
                {
                    Workers::EMA_Cross_Worker worker(
                        this->taapi_key, config["ema_cross"]["symbol"], 
                        timeframe, config["ema_cross"]["short_ema_period"],
                        config["ema_cross"]["long_ema_period"]
                    );
                    this->ema_cross_workers.push_back(worker);
                }
            }

        public:
            /**
             * @brief Construct a new Trade_Manager object
             * 
             * @param key taapi.io key
             */
            Workers_Manager(
                const string &key,
                nlohmann::json &config
            ) : taapi_key(key), config(config) { }

            /**
             * @brief Destroy the Workers_Manager object
             */
            ~Workers_Manager() { }

            void describe_workers()
            {
                for (Workers::EMA_Cross_Worker& worker : this->ema_cross_workers)
                {
                    map<string, string> description;
                    worker.describe_worker(description);
                    for (auto const& [key, val] : description)
                        cout << key << " : " << val << endl;
                    cout << endl;
                }
            }

            /**
             * @brief Initialize all Workers
             */
            void initialize_workers()
            {
                cout << "[?] Create EMA Cross Strategy Workers" << endl;
                this->initial_ema_cross();
                cout << "[+] Built EMA Cross Strategy Workers:" << endl;
            }

            void start()
            {
                for (Workers::EMA_Cross_Worker& worker : this->ema_cross_workers)
                {
                    map<string, string> description;

                    worker.describe_worker(description);
                    for (auto const& [key, val] : description)
                        std::cout << key << " : " << val << endl;

                    worker.resolve();

                    cout << "Buy signal: " << worker.get_buy_signal() << endl;
                    cout << "Sell signal: " << worker.get_sell_signal() << endl;
                    cout << endl;
                }
            }
    };

    /**
     * @brief Trading Manager. Responsable for all Trades
     */
    class Trade_Manager
    {
        private:

        public:
            /**
             * @brief Construct a new Trade_Manager object
             */
            Trade_Manager() { }
    };
}






















#endif