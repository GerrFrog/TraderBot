#pragma once

#ifndef TESTER_HEADER
#define TESTER_HEADER

#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>

#include "../../managers/inc/managers.hpp"
#include "../../indicators/inc/indicators.hpp"
#include "../../libs/csv/csv.hpp"

using std::string; 
using std::map; 
using std::cout; 
using std::endl; 
using std::vector;

/**
 * @brief Tester for configured strategy
 */
class Tester
{
    private:
        /**
         * @brief Filename with candles data
         */
        string filename;

        /**
         * @brief API Key for taapi.io
         */
        string taapi_key;

        /**
         * @brief Current balance
         */
        double balance;

        /**
         * @brief Stake amount to trade
         */
        double stake_amount;

        /**
         * @brief Count of Closed trades in backtesting
         */
        int closed_trades = 0;

        template <class Strategy>
        void work(
            nlohmann::json &config,
            Workers::Trader &trader,
            Workers::Solver<Strategy> &solver,
            Workers::Watcher &watcher,
            Candle &candle
        )
        {
            watcher.resolve(candle);
            
            map<string, double> params = watcher.get(
                config["strategy_params"],
                trader.get_symbol(),
                true
            );

            solver.resolve(params);
            trader.resolve(
                solver.get_buy_signal(),
                solver.get_sell_signal()
            );

            if (solver.get_sell_signal())
                this->closed_trades++;

            cout << trader.get_name() << endl;
            cout << trader.get_timeframe() << endl;
            cout << trader.get_symbol() << endl;
            for (auto& [key, val] : params)
                cout << key << " : " << val << endl;
            cout << "Work: " << trader.is_work() << endl;
            cout << "Sell signal: " << solver.get_sell_signal() << endl;
            cout << "Buy signal: " << solver.get_buy_signal() << endl;
            cout << "Closed trades: " << closed_trades << endl;
            cout << endl;
        }

    public:
        /**
         * @brief Construct a new Tester object
         * 
         * @param taapi_key API Key for taapi.io
         * @param data_file_root Relative path to dir with data
         */
        Tester(
            const string &data_file_root,
            const string& taapi_key
        ) : taapi_key(taapi_key), filename(data_file_root)
        { }

        /**
         * @brief Destroy the Tester object
         */
        ~Tester() 
        { }

        /**
         * @brief Start backtesting 
         * 
         * @tparam Strategy Strategy to backtest
         * @param config Config for strategy
         * @param start_balance Start balance
         */
        template <class Strategy>
        void backtest(
            nlohmann::json &config,
            double start_balance
        ) 
        {
            auto start_time = std::chrono::high_resolution_clock::now();

            string open_price, high_price, low_price, close_price,
                   volume, quote, trades_count, tbbav, tbqav,
                   open_time, close_time
            ;
            string symbol = config["trader_params"]["symbol"];
            string timeframe = config["trader_params"]["timeframe"];

            Workers::Watcher watcher;
            Workers::Trader trader(
                config["trader_params"]
            );
            Workers::Solver<Strategy> solver(
                config["strategy_params"]
            );

            this->balance = start_balance;

            watcher.set_strategies(
                config["strategy_params"]
            );
            watcher.set_taapi_key(
                this->taapi_key
            );
            watcher.initialize();

            std::remove_copy(
                symbol.begin(),
                symbol.end(),
                std::back_inserter(this->filename),
                '/'
            );
            this->filename += '_' + timeframe + ".csv";

            io::CSVReader<11> file(this->filename);
            while(file.read_row(
                open_time, open_price, high_price, 
                low_price, close_price, volume, close_time, 
                quote, trades_count, tbbav, tbqav
            )) {
                // TODO: Remake without try-catch. The first value of variables is columns names
                try {
                    Candle candle(
                        std::stod(open_time),
                        std::stod(close_time),
                        std::stod(open_price),
                        std::stod(high_price),
                        std::stod(low_price),
                        std::stod(close_price),
                        std::stod(volume),
                        std::stod(quote),
                        std::stod(trades_count),
                        std::stod(tbbav),
                        std::stod(tbqav)
                    );

                    this->work(
                        config,
                        trader,
                        solver,
                        watcher,
                        candle
                    );
                } catch (std::invalid_argument& exp) {
                    cout << exp.what() << endl;
                    continue;
                }
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            cout << "Test time (ms): " << exec_time.count() << endl;
        }
};
























#endif