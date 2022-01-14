#pragma once

#ifndef DIRECTOR_HEADER
#define DIRECTOR_HEADER

#include "../../managers/inc/managers.hpp"

// TODO: Parallel execution of Trading

/**
 * @brief Rule all Managers
 */
class Director
{
    private:
        /**
         * @brief EMA Cross Strategy Traders Manager
         */
        Managers::Analyst::EMA_Cross_Analyst ema_analyst;

        /**
         * @brief EMA Cross Strategy Workers Manager
         */
        Managers::Employers::EMA_Cross_Employer ema_employer;

        /**
         * @brief Start working for pair Worker-Trader
         * 
         * @param worker Worker
         * @param trader Trader
         */
        void work(
            Workers::TAAPI::EMA_Cross_Worker &worker,
            Traders::TAAPI::EMA_Cross_Trader &trader
        ) {
            worker.resolve();
            trader.resolve(
                worker.get_buy_signal(),
                worker.get_sell_signal()
            );

            map<string, string> worker_desc;
            map<string, string> trader_desc;

            worker.describe_worker(worker_desc);
            trader.describe_trader(trader_desc);

            cout << "WORKER" << endl;
            for (auto &[key, val] : worker_desc)
                cout << key << " : " << val;
            cout << "TRADER" << endl;
            for (auto &[key, val] : trader_desc)
                cout << key << " : " << val;
            cout << endl;
        }

    public:
        /**
         * @brief Construct a new Director object
         * 
         * @param config JSON Config (config["strategies"]["strategy"])
         * @param taapi_key API Key for taapi.io
         */
        Director(
            nlohmann::json &config,
            string &taapi_key
        ) 
        { 
            this->ema_analyst.initial_traders(config);
            this->ema_employer.initial_workers(config, taapi_key);
        }

        /**
         * @brief Destroy the Director object
         */
        ~Director()
        { }

        /**
         * @brief Start working for all
         */
        void run()
        {
            cout << "Started!" << endl;
            while (true)
            {
                for (int i = 0; i < this->workers.size(); i++)
                {
                    work(
                        this->workers[i],
                        this->traders[i]
                    );
                }
            }
        }
};























#endif