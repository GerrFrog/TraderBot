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
    // TODO: if symbol_amount = all then we trade for symbol increase else standard "short" position strategy
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
        double balance = 0;

        /**
         * @brief Minimal balance per testing
         */
        double minimal_balance;

        /**
         * @brief Maximum balance per testing
         */
        double maximum_balance;

        /**
         * @brief Count of symbols to trade "short" positions
         */
        double symbol_amount;

        /**
         * @brief Current balance of profit with "short" positions (in USDT)
         */
        double symbol_balance = 0;

        /**
         * @brief Minimal balance of profit with "short" positions per testing (in USDT)
         */
        double minimal_symbol_balance;

        /**
         * @brief Maximum balance of profit with "short" positions per testing (in USDT)
         */
        double maximum_symbol_balance;

        /**
         * @brief Absolute (USD) profit
         */
        double abs_profit_long = 0;

        /**
         * @brief Percentage profit
         */
        double per_profit_long = 0;

        /**
         * @brief Average absolute profit per trades
         */
        double average_abs_profit_per_trade_long = 0;

        /**
         * @brief Average percentage profit per trades
         */
        double average_per_profit_per_trade_long = 0;

        /**
         * @brief Absolute (USD) profit
         */
        double abs_profit_short = 0;

        /**
         * @brief Percentage profit
         */
        double per_profit_short = 0;

        /**
         * @brief Average absolute profit per trades
         */
        double average_abs_profit_per_trade_short = 0;

        /**
         * @brief Average percentage profit per trades
         */
        double average_per_profit_per_trade_short = 0;

        /**
         * @brief Total Trades per testing
         */
        int total_trades_long = 0;

        /**
         * @brief Total win trades
         */
        int wins_long = 0;

        /**
         * @brief Total lose trades
         */
        int loses_long = 0;

        /**
         * @brief Percent of win trades
         */
        double per_wins_long;

        /**
         * @brief Total Trades per testing
         */
        int total_trades_short = 0;

        /**
         * @brief Total win trades
         */
        int wins_short = 0;

        /**
         * @brief Total lose trades
         */
        int loses_short = 0;

        /**
         * @brief Percent of win trades
         */
        double per_wins_short;

        /**
         * @brief Best long Trade per testing
         */
        double best_long_trade = 0.0;

        /**
         * @brief Worst long Trade per testing
         */
        double worst_long_trade = 0.0;

        /**
         * @brief Best short Trae per testing
         */
        double best_short_trade = 0.0;

        /**
         * @brief Worst short Trae per testing
         */
        double worst_short_trade = 0.0;

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
            trader.set_symbol_abount(this->symbol_amount);
            trader.resolve(
                solver.get_buy_signal(),
                solver.get_sell_signal(),
                trade,
                &candle
            );

            if (trade.is_completed())
            {
                if (trade.get_position() == "long")
                {
                    this->balance *= (trade.get_per_profit() + 100) / 100;

                    if (this->best_long_trade == 0.0)
                        this->best_long_trade = trade.get_per_profit();
                    else
                        if (this->best_long_trade <= trade.get_per_profit())
                            this->best_long_trade = trade.get_per_profit();
                    if (this->worst_long_trade == 0.0)
                        this->worst_long_trade = trade.get_per_profit();
                    else
                        if (this->worst_long_trade >= trade.get_per_profit())
                            this->worst_long_trade = trade.get_per_profit();

                    if (this->balance > this->maximum_balance)
                        this->maximum_balance = this->balance;
                    if (this->balance < this->minimal_balance)
                        this->minimal_balance = this->balance;

                    this->average_abs_profit_per_trade_long += trade.get_abs_profit();
                    this->average_per_profit_per_trade_long += trade.get_per_profit();

                    this->total_trades_long++;

                    if ((trade.get_per_profit() + 100) / 100 >= 1)
                        this->wins_long++;
                    else
                        this->loses_long++;
                } else if (trade.get_position() == "short") {
                    this->symbol_balance +=trade.get_abs_profit();

                    if (this->best_short_trade == 0.0)
                        this->best_short_trade = trade.get_per_profit();
                    else
                        if (this->best_short_trade <= trade.get_per_profit())
                            this->best_short_trade = trade.get_per_profit();
                    if (this->worst_short_trade == 0.0)
                        this->worst_short_trade = trade.get_per_profit();
                    else
                        if (this->worst_short_trade >= trade.get_per_profit())
                            this->worst_short_trade = trade.get_per_profit();

                    if (this->symbol_balance > this->maximum_symbol_balance)
                        this->maximum_symbol_balance = this->symbol_balance;
                    if (this->symbol_balance < this->minimal_symbol_balance)
                        this->minimal_symbol_balance = this->symbol_balance;

                    this->average_abs_profit_per_trade_short += trade.get_abs_profit();
                    this->average_per_profit_per_trade_short += trade.get_per_profit();

                    this->total_trades_short++;

                    if ((trade.get_per_profit() + 100) / 100 >= 1)
                        this->wins_short++;
                    else
                        this->loses_short++;
                }

                cout << "Position: " << trade.get_position() << endl
                     << "Absolute profit: " << trade.get_abs_profit() << endl
                     << "Percentage profit: " << trade.get_per_profit() << endl
                     << "Open price: " << trade.get_open_price() << endl
                     << "Close price: " << trade.get_close_price() << endl
                     << "Stake amount: " << trade.get_stake_amount() << endl
                     << "Symbol amount: " << trade.get_symbol_amount() << endl
                     << "Current balance: " << this->balance << endl
                     << "Current symbol balance: " << this->symbol_balance << endl
                     << endl
                ;
            }

            cout << trader.get_name() << endl;
            cout << trader.get_timeframe() << endl;
            cout << trader.get_symbol() << endl;
            for (auto& [key, val] : params.items())
                cout << key << " : " << val << endl;
            cout << "Work: " << trader.is_work() << endl;
            cout << "Sell signal: " << solver.get_sell_signal() << endl;
            cout << "Buy signal: " << solver.get_buy_signal() << endl;
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
            double start_balance,
            double start_symbols
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
            this->symbol_amount = start_symbols;
            this->minimal_symbol_balance = 0;
            this->maximum_symbol_balance = 0;

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

            this->abs_profit_long = this->balance - start_balance;
            this->per_profit_long = this->balance / start_balance * 100 - 100;
            this->average_abs_profit_per_trade_long /= this->total_trades_long;
            this->average_per_profit_per_trade_long /= this->total_trades_long;

            this->average_abs_profit_per_trade_short /= this->total_trades_short;
            this->average_per_profit_per_trade_short /= this->total_trades_short;

            this->per_wins_long = (double)this->wins_long / (double)this->total_trades_long * 100;
            this->per_wins_short = (double)this->wins_short / (double)this->total_trades_short * 100;

            cout 
                << "----------------------------------" << endl
                << "LONG POSITION STATISTIC:" << endl
                << endl
                << "BALANCE:" << endl
                << "Start Balance: " << start_balance << " USDT" << endl
                << "Final Balance: " << this->balance << " USDT" << endl 
                << "Maximum balance: " << this->maximum_balance<< " USDT"  << endl
                << "Minimal balance: " << this->minimal_balance << " USDT" << endl
                << endl
                << "PROFIT:" << endl
                << "Absolute profit: " << this->abs_profit_long << " USDT" << endl
                << "Percentage profit: " << this->per_profit_long << " %" << endl
                << "Average absolute profit per trades: " << this->average_abs_profit_per_trade_long << " USDT" << endl
                << "Average percentage profit per trades: " << this->average_per_profit_per_trade_long << " %" << endl
                << endl
                << "TRADE" << endl
                << "Total trades: " << this->total_trades_long << endl
                << "Wins: " << this->wins_long << endl
                << "Loses: " << this->loses_long << endl
                << "Percent wins: " << this->per_wins_long << " %" << endl
                << "Best long trade: " << this->best_long_trade << " %" << endl
                << "Worst long trade: " << this->worst_long_trade << " %" << endl
                << "----------------------------------" << endl
                << "SHORT POSITION STATISTIC:" << endl
                << endl
                << "BALANCE:" << endl
                << "Start Balance: " << 0 << " USDT" << endl
                << "Final Balance: " << this->symbol_balance << " USDT" << endl 
                << "Maximum balance: " << this->maximum_symbol_balance<< " USDT"  << endl
                << "Minimal balance: " << this->minimal_symbol_balance << " USDT" << endl
                << endl
                << "PROFIT:" << endl
                // << "Absolute profit: " << this->abs_profit_short << " USDT" << endl
                // << "Percentage profit: " << this->per_profit_short << " %" << endl
                << "Average absolute profit per trades: " << this->average_abs_profit_per_trade_short << " USDT" << endl
                << "Average percentage profit per trades: " << this->average_per_profit_per_trade_short << " %" << endl
                << endl
                << "TRADE" << endl
                << "Total trades: " << this->total_trades_short << endl
                << "Wins: " << this->wins_short << endl
                << "Loses: " << this->loses_short << endl
                << "Percent wins: " << this->per_wins_short << " %" << endl
                << "Best long trade: " << this->best_short_trade << " %" << endl
                << "Worst long trade: " << this->worst_short_trade << " %" << endl
                << "----------------------------------" << endl
                << "Test time (ms): " << exec_time.count() << endl
            ;
        }
};
























#endif