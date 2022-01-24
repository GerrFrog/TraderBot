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

        // TODO: One work function (not for certain strategy)
        /**
         * @brief Start working for pair Worker-Trader
         * 
         * @param worker Worker
         * @param trader Trader
         */
        void ema_work(
            Workers::Worker<Strategies::EMA_Cross> &worker,
            Traders::Trader &trader
        ) {
            worker.resolve();
            trader.resolve(
                worker.get_buy_signal(),
                worker.get_sell_signal()
            );

            map<string, string> worker_desc;
            map<string, string> trader_desc;

            worker.get_worker_description(worker_desc);
            trader.get_description(trader_desc);

            cout << "WORKER" << endl;
            for (auto &[key, val] : worker_desc)
                cout << key << " : " << val << endl;;
            cout << endl;
            cout << "TRADER" << endl;
            for (auto &[key, val] : trader_desc)
                cout << key << " : " << val << endl;;
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
            const string &taapi_key
        ) 
        { 
            this->ema_analyst.initial_traders(config["ema_cross"]);
            this->ema_employer.initial_workers(config["ema_cross"], taapi_key);
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

            vector<Traders::Trader> traders = this->ema_analyst.get_traders();
            vector<Workers::Worker<Strategies::EMA_Cross>> workers = this->ema_employer.get_workers();

            cout << "[+] Start working" << endl;

            for (Traders::Trader &trader : traders)
            {
                cout << "[+] Got new Trader:" << endl;
                this->ema_analyst.describe_trader(trader);
                cout << endl;
            } cout << endl;

            for (Workers::Worker<Strategies::EMA_Cross> &worker : workers)
            {
                cout << "[+] Got new Worker:" << endl;
                this->ema_employer.describe_worker(worker);
                cout << endl;
            } cout << endl;

            while (true)
            {
                for (int i = 0; i < workers.size(); i++)
                    ema_work(workers[i], traders[i]);
            }
        }
};























#endif