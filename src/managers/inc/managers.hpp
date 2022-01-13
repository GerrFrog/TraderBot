#pragma once

#ifndef MANAGER_HEADER
#define MANAGER_HEADER

#include <string>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "../../workers/inc/workers.hpp"
#include "../../trade/inc/trade.hpp"

using std::map, std::string, std::cout, std::endl, std::vector;

// TODO: Parallel execution of Trading

/**
 * @brief All Traders which trade
 */
namespace Managers::Traders
{
    /**
     * @brief EMA Cross Strategy Trader
     */
    class EMA_Cross_Trader
    {
        private:
            /**
             * @brief Active Trade for Worker
             */
            Trade trade;

            /**
             * @brief The traded pair
             */
            string symbol;

            /**
             * @brief 
             */
            string interval;

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
                // TODO: Get current price for symbol (pair)
                return 100;
            }

            /**
             * @brief Initialize new Trade (when buy)
             */
            void initialize_trade()
            {
                this->trade.set_open_time();
                // TODO: Unique ID for each Trade
                this->trade.set_id(1);
                this->trade.set_interval(this->interval);
                this->trade.set_open_price(
                    this->get_current_price()
                );
                this->trade.set_open_time();
                this->trade.set_stake_amount(this->stake_amount);
                this->trade.set_symbol(this->symbol);
            }

            /**
             * @brief Clear the Current Trade and prepare new
             */
            void clear_trade()
            {
                // TODO: Clear the Trade
            }
            
        public:
            /**
             * @brief Construct a new ema trader object
             */
            EMA_Cross_Trader() 
            { }

            /**
             * @brief Construct a new ema cross trader object
             * 
             * @param symbol 
             * @param intr 
             * @param stake 
             */
            EMA_Cross_Trader(
                string &symbol,
                string &intr,
                double stake
            ) : symbol(symbol), interval(intr), stake_amount(stake)
            { }

            /**
             * @brief Destroy the ema trader object
             */
            ~EMA_Cross_Trader() 
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
             * @brief Get the stake amount
             * 
             * @return double 
             */
            double get_stake_amount() { return this->stake_amount; }

