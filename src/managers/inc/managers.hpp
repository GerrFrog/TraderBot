#pragma once

#ifndef MANAGER_HEADER
#define MANAGER_HEADER

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
     * @brief Manager for workers
     */
    class Manager
    {
        private:
            /**
             * @brief Strategies in config
             */
            nlohmann::json strategies;

            /**
             * @brief Watcher for indicators
             */
            Workers::Watcher watcher;

            /**
             * @brief All Traders
             */
            vector<Workers::Trader> ema_cross_traders;

            /**
             * @brief EMA Cross Strategy Solver
             */
            vector<Workers::Solver<Strategies::EMA_Cross>> ema_cross_solvers;

            /**
             * @brief Work for pair
             * 
             * @tparam Strategy Strategy to work
             * @param trader Trader
             * @param solver Solver
             */
            template <class Strategy>
            void work(
                Workers::Trader &trader,
                Workers::Solver<Strategy> &solver
            )
            {
                nlohmann::json strategy_params = solver.get_strategy_params();

                // TODO: Candle for watcher
                // TODO: Request to taapi for indicator when it's time (check timeframe in strategy)
                map<string, double> params = this->watcher.get(
                    strategy_params,
                    trader.get_symbol()
                );

                solver.resolve(params);
                trader.resolve(
                    solver.get_buy_signal(),
                    solver.get_sell_signal()
                );

                cout << trader.get_name() << endl;
                cout << trader.get_timeframe() << endl;
                cout << trader.get_symbol() << endl;
                cout << "Work: " << trader.is_work() << endl;
                cout << "Sell signal: " << solver.get_sell_signal() << endl;
                cout << "Buy signal: " << solver.get_buy_signal() << endl;
            }

        public:
            /**
             * @brief Construct a new Manager object
             * 
             * @param strategies Strategies in config
             * @param taapi_key API Key for taapi.io
             */
            Manager(
                nlohmann::json &strategies,
                const string &taapi_key
            ) : strategies(strategies)
            { 
                this->watcher.set_strategies(
                    strategies
                );
                this->watcher.set_taapi_key(
                    taapi_key
                );

                for (auto& [key1, val1] : strategies.items())
                {
                    for (auto& [key2, val2] : val1.items())
                    {
                        this->ema_cross_traders.push_back(
                            Workers::Trader(
                                val2["trader_params"]
                            )
                        );
                        this->ema_cross_solvers.push_back(
                            Workers::Solver<Strategies::EMA_Cross>(
                                val2["strategy_params"]
                            )
                        );
                    }

                }
            }

            /**
             * @brief Destroy the Manager object
             */
            ~Manager() = default;

            /**
             * @brief 
             */
            void run()
            {
                cout << "[+] Start working" << endl;

                while (true)
                    for (int i = 0; i < this->ema_cross_solvers.size(); i++)
                        this->work<Strategies::EMA_Cross>(
                            this->ema_cross_traders[i],
                            this->ema_cross_solvers[i]
                        );
            }
    };
}
















#endif