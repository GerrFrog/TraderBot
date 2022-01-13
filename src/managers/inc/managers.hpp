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
namespace Managers::Employers
{
    class EMA_Cross
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
             * @brief Start working to worker
             * 
             * @param worker 
             */
            void work(Workers::TAAPI::EMA_Cross_Worker &worker)
            {
                worker.resolve();

                this->describe_worker(worker);
                cout << "Buy signal: " << worker.get_buy_signal() << endl;
                cout << "Sell signal: " << worker.get_sell_signal() << endl;
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
            EMA_Cross() { }

            /**
             * @brief Construct a new Trade_Manager object
             * 
             * @param key taapi.io key
             * @param config JSON config of strategies
             */
            EMA_Cross(
                const string &key,
                nlohmann::json &config
            ) : taapi_key(key), config(config) { }

            /**
             * @brief Destroy the Workers_Manager object
             */
            ~EMA_Cross() { }

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

                for (Workers::TAAPI::EMA_Cross_Worker &worker : this->workers)
                    work(worker);
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