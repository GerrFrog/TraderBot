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
    public:
        /**
         * @brief Construct a new Tester object
         */
        Tester() = default;

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
            const string &data_dir
        ) {
            string sym;

            std::remove_copy(
                symbol.begin(),
                symbol.end(),
                std::back_inserter(sym),
                '/'
            );

            io::CSVReader<11> data_file(
                data_dir + sym + '_' + interval +".csv"               
            );

            Candles::Heikin_Ashi heikin_ashi;
            Candles::Candle candle;

            Workers::Watchers::Candle_Watcher candle_watcher(symbol, interval);

            auto start_time = std::chrono::high_resolution_clock::now();

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
         * @param data_dir Path to directory with data
         */
        template <class Indicator>
        void backtest_candle_indicator(
            nlohmann::json &config,
            const string &symbol,
            const string &interval,
            const string &data_dir
        )
        {
            string sym;

            std::remove_copy(
                symbol.begin(),
                symbol.end(),
                std::back_inserter(sym),
                '/'
            );

            io::CSVReader<11> data_file(
                data_dir + sym + '_' + interval +".csv"               
            );

            Workers::Watchers::Candle_Watcher candle_watcher(symbol, interval);

            Candles::Candle candle;

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
         * @param data_dir Path to directory with data
         */
        template <class Indicator>
        void backtest_heikin_ashi_indicator(
            nlohmann::json &config,
            const string &symbol,
            const string &interval,
            const string &data_dir
        )
        {
            string sym;

            std::remove_copy(
                symbol.begin(),
                symbol.end(),
                std::back_inserter(sym),
                '/'
            );

            io::CSVReader<11> data_file(
                data_dir + sym + '_' + interval +".csv"               
            );

            Workers::Watchers::Candle_Watcher candle_watcher(symbol, interval);

            Candles::Heikin_Ashi candle;

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
};
























#endif