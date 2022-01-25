#pragma once

#ifndef TESTER_HEADER
#define TESTER_HEADER

#include <iostream>
#include <map>
#include <vector>

#include "../../managers/inc/managers.hpp"
#include "../../indicators/inc/indicators.hpp"
#include "../../libs/csv/csv.hpp"

using std::string; 
using std::map; 
using std::cout; 
using std::endl; 
using std::vector;

class Tester
{
    private:
        /**
         * @brief Filename with candles data
         */
        string filename;

        /**
         * @brief Start balance
         */
        double start_balance;

        /**
         * @brief Current balance
         */
        double balance;

        /**
         * @brief Stake amount to trade
         */
        double stake_amount;

        template <class Strategy>
        void work(
            Workers::Solver<Strategy> &worker,
            Workers::Trader &trader
        ) {

        }

    public:
        /**
         * @brief Construct a new Tester object
         * 
         * @param symbol Symbol (pair) testing
         * @param timeframe Interval for testing
         * @param balance Your start balance
         * @param stake_amount Stake amount to open trade
         */
        Tester(
            const string &symbol,
            const string &timeframe,
            double balance = 10000.0,
            double stake_amount = 10000.0
        ) : start_balance(balance), balance(balance),
            stake_amount(stake_amount)
        { 
            this->filename = "../data/" + symbol + '_' + timeframe + ".csv";
        }

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
         * @param config_strategy The name of strategy in config file
         */
        template <class Strategy>
        void backtest(nlohmann::json &config, string &config_strategy)
        {
            io::CSVReader<11> file(this->filename);

            string open_price, high_price, low_price, close_price,
                   volume, quote, trades_count, tbbav, tbqav,
                   open_time, close_time
            ;

            // Managers::Employers::Employer<Strategy> employer;
            // Managers::Analysts::Analyst analyst;

            // employer.initial_workers(config[config_strategy]);

            // vector<Workers::Trader> traders = analyst.get_traders();
            // vector<Workers::Solver<Strategy>> workers = employer.get_workers();

            // while(file.read_row(
            //     open_time, open_price, high_price, 
            //     low_price, close_price, volume, close_time, 
            //     quote, trades_count, tbbav, tbqav
            // )) {
            //     // TODO: Remake without try-catch
            //     // The first value of variables is columns names
            //     try {
            //         Candle candle(
            //             std::stod(open_time),
            //             std::stod(close_time),
            //             std::stod(open_price),
            //             std::stod(high_price),
            //             std::stod(low_price),
            //             std::stod(close_price),
            //             std::stod(volume),
            //             std::stod(quote),
            //             std::stod(trades_count),
            //             std::stod(tbbav),
            //             std::stod(tbqav)
            //         );

                    
            //     } catch (std::invalid_argument& exp) {
            //         cout << exp.what() << endl;
            //         continue;
            //     }
            // }

            // Strategy strategy;

            // map<string, double> params;
            // map<string, bool> signals;

            // io::CSVReader<11> file(this->filename);

            // // TODO: Add what strategy we are testing
            // cout << "[+] Start backtesting for strategy" << endl;

            // cout << "[+] Strategy params:" << endl;
            // for (auto& [key, val] : strategy_params.items())
            // {
            //     cout << key << endl;
            //     for (auto& [k, v] : val["indicator_params"].items())
            //         cout << "    " << k << " : " << v << endl;
            // }

            // for (auto& [key, val] : strategy_params.items())
            // {
            //     if (val["indicator"] == "EMA")
            //     {
            //         Indicators::Integral::EMA ema(
            //             val["indicator_params"]
            //         );
            //     }
            // }

            // for (auto& [key, val] : strategy_params)
            // {
            //     params[key] = Indicators::Integral::EMA(
            //         val["indicator_params"]
            //     );
            // }
        }

        /**
         * @brief Start backtesting
         */
        // void backtest()
        // {
        //     io::CSVReader<11> file(this->filename);

        //     map<string, bool> signals;
        //     Indicators::Integral::EMA ema_12(12);
        //     Indicators::Integral::EMA ema_24(24);

        //     string open_price, high_price, low_price, close_price,
        //            volume, quote, trades_count, tbbav, tbqav,
        //            open_time, close_time
        //            ;
            
        //     while(file.read_row(
        //         open_time, open_price, high_price, 
        //         low_price, close_price, volume, close_time, 
        //         quote, trades_count, tbbav, tbqav
        //     )) {
        //         // TODO: Remake without try-catch
        //         // The first value of variables is columns names
        //         try {
        //             Candle candle(
        //                 std::stod(open_time),
        //                 std::stod(close_time),
        //                 std::stod(open_price),
        //                 std::stod(high_price),
        //                 std::stod(low_price),
        //                 std::stod(close_price),
        //                 std::stod(volume),
        //                 std::stod(quote),
        //                 std::stod(trades_count),
        //                 std::stod(tbbav),
        //                 std::stod(tbqav)
        //             );

        //             ema_12.resolve(candle);
        //             ema_24.resolve(candle);

        //             this->strategy.resolve(
        //                 ema_12.get_ema(), 
        //                 ema_24.get_ema(), 
        //                 signals
        //             );

        //             for (auto& [key, val] : signals)
        //                 cout << key << " : " << val << endl;
                    
        //         } catch (std::invalid_argument& exp) {
        //             cout << exp.what() << endl;
        //             continue;
        //         }
        //     }
        // }
};
























#endif