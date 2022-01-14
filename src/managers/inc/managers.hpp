#pragma once

#ifndef MANAGER_HEADER
#define MANAGER_HEADER

#include <string>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "../../workers/inc/workers.hpp"
#include "../../traders/inc/traders.hpp"

using std::map, std::string, std::cout, std::endl, std::vector;


/**
 * @brief All Traders which trade
 */
namespace Managers::Analyst
{
    /**
     * @brief EMA Cross Strategy Analyst
     */
    class EMA_Cross_Analyst
    {
        private:
            /**
             * @brief Vector for EMA Cross Strategy Traders
             */
            vector<Traders::TAAPI::EMA_Cross_Trader> traders;

            /**
             * @brief Describe Traders
             */
            void describe_traders()
            {
                for (Traders::TAAPI::EMA_Cross_Trader &trader : this->traders)
                {
                    map<string, string> descr;
                    trader.describe_trader(descr);
                    for (auto &[key, val] : descr)
                        cout << key << " : " << val << endl;
                }
            }

            /**
             * @brief Describe curtain Trader
             * 
             * @param trader Trader
             */
            void describe_trader(Traders::TAAPI::EMA_Cross_Trader &trader)
            {
                map<string, string> descr;
                trader.describe_trader(descr);
                for (auto &[key, val] : descr)
                    cout << key << " : " << val << endl;
            }
            
        public:
            /**
             * @brief Construct a new ema trader object
             */
            EMA_Cross_Analyst() 
            { }

            /**
             * @brief Destroy the ema trader object
             */
            ~EMA_Cross_Analyst() 
            { }

            /**
             * @brief Initialize all traders by config
             * 
             * @param config JSON Config for Trader
             */
            void initial_traders(nlohmann::json &config)
            {
                vector<string> timeframes;

                for (auto& elem : config["timeframes"])
                    timeframes.push_back(elem);

                for (string &timeframe : timeframes)
                {
                    string symbol = config["symbol"];
                    Traders::TAAPI::EMA_Cross_Trader trader(
                        symbol,
                        timeframe,
                        config["trader"]["stake_amount"]
                    );
                    this->traders.push_back(trader);
                }
            }
    };
}

/**
 * @brief All Managers for working
 */
namespace Managers::Employers
{
    /**
     * @brief EMA Cross Strategy Employer
     */
    class EMA_Cross_Employer
    {
        private:
            /**
             * @brief Vector for EMA Cross Strategy Workers
             */
            vector<Workers::TAAPI::EMA_Cross_Worker> workers;

            /**
             * @brief Describe Workers
             */
            void describe_workers()
            {
                for (auto& worker : this->workers)
                {
                    map<string, string> description;
                    worker.describe_worker(description);
                    for (auto& [key, val] : description)
                        cout << key << " : " << val << endl;
                }
            }

            /**
             * @brief Describe curtain Worker
             * @param worker Worker
             */
            void describe_worker(Workers::TAAPI::EMA_Cross_Worker &worker)
            {
                map<string, string> description;
                worker.describe_worker(description);
                for (auto& [key, val] : description)
                    cout << key << " : " << val << endl;
            }

        public:
            /**
             * @brief Construct a new Workers_Manager object
             */
            EMA_Cross_Employer() 
            { }

            /**
             * @brief Destroy the Workers_Manager object
             */
            ~EMA_Cross_Employer() 
            { }

            /**
             * @brief Initialize all workers by config
             * 
             * @param config JSON Config for Worker
             * @param taapi_key API Key for taapi.io
             */
            void initial_workers(nlohmann::json &config, string &taapi_key)
            {
                vector<string> timeframes;

                for (auto& elem : config["timeframes"])
                    timeframes.push_back(elem);

                for (string& timeframe : timeframes)
                {
                    Workers::TAAPI::EMA_Cross_Worker worker(
                        taapi_key, config["symbol"], 
                        timeframe, config["worker"]["short_ema_period"],
                        config["worker"]["long_ema_period"]
                    );
                    this->workers.push_back(worker);
                }
            }
    };
}



















#endif