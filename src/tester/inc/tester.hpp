#pragma once

#ifndef TESTER_HEADER
#define TESTER_HEADER

#include <iostream>
#include <map>

#include "../../libs/csv/csv.hpp"

using std::string, std::map, std::cout, std::endl;

template <class Strategy>
class Tester
{
    private:
        /**
         * @brief Strategy for testing
         */
        Strategy strategy;

        /**
         * @brief Filename with candles data
         */
        string filename;

    public:
        /**
         * @brief Construct a new Tester object
         */
        Tester() { }

        /**
         * @brief Construct a new Tester object
         * 
         * @param strategy Strategy for testing
         * @param symbol Symbol (pair) testing
         * @param timeframe Interval for testing
         */
        Tester(
            const string &symbol,
            const string &timeframe
        )
        { 
            this->filename = "../data/" + symbol + '_' + timeframe + ".csv";
        }

        /**
         * @brief Destroy the Tester object
         */
        ~Tester() { }

        /**
         * @brief Start backtesting
         */
        void backtest()
        {
            map<string, bool> signals;
            io::CSVReader<13> file(this->filename);

            this->strategy.resolve(1, 2, signals);
            this->strategy.resolve(4, 2, signals);

            for (auto& [key, val] : signals)
                cout << key << " : " << val << endl;
        }
};
























#endif