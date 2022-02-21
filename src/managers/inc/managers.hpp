#pragma once

#ifndef MANAGER_HEADER
#define MANAGER_HEADER

#include <iostream> // std::cout
#include <thread> // std::thread, std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
#include <future>
#include <queue>

#include "../../workers/inc/workers.hpp"

using std::map; 
using std::string; 
using std::cout; 
using std::endl; 
using std::vector;

/**
 * @brief All Managers 
 */
namespace Managers
{
    /**
     * @brief Manager for Workers (who trades)
     */
    class Trade_Manager
    {
        private:
            /**
             * @brief JSON Configuration of Worker
             */
            nlohmann::json configuration;

            /**
             * @brief All Candle Workers with EMA Cross strategy
             */
            vector<Workers::Worker<Strategies::EMA_Cross, Candles::Candle>> w_ema_cross_cd;

            /**
             * @brief All Candle Workers with Normalized MACD Cross strategy
             */
            vector<Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Candle>> w_n_macd_cross_cd;

            /**
             * @brief All Candle Workers with RSXC ADX strategy
             */
            vector<Workers::Worker<Strategies::RSXC_ADX, Candles::Candle>> w_rsxc_adx_cd;

            /**
             * @brief All Heikin Ashi Workers with EMA Cross strategy
             */
            vector<Workers::Worker<Strategies::EMA_Cross, Candles::Heikin_Ashi>> w_ema_cross_ha;

            /**
             * @brief All Heikin Ashi Workers with Normalized MACD Cross strategy
             */
            vector<Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Heikin_Ashi>> w_n_macd_cross_ha;

            /**
             * @brief All Heikin Ashi Workers with RSXC ADX strategy
             */
            vector<Workers::Worker<Strategies::RSXC_ADX, Candles::Heikin_Ashi>> w_rsxc_adx_ha;

        public:
            /**
             * @brief Destroy the Manager object
             * 
             * @param config JSON Configuration of Manager
             */
            Trade_Manager(
                nlohmann::json &config
            ) : configuration(config)
            { }

            /**
             * @brief Destroy the Manager object
             */
            ~Trade_Manager() = default;

            /**
             * @brief Initialize Workers
             */
            void initialize()
            {
                for (auto& [key, val] : this->configuration["workers"].items())
                {
                    if (val["strategy"] == "EMA_Cross")
                    {
                        if (val["candle"] == "Candle")
                        {
                            Workers::Worker<Strategies::EMA_Cross, Candles::Candle> worker(
                                this->configuration["workers"][key]
                            );
                            worker.configurate(this->configuration["exchange"]);
                            this->w_ema_cross_cd.push_back(worker);
                        }
                        if (val["candle"] == "Heikin_Ashi")
                        {
                            Workers::Worker<Strategies::EMA_Cross, Candles::Heikin_Ashi> worker(
                                this->configuration["workers"][key]
                            );
                            worker.configurate(this->configuration["exchange"]);
                            this->w_ema_cross_ha.push_back(worker);
                        }
                    }
                    if (val["strategy"] == "Normalized_MACD_Cross")
                    {
                        if (val["candle"] == "Candle")
                        {
                            Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Candle> worker(
                                this->configuration["workers"][key]
                            );
                            worker.configurate(this->configuration["exchange"]);
                            this->w_n_macd_cross_cd.push_back(worker);
                        }

                        if (val["candle"] == "Heikin_Ashi")
                        {
                            Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Heikin_Ashi> worker(
                                this->configuration["workers"][key]
                            );
                            worker.configurate(this->configuration["exchange"]);
                            this->w_n_macd_cross_ha.push_back(worker);
                        }
                    }
                    if (val["strategy"] == "RSXC_ADX")
                    {
                        if (val["candle"] == "Candle")
                        {
                            Workers::Worker<Strategies::RSXC_ADX, Candles::Candle> worker(
                                this->configuration["workers"][key]
                            );
                            worker.configurate(this->configuration["exchange"]);
                            this->w_rsxc_adx_cd.push_back(worker);
                        }

                        if (val["candle"] == "Heikin_Ashi")
                        {
                            Workers::Worker<Strategies::RSXC_ADX, Candles::Heikin_Ashi> worker(
                                this->configuration["workers"][key]
                            );
                            worker.configurate(this->configuration["exchange"]);
                            this->w_rsxc_adx_ha.push_back(worker);
                        }
                    }
                }
            }

            /**
             * @brief Describe Workers
             */
            void describe_workers()
            {
                for (auto& w : this->w_ema_cross_cd)
                    w.describe_worker();
                for (auto& w : this->w_ema_cross_ha)
                    w.describe_worker();
                for (auto& w : this->w_n_macd_cross_cd)
                    w.describe_worker();
                for (auto& w : this->w_n_macd_cross_ha)
                    w.describe_worker();
                for (auto& w : this->w_rsxc_adx_cd)
                    w.describe_worker();
                for (auto& w : this->w_rsxc_adx_ha)
                    w.describe_worker();
            }

            /**
             * @brief Online backtesting
             * 
             * @param data_dir Path to data directory
             * @param start_balance Start balance in USDT
             * @param symbol_balance Start balance in Symbol
             */
            void online_backtest(
                const string& data_dir,
                double start_balance,
                double symbol_balance
            ) {
                std::queue<std::future<void>> queue;

                for (auto& w : this->w_ema_cross_cd)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::EMA_Cross, Candles::Candle>::online_backtest,
                        &w,
                        start_balance,
                        symbol_balance,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_ema_cross_ha)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::EMA_Cross, Candles::Heikin_Ashi>::online_backtest,
                        &w,
                        start_balance,
                        symbol_balance,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_n_macd_cross_cd)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Candle>::online_backtest,
                        &w,
                        start_balance,
                        symbol_balance,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_n_macd_cross_ha)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Heikin_Ashi>::online_backtest,
                        &w,
                        start_balance,
                        symbol_balance,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_rsxc_adx_cd)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::RSXC_ADX, Candles::Candle>::online_backtest,
                        &w,
                        start_balance,
                        symbol_balance,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_rsxc_adx_ha)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::RSXC_ADX, Candles::Heikin_Ashi>::online_backtest,
                        &w,
                        start_balance,
                        symbol_balance,
                        data_dir
                    );
                    f.get();
                }
            }
    
            /**
             * @brief Start trading for real money
             * 
             * @param data_dir Path to data directory
             */
            void online_start(
                const string& data_dir
            ) {
                cout
                    << "[+] Start trading for Real money" << endl
                << endl;

                std::queue<std::future<void>> queue;

                for (auto& w : this->w_ema_cross_cd)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::EMA_Cross, Candles::Candle>::start,
                        &w,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_ema_cross_ha)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::EMA_Cross, Candles::Heikin_Ashi>::start,
                        &w,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_n_macd_cross_cd)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Candle>::start,
                        &w,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_n_macd_cross_ha)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::Normalized_MACD_Cross, Candles::Heikin_Ashi>::start,
                        &w,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_rsxc_adx_cd)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::RSXC_ADX, Candles::Candle>::start,
                        &w,
                        data_dir
                    );
                    f.get();
                }
                for (auto& w : this->w_rsxc_adx_ha)
                {
                    std::future<void> f = std::async(
                        std::launch::async,
                        &Workers::Worker<Strategies::RSXC_ADX, Candles::Heikin_Ashi>::start,
                        &w,
                        data_dir
                    );
                    f.get();
                }
            }
    };
}
















#endif