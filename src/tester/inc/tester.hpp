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

// TODO: Backtest with profit/stop-loss strategy

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
         * @brief Stake amount to trade
         */
        double stake_amount;

        /**
         * @brief Current balance
         */
        double balance;

        /**
         * @brief Minimal balance per testing
         */
        double minimal_balance;

        /**
         * @brief Maximum balance per testing
         */
        double maximum_balance;

        /**
         * @brief Absolute (USD) profit
         */
        double abs_profit = 0;

        /**
         * @brief Percentage profit
         */
        double per_profit = 0;

        /**
         * @brief Total Trades per testing
         */
        int total_trades = 0;

        /**
         * @brief Total win trades
         */
        int wins = 0;

        /**
         * @brief Total lose trades
         */
        int loses = 0;

        /**
         * @brief Percent of win trades
         */
        double per_wins;

        /**
         * @brief Best Trade per testing
         */
        double best_trade = 0.0;

        /**
         * @brief Worst Trade per testing
         */
        double worst_trade = 0.0;

        /**
         * @brief Status of Trade for previous Candle
         */
        bool last_trade_state = false;

        /**
         * @brief Work for got candle
         * 
         * @tparam Strategy Strategy for testing
         * @param config Configuration for strategy
         * @param trader Trader
         * @param solver Solver
         * @param watcher Watcher
         * @param candle Current Candle
         */
        template <class Strategy>
        void work(
            nlohmann::json &config,
            Workers::Trader &trader,
            Workers::Solver<Strategy> &solver,
            Workers::Watcher &watcher,
            Candle &candle
        )
        {
            Trade trade;

            watcher.resolve(candle);

            nlohmann::json params = watcher.get(
                config["strategy_params"],
                trader.get_symbol(),
                true
            );

            solver.resolve(params);
            trader.set_stake_amount(this->balance);
            trader.resolve(
                solver.get_buy_signal(),
                solver.get_sell_signal(),
                trade,
                &candle
            );

            if (trader.is_work() && !this->last_trade_state)
                this->last_trade_state = true;
            if (!trader.is_work() && this->last_trade_state)
            {
                this->last_trade_state = false;
                this->balance *= (trade.get_per_profit() + 100) / 100;
                this->total_trades++;

                if (this->best_trade == 0.0)
                    this->best_trade = trade.get_per_profit();
                else
                    if (this->best_trade <= trade.get_per_profit())
                        this->best_trade = trade.get_per_profit();
                if (this->worst_trade == 0.0)
                    this->worst_trade = trade.get_per_profit();
                else
                    if (this->worst_trade >= trade.get_per_profit())
                        this->worst_trade = trade.get_per_profit();

                if ((trade.get_per_profit() + 100) / 100 >= 1)
                    this->wins++;
                else
                    this->loses++;

                if (balance > this->maximum_balance)
                    this->maximum_balance = balance;
                if (balance < this->minimal_balance)
                    this->minimal_balance = balance;

                cout << "Absolute profit: " << trade.get_abs_profit() << endl;
                cout << "Percentage profit: " << trade.get_per_profit() << endl;
                cout << "Open price: " << trade.get_open_price() << endl;
                cout << "Close price: " << trade.get_close_price() << endl;
                cout << "Stake amount: " << trade.get_stake_amount() << endl;
                cout << "Current balance: " << this->balance << endl;
                cout << endl;
            }

            cout << trader.get_name() << endl;
            cout << trader.get_timeframe() << endl;
            cout << trader.get_symbol() << endl;
            for (auto& [key, val] : params.items())
                cout << key << " : " << val << endl;
            cout << "Work: " << trader.is_work() << endl;
            cout << "Sell signal: " << solver.get_sell_signal() << endl;
            cout << "Buy signal: " << solver.get_buy_signal() << endl;
            cout << "Total trades: " << this->total_trades << endl;
            cout << "Candle close: " << candle.get_close_price() << endl;
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
            this->minimal_balance = start_balance;
            this->maximum_balance = start_balance;

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

            this->abs_profit = this->balance - start_balance;
            this->per_profit = this->balance / start_balance * 100;

            this->per_wins = (double)this->wins / (double)this->total_trades;

            cout 
                << "Wins: " << this->wins << endl
                << "Loses: " << this->loses << endl
                << "Percent wins: " << this->per_wins << '%' << endl
                << "----------------------------------" << endl
                << "Start Balance: " << start_balance << endl
                << "Final Balance: " << this->balance << endl 
                << "Maximum balance: " << this->maximum_balance << endl
                << "Minimal balance: " << this->minimal_balance << endl
                << "----------------------------------" << endl
                << "Total trades: " << this->total_trades << endl
                << "Best trade: " << this->best_trade << '%' << endl
                << "Worst trade: " << this->worst_trade << '%' << endl
                << "----------------------------------" << endl
                << "Absolute profit: " << this->abs_profit << endl
                << "Percentage profit: " << this->per_profit << '%' << endl
                << "----------------------------------" << endl
                << "Test time (ms): " << exec_time.count() << endl
            ;
        }
};
























#endif