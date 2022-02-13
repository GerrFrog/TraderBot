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
     * @brief Manager for Workers (who trades)
     */
    // class Trade_Manager
    // {
    //     private:
    //         /**
    //          * @brief JSON Configuration of Worker
    //          */
    //         nlohmann::json configuration;

    //         vector<Workers::Candle_Watcher> candle_watchers;

    //         vector<Workers::Indicator_Watcher<Candles::Candle>> indicator_watchers_cd;

    //         vector<Workers::Indicator_Watcher<Candles::Heikin_Ashi>> indicator_watchers_ha;

    //     public:
    //         /**
    //          * @brief Construct a new Manager object
    //          */
    //         Trade_Manager() = default;

    //         /**
    //          * @brief Destroy the Manager object
    //          * 
    //          * @param config JSON Configuration of Manager
    //          */
    //         Trade_Manager(
    //             nlohmann::json &config
    //         ) : configuration(config)
    //         { }

    //         /**
    //          * @brief Destroy the Manager object
    //          */
    //         ~Trade_Manager() = default;

    //         /**
    //          * @brief Set the configuration object
    //          * 
    //          * @param config JSON Configuration
    //          */
    //         void set_configuration(nlohmann::json &config) { this->configuration = config; }

    //         void initialize(const string& data_dir)
    //         {
    //             nlohmann::json strategies = this->configuration["strategies"];
    //             nlohmann::json strategy_params;

    //             string symbol;
    //             string interval;

    //             /* Initialize Candle Watchers */
    //             cout << "[+] Start initializing Candle Watchers" << endl;
    //             // from trader_params
    //             symbol = this->configuration["trader_params"]["symbol"];
    //             interval = this->configuration["trader_params"]["interval"];
    //             Workers::Candle_Watcher new_cw(symbol, interval);
    //             if (
    //                 std::find(
    //                     this->candle_watchers.begin(), 
    //                     this->candle_watchers.end(), 
    //                     new_cw
    //                 ) == this->candle_watchers.end()
    //             )
    //                 this->candle_watchers.push_back(new_cw);
                
    //             // from strategy_params
    //             strategy_params = this->configuration["strategy_params"];
    //             symbol = this->configuration["trader_params"]["symbol"];
    //             for (auto& [key3, val3] : strategy_params.items())
    //             {
    //                 interval = val3["indicator_params"]["interval"];
    //                 Workers::Candle_Watcher new_cw(symbol, interval);
    //                 if (
    //                     std::find(
    //                         this->candle_watchers.begin(), 
    //                         this->candle_watchers.end(), 
    //                         new_cw
    //                     ) == this->candle_watchers.end()
    //                 )
    //                     this->candle_watchers.push_back(new_cw);
    //             }
                
    //             cout << "Candle Watchers:" << endl;
    //             for (Workers::Candle_Watcher& cw : this->candle_watchers)
    //                 cout << cw.get_symbol() << " : " << cw.get_interval() << endl;
    //             cout << endl;
                
    //             /* Initialize Indicators */
    //             cout << "[+] Start initializing Indicator Watchers" << endl;
    //             for (auto& [key, val] : this->configuration["strategy_params"].items())
    //             {
    //                 if (val["candle"] == "Candle")
    //                     this->indicator_watchers_cd.push_back(
    //                         Workers::Indicator_Watcher<Candles::Candle>(
    //                             this->configuration["strategy_params"][key],
    //                             ""
    //                         )
    //                     );
    //                 if (val["candle"] == "Heikin_Ashi")
    //                     this->indicator_watchers_ha.push_back(
    //                         Workers::Indicator_Watcher<Candles::Heikin_Ashi>(
    //                             this->configuration["strategy_params"][key],
    //                             ""
    //                         )
    //                     );
    //             }

    //             cout << "Indicator Watchers:" << endl;
    //             for (auto& ind_w : this->indicator_watchers_cd)
    //                 cout << ind_w.get_interval() << " : " << ind_w.get_description() << endl;
    //             cout << endl;
    //             for (auto& ind_w : this->indicator_watchers_ha)
    //                 cout << ind_w.get_interval() << " : " << ind_w.get_description() << endl;

    //             for (auto& ind_w : this->indicator_watchers_cd)
    //                 ind_w.initialize_indicators();
    //             for (auto& ind_w : this->indicator_watchers_ha)
    //                 ind_w.initialize_indicators();
                
    //             cout << "Indicators initialized:"<< endl;
    //             for (auto& ind_w : this->indicator_watchers_cd)
    //                 ind_w.describe_indicators();
    //             for (auto& ind_w : this->indicator_watchers_ha)
    //                 ind_w.describe_indicators();

                // for (Workers::Candle_Watcher& cw : this->candle_watchers)
                // {
                //     string sym = cw.get_symbol();
                //     string symbol;
                //     string interval = cw.get_interval();
                //     string filename;

                //     Candles::Candle candle;
                //     Candles::Heikin_Ashi heikin_ashi;

                //     std::remove_copy(
                //         sym.begin(),
                //         sym.end(),
                //         std::back_inserter(symbol),
                //         '/'
                //     );

                //     filename = data_dir + symbol + "_" + interval + ".csv";

                //     io::CSVReader<11> data_file(
                //         filename
                //     );

                //     cout 
                //         << "[+] Start initializing Indicator Watcher" << endl
                //         << "Candle Watcher: " << sym << ", " << interval << endl
                //         << "Data file: " << filename << endl
                //     << endl;

                //     // TODO: "1", "2" and etc are workers
                //     // They have own indicator watcher and candle watcher
                //     // They are workers
                //     // Regardless they use the same indicators and candles they must be separated
                //     // TODO: Candle Watcher must be templated

                //     cw.read_file_once(data_file);
                //     while(cw.read_file_once(data_file))
                //     {
                //         candle = cw.get_candle();
                //         heikin_ashi = cw.get_heikin_ashi();

                //         this->indicator_watcher_cd.resolve(
                //             candle
                //         );
                //         this->indicator_watcher_ha.resolve(
                //             heikin_ashi
                //         );
                //     }
                // }
    //         }

    // };

}
















#endif