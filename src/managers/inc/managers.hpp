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
     * @brief Manager for Workers
     */
    class Manager
    {
        private:
            /**
             * @brief JSON Configuration of Manager
             */
            nlohmann::json configuration;

            /**
             * @brief Array of Candle Watcher objects
             */
            vector<Workers::Candle_Watcher> candle_watchers;

            /**
             * @brief Indicator Watcher object fo Candle type
             */
            Workers::Indicator_Watcher<Candles::Candle> indicator_watcher_cd;

            /**
             * @brief Indicator Watcher object for Heikin Ashi type
             */
            Workers::Indicator_Watcher<Candles::Heikin_Ashi> indicator_watcher_ha;

            /**
             * @brief Get the klines object
             * 
             * @param symbol Symbol (pair)
             * @param interval Interval (candle timeframe)
             * @param startTime Start time
             * @param endTime End time
             * @return nlohmann::json 
             */
            nlohmann::json get_klines(
                const string& symbol,
                const string& interval,
                double startTime,
                double endTime
            ) {

            }

        public:
            /**
             * @brief Construct a new Manager object
             */
            Manager() = default;

            /**
             * @brief Destroy the Manager object
             * 
             * @param config JSON Configuration of Manager
             */
            Manager(
                nlohmann::json &config
            ) : configuration(config)
            { }

            /**
             * @brief Destroy the Manager object
             */
            ~Manager() = default;

            /**
             * @brief Set the configuration object
             * 
             * @param config JSON Configuration
             */
            void set_configuration(nlohmann::json &config) { this->configuration = config; }

            void initialize(io::CSVReader<11> &data_file)
            {
                nlohmann::json strategies = this->configuration["strategies"];
                nlohmann::json strategy_params;

                string symbol;
                string interval;

                /* Initialize Candle Watchers */
                // from trader_params
                for (auto& [key, val] : strategies.items())
                    for (auto& [k, v] : val.items())
                    {
                        symbol = v["trader_params"]["symbol"];
                        interval = v["trader_params"]["interval"];
                        Workers::Candle_Watcher new_cw(symbol, interval);
                        if (
                            std::find(
                                this->candle_watchers.begin(), 
                                this->candle_watchers.end(), 
                                new_cw
                        ) == this->candle_watchers.end())
                            this->candle_watchers.push_back(new_cw);
                    }
                
                // from strategy_params
                for (auto& [key1, val1] : strategies.items())
                    for (auto& [key2, val2] : val1.items())
                    {
                        strategy_params = val2["strategy_params"];
                        symbol = val2["trader_params"]["symbol"];
                        for (auto& [key3, val3] : strategy_params.items())
                        {
                            interval = val3["indicator_params"]["interval"];
                            Workers::Candle_Watcher new_cw(symbol, interval);
                            if (
                                std::find(
                                    this->candle_watchers.begin(), 
                                    this->candle_watchers.end(), 
                                    new_cw
                            ) == this->candle_watchers.end())
                                this->candle_watchers.push_back(new_cw);
                        }
                    }
                
                cout << "Candle Watchers:" << endl;
                for (Workers::Candle_Watcher& cw : this->candle_watchers)
                    cout << cw.get_symbol() << " : " << cw.get_interval() << endl;
                cout << endl;
                
                /* Initialize Indicators */
                this->indicator_watcher_cd.set_strategies(
                    this->configuration["strategies"]
                );
                this->indicator_watcher_ha.set_strategies(
                    this->configuration["strategies"]
                );

                this->indicator_watcher_cd.set_taapi_key(
                    this->configuration["taapi"]["key"]
                );
                this->indicator_watcher_ha.set_taapi_key(
                    this->configuration["taapi"]["key"]
                );

                this->indicator_watcher_cd.initialize_indicators();
                this->indicator_watcher_ha.initialize_indicators();

                cout << "Candle:" << endl;
                this->indicator_watcher_cd.describe_indicators();
                cout << endl;
                cout << "Heikin Ashi:" << endl;
                this->indicator_watcher_ha.describe_indicators();
                cout << endl;

                for (Workers::Candle_Watcher& cw : this->candle_watchers)
                {
                    cw.read_file_once(data_file);
                    while(cw.read_file_once(data_file))
                    {

                    }
                }

                for (Workers::Candle_Watcher& cw : this->candle_watchers)
                {

                }
            }
    };
}
















#endif