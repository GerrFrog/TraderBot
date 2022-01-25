#pragma once

#ifndef TESTER_HEADER
#define TESTER_HEADER

#include <iostream>
#include <map>

#include "../../libs/csv/csv.hpp"
#include "../../indicators/inc/indicators.hpp"

using std::string, std::map, std::cout, std::endl;

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
        double start_balance = 10000.0;

        /**
         * @brief Current balance
         */
        double balance = 10000.0;

        /**
         * @brief Stake amount to trade
         */
        double stake_amount = 10000.0;

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

        void backtest()
        {

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