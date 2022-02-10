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
         * @brief Current balance of profit with "short" positions (in symbol)
         */
        double symbol_balance;

        /**
         * @brief Current balance of profit with "short" positions (in USDT)
         */
        double symbol_balance_usdt = 0;

        /**
         * @brief Minimal balance of profit with "short" positions per testing (in symbol)
         */
        double minimal_symbol_balance;

        /**
         * @brief Minimal balance of profit with "short" positions per testing (in usdt)
         */
        double minimal_symbol_balance_usdt;

        /**
         * @brief Maximum balance of profit with "short" positions per testing (in symbol)
         */
        double maximum_symbol_balance;

        /**
         * @brief Maximum balance of profit with "short" positions per testing (in USDT)
         */
        double maximum_symbol_balance_usdt;

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
         * @brief Work for got Candle
         * 
         * @tparam Strategy Strategy type
         * @param config Config
         * @param trader Trader object
         * @param solver Solver object
         * @param candle_watcher Candle_Watcher object
         * @param indicator_watcher Indicator_Watcher object
         */
        template <class Strategy, class Candle_T>
        void work(
            nlohmann::json& config,
            Workers::Trader& trader,
            Workers::Solver<Strategy>& solver,
            Workers::Candle_Watcher& candle_watcher,
            Workers::Indicator_Watcher<Candle_T>& indicator_watcher
        )
        {
            // TODO: Remake short position
            nlohmann::json params;

            map<string, bool> signals;

            vector<Trade> trades;

            string short_goal = config["trader_params"]["strategy"]["short_goal"];

            double candle_close_price;
            double max_open_trades = config["trader_params"]["strategy"]["max_open_trade"];
            double stake_amount_trade = this->balance / max_open_trades;
            double symbol_amount_trade;
            if (short_goal == "symbol")
                symbol_amount_trade = this->symbol_balance / max_open_trades;
            if (short_goal == "usdt")
                symbol_amount_trade = this->symbol_balance_usdt / max_open_trades;

            params = indicator_watcher.get(
                config["strategy_params"],
                trader.get_symbol(),
                true
            );

            solver.resolve(params);
            trader.set_stake_amount(stake_amount_trade);
            trader.set_symbol_abount(symbol_amount_trade);
            if (config["trader_params"]["candle"] == "Candle")
                candle_close_price = candle_watcher.get_candle().get_close_price();
            else if (config["trader_params"]["candle"] == "Heikin_Ashi")
                candle_close_price = candle_watcher.get_heikin_ashi().get_close_price();
            signals = solver.get_signals(),
            trader.resolve(
                signals,
                trades,
                candle_close_price
            );

            for (Trade& trade : trades) 
            {
                if (trade.is_completed())
                {
                    if (trade.get_position() == "long")
                    {
                        // this->balance *= (trade.get_per_profit() + 100) / 100;
                        this->balance += trade.get_abs_profit();

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
                        if (short_goal == "symbol")
                        {
                            this->symbol_balance +=trade.get_abs_profit();

                            if (this->symbol_balance > this->maximum_symbol_balance)
                                this->maximum_symbol_balance = this->symbol_balance;

                            if (this->symbol_balance < this->minimal_symbol_balance)
                                this->minimal_symbol_balance = this->symbol_balance;
                        }
                        if (short_goal == "usd")
                        {
                            this->symbol_balance_usdt +=trade.get_abs_profit();

                            if (this->symbol_balance_usdt > this->maximum_symbol_balance_usdt)
                                this->maximum_symbol_balance_usdt = this->symbol_balance_usdt;

                            if (this->symbol_balance_usdt < this->minimal_symbol_balance_usdt)
                                this->minimal_symbol_balance_usdt = this->symbol_balance_usdt;
                        }

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

                        this->average_abs_profit_per_trade_short += trade.get_abs_profit();
                        this->average_per_profit_per_trade_short += trade.get_per_profit();

                        this->total_trades_short++;

                        if ((trade.get_per_profit() + 100) / 100 >= 1)
                            this->wins_short++;
                        else
                            this->loses_short++;
                    }

                    cout 
                        << "Position: " << trade.get_position() << endl
                        << "Absolute profit: " << trade.get_abs_profit() << endl
                        << "Percentage profit: " << trade.get_per_profit() << endl
                        << "Open price: " << trade.get_open_price() << endl
                        << "Close price: " << trade.get_close_price() << endl
                        << "Stake amount: " << trade.get_stake_amount() << endl
                        << "Symbol amount: " << trade.get_symbol_amount() << endl
                        << "Current balance: " << this->balance << endl
                        << "Current symbol balance: " << this->symbol_balance << endl
                    << endl;
                }
            }

            cout 
                << trader.get_name() << endl
                << trader.get_interval() << endl
                << trader.get_symbol() << endl;
            for (auto& [key, val] : solver.get_signals())
                cout << key << " : " << val << endl;
            cout
                << "Candle close: " << candle_close_price << endl
                << "Indicator params:" << endl
                << params << endl
            << endl;
        }

    public:
        /**
         * @brief Construct a new Tester object
         * 
         * @param taapi_key API Key for taapi.io
         */
        Tester(
            const string &data_file_root,
            const string& taapi_key
        ) : taapi_key(taapi_key)
        { }

        /**
         * @brief Destroy the Tester object
         */
        ~Tester() = default;

        /**
         * @brief Backtest for Candles
         * 
         * @param symbol Symbol (pair)
         * @param interval Interval (timeframe)
         * @param data_file CSV Data file object
         */
        void backtest_candle(
            const string &symbol,
            const string &interval,
            io::CSVReader<11> &data_file
        ) {
            auto start_time = std::chrono::high_resolution_clock::now();

            Heikin_Ashi heikin_ashi;
            Candle candle;

            Workers::Candle_Watcher candle_watcher(symbol, interval);

            candle_watcher.read_file_once(data_file);
            while(candle_watcher.read_file_once(data_file))
            {
                heikin_ashi = candle_watcher.get_heikin_ashi();
                candle = candle_watcher.get_candle();

                cout
                    << "Heikin Ashi" << endl
                    << "Open price: " << heikin_ashi.get_open_price() << endl
                    << "Close price: " << heikin_ashi.get_close_price() << endl
                    << "High price: " << heikin_ashi.get_high_price() << endl
                    << "Low price: " << heikin_ashi.get_low_price() << endl
                << endl;

                cout
                    << "Standard Candle" << endl
                    << "Open price: " << candle.get_open_price() << endl
                    << "Close price: " << candle.get_close_price() << endl
                    << "High price: " << candle.get_high_price() << endl
                    << "Low price: " << candle.get_low_price() << endl
                << endl;    
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            cout << "Test time (ms): " << exec_time.count() << endl;
        }

        /**
         * @brief Start backtesting for candle indicator
         * 
         * @tparam Indicator Indicator to backtest
         * @param indicator_params Indicator params
         * @param symbol Symbol to test indicator
         * @param interval Interval of candles
         * @param data_file CSV Data file object
         */
        template <class Indicator>
        void backtest_candle_indicator(
            nlohmann::json &config,
            const string &symbol,
            const string &interval,
            io::CSVReader<11> &data_file
        )
        {
            Workers::Candle_Watcher candle_watcher(symbol, interval);

            Candle candle;

            Indicator indicator;

            nlohmann::json ret;

            double close_price;

            auto start_time = std::chrono::high_resolution_clock::now();

            indicator.set_indicator_params(config);

            candle_watcher.read_file_once(data_file);
            while(candle_watcher.read_file_once(data_file))
            {
                close_price = candle_watcher.get_candle().get_close_price();
                candle = candle_watcher.get_candle();

                indicator.resolve(
                    candle   
                );

                ret = indicator.get();
                cout 
                    << "Candle close: " << close_price << endl
                    << "Indicator params:" << endl
                << endl;

                for (auto& [key, val] : ret.items())
                    cout << key << " : " << val << endl;
                cout << endl;
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            cout << "Test time (ms): " << exec_time.count() << endl;
        }

        /**
         * @brief Start backtesting for Heikin Ashi indicator
         * 
         * @tparam Indicator Indicator to backtest
         * @param indicator_params Indicator params
         * @param symbol Symbol to test indicator
         * @param interval Interval of candles
         * @param data_file CSV Data file object
         */
        template <class Indicator>
        void backtest_heikin_ashi_indicator(
            // Indicator &indicator,
            nlohmann::json &config,
            const string &symbol,
            const string &interval,
            io::CSVReader<11> &data_file
        )
        {
            Workers::Candle_Watcher candle_watcher(symbol, interval);

            Heikin_Ashi candle;

            Indicator indicator;

            nlohmann::json ret;

            double close_price;

            auto start_time = std::chrono::high_resolution_clock::now();

            indicator.set_indicator_params(config);

            candle_watcher.read_file_once(data_file);
            while(candle_watcher.read_file_once(data_file))
            {
                close_price = candle_watcher.get_heikin_ashi().get_close_price();
                candle = candle_watcher.get_heikin_ashi();

                indicator.resolve(
                    candle   
                );

                ret = indicator.get();
                cout 
                    << "Candle close: " << close_price << endl
                    << "Indicator params:" << endl
                << endl;

                for (auto& [key, val] : ret.items())
                    cout << key << " : " << val << endl;
                cout << endl;
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            cout << "Test time (ms): " << exec_time.count() << endl;
        }

        /**
         * @brief Start backtesting for strategy
         * 
         * @tparam Strategy Strategy to backtest
         * @param config Config for strategy
         * @param start_balance Start balance
         * @param start_symbol Start balance of symbols
         * @param data_file CSV Data file object
         */
        template <class Strategy>
        void backtest_strategy(
            nlohmann::json &config,
            double start_balance,
            double start_symbols,
            io::CSVReader<11> &data_file
        ) 
        {
            /* Variables */
            double max_open_trades = config["trader_params"]["strategy"]["max_open_trade"];

            string interval = config["trader_params"]["interval"];
            string symbol = config["trader_params"]["symbol"];
            string candle_type = config["trader_params"]["candle"];

            Candle candle;
            Heikin_Ashi heikin_ashi;

            Workers::Indicator_Watcher<Candle>indicator_watcher_cd;
            Workers::Indicator_Watcher<Heikin_Ashi> indicator_watcher_ha;
            Workers::Candle_Watcher candle_watcher(symbol, interval);
            Workers::Trader trader(
                config["trader_params"]
            );
            Workers::Solver<Strategy> solver(
                config["strategy_params"]
            );

            /* Initialize */
            this->balance = start_balance;
            this->minimal_balance = start_balance;
            this->maximum_balance = start_balance;

            this->symbol_balance = start_symbols;
            this->minimal_symbol_balance = start_symbols;
            this->maximum_symbol_balance = start_symbols;

            if (candle_type == "Candle")
            {
                indicator_watcher_cd.set_strategies(
                    config["strategy_params"]
                );
                indicator_watcher_cd.set_taapi_key(
                    this->taapi_key
                );
                indicator_watcher_cd.initialize_indicators();
            } else if (candle_type == "Heikin_Ashi") {
                indicator_watcher_ha.set_strategies(
                    config["strategy_params"]
                );
                indicator_watcher_ha.set_taapi_key(
                    this->taapi_key
                );
                indicator_watcher_ha.initialize_indicators();
            }

            /* Start testing */
            auto start_time = std::chrono::high_resolution_clock::now();

            candle_watcher.read_file_once(data_file);
            while(candle_watcher.read_file_once(data_file))
            {
                if (candle_type == "Candle")
                {
                    candle = candle_watcher.get_candle();
                    indicator_watcher_cd.resolve(
                        candle
                    );
                    this->work<Strategy, Candle>(
                        config,
                        trader,
                        solver,
                        candle_watcher,
                        indicator_watcher_cd
                    );
                }
                if (candle_type == "Heikin_Ashi")
                {
                    heikin_ashi = candle_watcher.get_heikin_ashi();
                    indicator_watcher_ha.resolve(
                        heikin_ashi
                    );
                    this->work<Strategy, Heikin_Ashi>(
                        config,
                        trader,
                        solver,
                        candle_watcher,
                        indicator_watcher_ha
                    );
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
                << "BALANCE:" << endl;
            if (config["trader_params"]["strategy"]["short_goal"] == "usdt")
            {
                cout 
                    << "Start Balance: " << 0 << " USDT" << endl
                    << "Final Balance: " << this->symbol_balance_usdt << " USDT" << endl 
                    << "Maximum balance: " << this->maximum_symbol_balance_usdt << " USDT"  << endl
                    << "Minimal balance: " << this->minimal_symbol_balance_usdt << " USDT" << endl
                    << endl
                    << "PROFIT:" << endl
                    << "Average absolute profit per trades: " << this->average_abs_profit_per_trade_short << " USDT" << endl
                    << "Average percentage profit per trades: " << this->average_per_profit_per_trade_short << " %" << endl
                    << endl;
            } else if (config["trader_params"]["strategy"]["short_goal"] == "symbol") {
                cout 
                    << "Start Balance: " << start_symbols << " Symbols" << endl
                    << "Final Balance: " << this->symbol_balance << " Symbols" << endl 
                    << "Maximum balance: " << this->maximum_symbol_balance << " Symbols"  << endl
                    << "Minimal balance: " << this->minimal_symbol_balance << " Symbols" << endl
                    << endl
                    << "PROFIT:" << endl
                    << "Average absolute profit per trades: " << this->average_abs_profit_per_trade_short << " Symbols" << endl
                    << "Average percentage profit per trades: " << this->average_per_profit_per_trade_short << " %" << endl
                    << endl;
            }
            cout 
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