            /**
             * @brief Decide what to do with trade
             * 
             * @param buy_signal Buy signal
             * @param sell_signal Sell signal
             */
            void resolve(bool buy_signal, bool sell_signal)
            {
                if (this->trade.is_opened())
                {
                    if (sell_signal) 
                    {
                        this->trade.close_trade(
                            this->get_current_price()
                        );
                        // Do something with Trade
                        cout << "[+] Trade is closed" << endl;
                        this->clear_trade();
                    }
                    return;
                } else {
                    if (buy_signal)
                    {
                        this->initialize_trade();
                        this->trade.open_trade();
                        cout << "[+] Trade is opened" << endl;
                    }
                    return;
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
            vector<Workers::TAAPI::EMA_Cross_Worker> workers;

            /**
             * @brief Vector for EMA Cross Strategy Traders
             */
            vector<Managers::Traders::EMA_Cross_Trader> traders;

            /**
             * @brief Initialize all workers by config
             */
            void initial_workers()
            {
                vector<string> timeframes;

                for (auto& elem : this->config["timeframes"])
                    timeframes.push_back(elem);

                for (string& timeframe : timeframes)
                {
                    Workers::TAAPI::EMA_Cross_Worker worker(
                        this->taapi_key, config["symbol"], 
                        timeframe, config["short_ema_period"],
                        config["long_ema_period"]
                    );
                    this->workers.push_back(worker);
                }
            }

            /**
             * @brief Initialize all traders by config
             */
            void initial_traders()
            {
                vector<string> timeframes;

                for (auto& elem : this->config["timeframes"])
                    timeframes.push_back(elem);

                for (string &timeframe : timeframes)
                {
                    string symbol = this->config["symbol"];
                    Managers::Traders::EMA_Cross_Trader trader(
                        symbol,
                        timeframe,
                        this->config["stake_amount"]
                    );
                    this->traders.push_back(trader);
                }
            }

            /**
             * @brief Start working to worker
             * 
             * @param worker 
             */
            void work(
                Workers::TAAPI::EMA_Cross_Worker &worker,
                Managers::Traders::EMA_Cross_Trader &trader
            ) {
                worker.resolve();
                trader.resolve(
                    worker.get_buy_signal(),
                    worker.get_sell_signal()
                );

                this->describe_worker(worker);
                cout << "Trader interval: " << trader.get_interval() << endl;
                cout << endl;
            }

            void describe_workers()
            {
                for (auto& worker : this->workers)
                {
                    map<string, string> description;
                    worker.describe_worker(description);
                    for (auto& [key, val] : description)
                        cout << key << " : " << val << endl;
                    cout << endl;
                }
            }

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
            EMA_Cross_Employer() { }

            /**
             * @brief Construct a new Trade_Manager object
             * 
             * @param key taapi.io key
             * @param config JSON config of strategies
             */
            EMA_Cross_Employer(
                const string &key,
                nlohmann::json &config
            ) : taapi_key(key), config(config) { }

            /**
             * @brief Destroy the Workers_Manager object
             */
            ~EMA_Cross_Employer() { }

            /**
             * @brief Set the key 
             * 
             * @param key API Key for taapi.io
             */
            void set_key(const string &key) { this->taapi_key = key; }

            /**
             * @brief Set the config 
             * 
             * @param conf 
             */
            void set_config(nlohmann::json &conf) { this->config = conf; }

            /**
             * @brief Start working for all workers
             */
            void run()
            {
                this->initial_workers();
                this->describe_workers();
                this->initial_traders();

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


                // for (Workers::TAAPI::EMA_Cross_Worker &worker : this->workers)
                // {
                //     map<string, string> desc;
                //     worker.describe_worker(desc);
                //     for (auto &[key, val] : desc)
                //         cout << key << " : " << val << endl;
                //     // work(worker);
                // }
            }
    };

    /**
     * @brief Workers Manager. Responsable for all Workers
     */
    // class Workers_Manager
    // {
    //     private:
    //         /**
    //          * @brief taapi.io key
    //          */
    //         string taapi_key;

    //         /**
    //          * @brief Configuration for strategies
    //          */
    //         nlohmann::json config;

    //         /**
    //          * @brief Vector for EMA Cross Strategy Workers
    //          */
    //         vector<Workers::TAAPI::EMA_Cross_Worker> ema_cross_workers;

    //         /**
    //          * @brief Initialize EMA Cross Strategy Workers
    //          */
    //         void initial_ema_cross()
    //         {
    //             vector<string> timeframes;

    //             for (auto& elem : this->config["ema_cross"]["timeframes"])
    //                 timeframes.push_back(elem);

    //             for (string& timeframe : timeframes)
    //             {
    //                 Workers::TAAPI::EMA_Cross_Worker worker(
    //                     this->taapi_key, config["ema_cross"]["symbol"], 
    //                     timeframe, config["ema_cross"]["short_ema_period"],
    //                     config["ema_cross"]["long_ema_period"]
    //                 );
    //                 this->ema_cross_workers.push_back(worker);
    //             }
    //         }

    //     public:
    //         /**
    //          * @brief Construct a new Workers_Manager object
    //          */
    //         Workers_Manager() { }

    //         /**
    //          * @brief Construct a new Trade_Manager object
    //          * 
    //          * @param key taapi.io key
    //          * @param config JSON config of strategies
    //          */
    //         Workers_Manager(
    //             const string &key,
    //             nlohmann::json &config
    //         ) : taapi_key(key), config(config) { }

    //         /**
    //          * @brief Destroy the Workers_Manager object
    //          */
    //         ~Workers_Manager() { }

    //         /**
    //          * @brief Set the key 
    //          * 
    //          * @param key API Key for taapi.io
    //          */
    //         void set_key(const string &key) { this->taapi_key = key; }

    //         /**
    //          * @brief Set the config 
    //          * 
    //          * @param conf 
    //          */
    //         void set_config(nlohmann::json &conf) { this->config = conf; }

    //         /**
    //          * @brief Get the ema workers 
    //          * 
    //          * @return vector<Workers::EMA_Cross_Worker> 
    //          */
    //         vector<Workers::TAAPI::EMA_Cross_Worker> get_ema_workers()
    //         {
    //             return this->ema_cross_workers;
    //         }

    //         void describe_workers()
    //         {
    //             for (Workers::TAAPI::EMA_Cross_Worker& worker : this->ema_cross_workers)
    //             {
    //                 map<string, string> description;
    //                 worker.describe_worker(description);
    //                 for (auto const& [key, val] : description)
    //                     cout << key << " : " << val << endl;
    //                 cout << endl;
    //             }
    //         }

    //         /**
    //          * @brief Initialize all Workers
    //          */
    //         void initialize()
    //         {
    //             cout << "[?] Create EMA Cross Strategy Workers" << endl;
    //             this->initial_ema_cross();
    //             cout << "[+] Built EMA Cross Strategy Workers:" << endl;
    //         }

    //         // void start()
    //         // {
    //         //     for (Workers::EMA_Cross_Worker& worker : this->ema_cross_workers)
    //         //     {
    //         //         map<string, string> description;

    //         //         worker.describe_worker(description);
    //         //         for (auto const& [key, val] : description)
    //         //             std::cout << key << " : " << val << endl;

    //         //         worker.resolve();

    //         //         cout << "Buy signal: " << worker.get_buy_signal() << endl;
    //         //         cout << "Sell signal: " << worker.get_sell_signal() << endl;
    //         //         cout << endl;
    //         //     }
    //         // }
    // };

    // /**
    //  * @brief Trading Manager. Responsable for all Trades
    //  */
    // class Trade_Manager
    // {
    //     private:
    //         /**
    //          * @brief Workers Manager
    //          */
    //         Managers::Workers_Manager worker;

    //         /**
    //          * @brief Initialize Worker Manager
    //          */
    //         void initialize_worker(const string& key, nlohmann::json &config)
    //         {
    //             this->worker.set_key(key);
    //             this->worker.set_config(config);

    //             this->worker.initialize();
    //         }

    //     public:
    //         /**
    //          * @brief Construct a new Trade_Manager object
    //          */
    //         Trade_Manager() { }

    //         /**
    //          * @brief Construct a new Trade_Manager object
    //          * 
    //          * @param key API Key for taapi.io
    //          * @param config JSON Configuration for strategies
    //          */
    //         Trade_Manager(const string& key, nlohmann::json &config)
    //         {
    //             this->initialize(key, config);
    //         }

    //         /**
    //          * @brief Destroy the Trade_Manager object
    //          */
    //         ~Trade_Manager() { }

    //         /**
    //          * @brief Initialize Trade Manager
    //          * 
    //          * @param key API Key for taapi.io
    //          * @param config JSON Configuration for strategies
    //          */
    //         void initialize(const string& key, nlohmann::json &config)
    //         {
    //             this->initialize_worker(key, config);
    //         }
    // };
}



















#endif