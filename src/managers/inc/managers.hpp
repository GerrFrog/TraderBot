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
            vector<Traders::Trader> traders;
            
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
             * @brief Describe Traders
             */
            void describe_traders()
            {
                for (Traders::Trader &trader : this->traders)
                {
                    map<string, string> descr;
                    trader.get_description(descr);
                    for (auto &[key, val] : descr)
                        cout << key << " : " << val << endl;
                }
            }

            /**
             * @brief Describe curtain Trader
             * 
             * @param trader Trader
             */
            void describe_trader(Traders::Trader &trader)
            {
                map<string, string> descr;
                trader.get_description(descr);
                for (auto &[key, val] : descr)
                    cout << key << " : " << val << endl;
            }

            /**
             * @brief Get the array with Traders
             * 
             * @return vector<Traders::TAAPI::EMA_Cross_Trader>
             */
            vector<Traders::Trader> get_traders()
            {
                return this->traders;
            }

            /**
             * @brief Initialize all traders by config
             * 
             * @param config JSON Config for Trader
             */
            void initial_traders(nlohmann::json &config)
            {
                for (auto& [key, val] : config.items())
                {
                    vector<string> timeframes;

                    for (auto& elem : val["timeframes"])
                        timeframes.push_back(elem);

                    for (string &timeframe : timeframes)
                    {
                        Traders::Trader trader(
                            val["symbol"],
                            timeframe,
                            val["name"],
                            val["trader_params"]["stake_amount"]
                        );
                        this->traders.push_back(trader);
                    }
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
            vector<Workers::Worker<Strategies::EMA_Cross>> workers;

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
             * @brief Describe Workers
             */
            void describe_workers()
            {
                for (auto& worker : this->workers)
                {
                    map<string, string> description;
                    worker.get_worker_description(description);
                    for (auto& [key, val] : description)
                        cout << key << " : " << val << endl;
                }
            }

            /**
             * @brief Describe curtain Worker
             * @param worker Worker
             */
            void describe_worker(Workers::Worker<Strategies::EMA_Cross> &worker)
            {
                map<string, string> description;
                worker.get_worker_description(description);
                for (auto& [key, val] : description)
                    cout << key << " : " << val << endl;
            }

            /**
             * @brief Get the array with Workers
             * 
             * @return vector<Workers::TAAPI::EMA_Cross_Worker> 
             */
            vector<Workers::Worker<Strategies::EMA_Cross>> get_workers()
            {
                return this->workers; 
            }

            /**
             * @brief Initialize all workers by config
             * 
             * @param config JSON Config for Worker
             * @param taapi_key API Key for taapi.io
             */
            void initial_workers(nlohmann::json &config, const string &taapi_key)
            {
                for (auto& [key, val] : config.items())
                {
                    vector<string> timeframes;

                    for (auto& elem : val["timeframes"])
                        timeframes.push_back(elem);

                    for (string& timeframe : timeframes)
                    {
                        Workers::Worker<Strategies::EMA_Cross> worker(
                            taapi_key, val["symbol"], 
                            timeframe, val["name"],
                            val["strategy_params"]
                        );
                        this->workers.push_back(worker);
                    }
                }
            }
    };
}



















#endif