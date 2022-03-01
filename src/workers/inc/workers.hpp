#pragma once

#ifndef WORKERS_HEADER
#define WORKERS_HEADER

#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <vector>
#include <type_traits>
#include <chrono>
#include <sstream>
#include <memory>
#include <fstream>

#include "../../strategies/inc/strategies.hpp"
#include "../../indicators/inc/indicators.hpp"
#include "../../candles/inc/candles.hpp"
#include "../../exceptions/inc/exceptions.hpp"
#include "../../trade/inc/trade.hpp"
#include "../../request/inc/request.hpp"
#include "../../binapi/inc/binapi.hpp"
#include "../../libs/csv/csv.hpp"

#define CANDLE_1D_DURATION  86399999
#define CANDLE_12H_DURATION 43199999
#define CANDLE_8H_DURATION  28799999
#define CANDLE_6H_DURATION  21599999
#define CANDLE_4H_DURATION  14399999
#define CANDLE_2H_DURATION  7199999
#define CANDLE_1H_DURATION  3599999
#define CANDLE_30M_DURATION 1799999
#define CANDLE_15M_DURATION 899999
#define CANDLE_5M_DURATION  299999
#define CANDLE_1M_DURATION  59999

using std::string; 
using std::cout; 
using std::endl; 
using std::map; 
using std::vector;

/**
 * @brief Abstract classes for Workers namespace
 */
namespace Workers::Abstract
{
    /**
     * @brief Abstract class for Workers
     * 
     * @tparam Candle_T Type of Candle
     */
    class Worker_Abstract
    {
        private:

        protected:
            /**
             * @brief Worker name
             */
            string name;
            
            /**
             * @brief Worker strategy
             */
            string strategy;

            /**
             * @brief Candle type of Worker
             */
            string candle_type;

            /**
             * @brief Worker interval
             */
            string interval;

            /**
             * @brief Worker pair
             */
            string pair;

            /**
             * @brief Path to data directory
             */
            string data_dir;

            /**
             * @brief JSON Worker configuration
             */
            nlohmann::json worker_configuration;

        public:
            /**
             * @brief Construct a new Worker_Abstract object
             * 
             * @param config Worker configuration JSON
             * @param data_dir Path to directory with data
             */
            Worker_Abstract(
                nlohmann::json &config,
                const string &data_dir
            ) : worker_configuration(config),
                data_dir(data_dir)
            {
                this->name = worker_configuration["name"];
                this->strategy = worker_configuration["strategy"];
                this->candle_type = worker_configuration["candle"];
                this->interval = worker_configuration["interval"];
                this->pair = worker_configuration["trader_params"]["pair"];
            }

            /**
             * @brief Describe worker
             */
            void describe_worker()
            {
                cout
                    << "Worker description:" << endl
                    << "Name: " << this->name << endl
                    << "Strategy: " << this->strategy << endl
                    << "Candle type: " << this->candle_type << endl
                    << "Interval: " << this->interval << endl
                    << "Symbol: " << this->pair << endl 
                << endl;
            }

            /**
             * @brief Backtest Worker with file
             * 
             * @param long_balance Start balance for long trades
             * @param short_balance Start balance for short trades
             */
            virtual void file_backtest(double long_balance, double short_balance) = 0;

            /**
             * @brief Online backtesting
             * 
             * @param long_balance Start balance for long trades
             * @param short_balance Start balance for short trades
             */
            virtual void online_backtest(double long_balance, double short_balance) = 0;
    };

    /**
     * @brief Abstract class for Trader objects
     */
    class Trader_Abstract
    {
        private:

        protected:
            /**
             * @brief Params for trader
             */
            nlohmann::json trader_params;

            /**
             * @brief Pair to trade
             */
            string pair;

            /**
             * @brief String of Pair without '/' character
             */
            string erased_pair;

            /**
             * @brief First symbol from pair
             */
            string short_symbol;

            /**
             * @brief Second symbol from pair
             */
            string long_symbol;

            /**
             * @brief Exchange
             */
            string exchange;

            /**
             * @brief Trader type
             */
            string type = "position";

            /**
             * @brief What positions are traded
             */
            string positions = "both";

            /**
             * @brief Start short balance
             */
            double short_start_balance = 0;

            /**
             * @brief Balance for short trades
             */
            double short_balance = 0;

            /**
             * @brief Start long balance
             */
            double long_start_balance = 0;

            /**
             * @brief Balance for long trades
             */
            double long_balance = 0;

            /**
             * @brief Maximum opened trades
             */
            double max_open_trade = 1;

            /**
             * @brief Stake amount to "long" trade
             */
            double long_amount = 0.0;

            /**
             * @brief Stake amount to "short" trade
             */
            double short_amount = 0.0;

            /**
             * @brief Stop-loss percent
             */
            double stop_loss = 0.0;

            /**
             * @brief Target percent
             */
            double target = 0.0;

        public:
            /**
             * @brief Construct a new Trader_Abstract object
             * 
             * @param trader_params Parameters for Trader
             */
            Trader_Abstract(
                nlohmann::json &trader_params
            ) {
                this->pair = (string)trader_params["pair"];
                this->positions = (string)trader_params["positions"];
                this->exchange = (string)trader_params["exchange"];

                this->long_amount = (double)trader_params["long_amount"];
                this->short_amount = (double)trader_params["short_amount"];
                this->max_open_trade = (double)trader_params["max_open_trade"];

                std::remove_copy(
                    this->pair.begin(),
                    this->pair.end(),
                    std::back_inserter(this->erased_pair),
                    '/'
                );

                std::stringstream s(this->pair);
                vector<string> symbols;
                string segment;
                while(std::getline(s, segment, '/'))
                    symbols.push_back(segment);
                this->short_symbol = symbols[0];
                this->long_symbol = symbols[1];

                if ((string)trader_params["type"] == "scalping")
                {
                    this->type = (string)trader_params["type"];
                    this->stop_loss = (double)trader_params["stop-loss"];
                    this->target = (double)trader_params["target"];
                } else if ((string)trader_params["type"] == "position") {
                    this->type = (string)trader_params["type"];
                }
            }

            /**
             * @brief Destroy the Trader_Abstract object
             */
            ~Trader_Abstract() = default;

            /**
             * @brief Get the description object
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->trader_params; }

            /**
             * @brief Get the symbol
             * 
             * @return string 
             */
            string get_pair() { return this->pair; }

            /**
             * @brief Get the exchange 
             * 
             * @return string 
             */
            string get_exchange() { return this->exchange; }

            /**
             * @brief Get the stake amount
             * 
             * @return double 
             */
            double get_long_amount() { return this->long_amount; }
    };
}

/**
 * @brief All Watchers
 */
namespace Workers::Watchers
{
    /**
     * @brief Responsable for Indicators
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class Indicator_Watcher
    {
        private:
            /**
             * @brief All strategies in config
             */
            nlohmann::json strategy_params;

            /**
             * @brief Base URL of taapi.io
             */
            string taapi_url = "https://api.taapi.io/";

            /**
             * @brief API Key for taapi.io
             */
            string taapi_key;

            /**
             * @brief All EMA indicators
             */
            vector<Indicators::Integral::EMA<Candle_T>> all_ema;

            /**
             * @brief All WMA indicators
             */
            vector<Indicators::Integral::WMA<Candle_T>> all_wma;

            /**
             * @brief All SMA indicators
             */
            vector<Indicators::Integral::SMA<Candle_T>> all_sma;

            /**
             * @brief All SSMA indicators
             */
            vector<Indicators::Integral::SSMA<Candle_T>> all_ssma;

            /**
             * @brief All RSI indicators
             */
            vector<Indicators::Integral::RSI<Candle_T>> all_rsi;

            /**
             * @brief All TR indicators
             */
            vector<Indicators::Integral::TR<Candle_T>> all_tr;

            /**
             * @brief All ATR indicators
             */
            vector<Indicators::Integral::ATR<Candle_T>> all_atr;

            /**
             * @brief All DMI indicators
             */
            vector<Indicators::Integral::DMI<Candle_T>> all_dmi;

            /**
             * @brief All ADX indicators
             */
            vector<Indicators::Integral::ADX<Candle_T>> all_adx;

            /**
             * @brief All Normalized_MACD indicators
             */
            vector<Indicators::TradingView::Normalized_MACD<Candle_T>> all_normalized_macd;

            /**
             * @brief All RSXC_LB indicators
             */
            vector<Indicators::TradingView::RSXC_LB<Candle_T>> all_rsxc_lb;

        public:
            /**
             * @brief Construct a new Indicator_Watcher object
             * 
             * @param strategies All strategies in config
             * @param taapi_key API Key for taapi.io
             */
            Indicator_Watcher(
                nlohmann::json &strategy_params,
                const string &taapi_key
            ) : taapi_key(taapi_key), strategy_params(strategy_params)
            { }

            /**
             * @brief Destroy the Indicator_Watcher object
             */
            ~Indicator_Watcher() = default;

            /**
             * @brief Set the taapi key 
             * 
             * @param key API Key for taapi.io
             */
            void set_taapi_key(const string &key) { this->taapi_key = key; }

            /**
             * @brief Initialize indicators 
             */
            void initialize_indicators()
            {
                // TODO: Create instance from config. Without if-else
                for (auto& [key, val] : this->strategy_params.items())
                {
                    if (val["indicator"] == "EMA")
                        this->all_ema.push_back(
                            Indicators::Integral::EMA<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "WMA")
                        this->all_wma.push_back(
                            Indicators::Integral::WMA<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "SMA")
                        this->all_sma.push_back(
                            Indicators::Integral::SMA<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "SSMA")
                        this->all_ssma.push_back(
                            Indicators::Integral::SSMA<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "RSI")
                        this->all_rsi.push_back(
                            Indicators::Integral::RSI<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "TR")
                        this->all_tr.push_back(
                            Indicators::Integral::TR<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "ATR")
                        this->all_atr.push_back(
                            Indicators::Integral::ATR<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "DMI")
                        this->all_dmi.push_back(
                            Indicators::Integral::DMI<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "ADX")
                        this->all_adx.push_back(
                            Indicators::Integral::ADX<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "Normalized_MACD")
                        this->all_normalized_macd.push_back(
                            Indicators::TradingView::Normalized_MACD<Candle_T>(
                                val
                            )
                        );
                    if (val["indicator"] == "RSXC_LB")
                        this->all_rsxc_lb.push_back(
                            Indicators::TradingView::RSXC_LB<Candle_T>(
                                val
                            )
                        );
                }
            }

            /**
             * @brief Describe indicators
             */
            void describe_indicators()
            {
                for (auto& ema : this->all_ema)
                    cout << "EMA: " << ema.get_description() << endl;
                for (auto& sma : this->all_sma)
                    cout << "SMA: " << sma.get_description() << endl;
                for (auto& ssma : this->all_ssma)
                    cout << "SSMA: " << ssma.get_description() << endl;
                for (auto& wma : this->all_wma)
                    cout << "WMA: " << wma.get_description() << endl;
                for (auto& rsi : this->all_rsi) 
                    cout << "RSI: " << rsi.get_description() << endl;
                for (auto& tr : this->all_tr)
                    cout << "TR: " << tr.get_description() << endl;
                for (auto& atr : this->all_atr)
                    cout << "ATR: " << atr.get_description() << endl;
                for (auto& dmi : this->all_dmi)
                    cout << "DMI: " << dmi.get_description() << endl;
                for (auto& adx : this->all_adx)
                    cout << "ADX: " << adx.get_description() << endl;
                for (auto& n_macd : this->all_normalized_macd)
                    cout << "Normalized MACD: " << n_macd.get_description() << endl;
                for (auto& rsx : this->all_rsxc_lb)
                    cout << "RSXC_LB:" << rsx.get_description() << endl;
            }

            /**
             * @brief When next candle is set Watcher should be resolved
             * 
             * @param candle Candle object
             */
            void resolve(Candle_T& candle)
            {
                for (auto& ema : this->all_ema)
                    ema.resolve(candle);
                for (auto& sma : this->all_sma)
                    sma.resolve(candle);
                for (auto& ssma : this->all_ssma)
                    ssma.resolve(candle);
                for (auto& wma : this->all_wma)
                    wma.resolve(candle);
                for (auto& rsi : this->all_rsi) 
                    rsi.resolve(candle);
                for (auto& tr : this->all_tr)
                    tr.resolve(candle);
                for (auto& atr : this->all_atr)
                    atr.resolve(candle);
                for (auto& dmi : this->all_dmi)
                    dmi.resolve(candle);
                for (auto& adx : this->all_adx)
                    adx.resolve(candle);
                for (auto& n_macd : this->all_normalized_macd)
                    n_macd.resolve(candle);
                for (auto& rsx : this->all_rsxc_lb)
                    rsx.resolve(candle);    
            }

            /**
             * @brief Get values for specified indicators
             * 
             * @param strategy_params strategy_params in config
             * @param symbol Symbol (pair) to trade
             * @param backtest If backtest all Indicators will be calculated as Integral
             * @return nlohmann::json
             */
            nlohmann::json get(
                nlohmann::json &strategy_params, 
                const string &symbol,
                bool backtest = false
            )
            {
                nlohmann::json params;

                for (auto& [key, val] : strategy_params.items())
                {
                    string indicator = val["indicator"];
                    string type = val["type"];

                    // TODO: For TAAPI indicators
                    // if (type == "Indicators::TAAPI" && !backtest)
                    // {
                    //     if (indicator == "EMA")
                    //     {
                    //     again:
                    //         try {
                    //             Indicators::TAAPI::EMA ema;
                    //             params[key] = ema.get(
                    //                 this->taapi_url,
                    //                 this->taapi_key, symbol,
                    //                 val["indicator_params"]["interval"], 
                    //                 val["indicator_params"]
                    //             );
                    //         } catch (Exceptions::TAAPI::Rate_Limit& exp) {
                    //             cout << exp.what() << endl;
                    //             // TODO: Pass delay from config
                    //             std::this_thread::sleep_for(
                    //                 std::chrono::seconds(
                    //                     15
                    //                 )
                    //             );
                    //             goto again;
                    //         }
                    //     }
                    // }
                    if (
                        type == "Indicators::Integral" || 
                        backtest
                    )
                    {
                        if (indicator == "EMA")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &ema : this->all_ema)
                                    if (ema.get_description() == val)
                                        params[key] = ema.get();
                        if (indicator == "SMA")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &sma : this->all_sma)
                                    if (sma.get_description() == val)
                                        params[key] = sma.get();
                        if (indicator == "SSMA")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &ssma : this->all_ssma)
                                    if (ssma.get_description() == val)
                                        params[key] = ssma.get();
                        if (indicator == "WMA")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &wma : this->all_wma)
                                    if (wma.get_description() == val)
                                        params[key] = wma.get();
                        if (indicator == "RSI")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &rsi : this->all_rsi)
                                    if (rsi.get_description() == val)
                                        params[key] = rsi.get();
                        if (indicator == "TR")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &tr : this->all_tr)
                                    if (tr.get_description() == val)
                                        params[key] = tr.get();
                        if (indicator == "ATR")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &atr : this->all_atr)
                                    if (atr.get_description() == val)
                                        params[key] = atr.get();
                        if (indicator == "DMI")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &dmi : this->all_dmi)
                                    if (dmi.get_description() == val)
                                        params[key] = dmi.get();
                        if (indicator == "ADX")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &adx : this->all_adx)
                                    if (adx.get_description() == val)
                                        params[key] = adx.get();

                    }
                    if (type == "Indicators::TradingView" || backtest)
                    {
                        if (indicator == "Normalized_MACD")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &n_macd : this->all_normalized_macd)
                                    if (n_macd.get_description() == val)
                                        params[key] = n_macd.get();
                        if (indicator == "RSXC_LB")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &rsx : this->all_rsxc_lb)
                                    if (rsx.get_description() == val)
                                        params[key] = rsx.get();
                    }
                }
                return params;
            }
    };

    /**
     * @brief Responsable for Candles
     */
    template <class Candle_T>
    class Candle_Watcher
    {
        private:
            /**
             * @brief Data file with candles
             */
            std::unique_ptr<io::CSVReader<11>> data_file;

            /**
             * @brief Candle type
             */
            Candle_T candle;

            /**
             * @brief Pair (Symbol)
             */
            string pair;

            /**
             * @brief Pair string without '/' character
             */
            string erased_pair;

            /**
             * @brief Interval (timeframe)
             */
            string interval;

            /**
             * @brief Previous Heikin Ashi open price
             */
            double prev_ha_open = 0.0;

            /**
             * @brief Previous Heikin Ashi close price
             */
            double prev_ha_close = 0.0;

            /**
             * @brief Last candle close time
             */
            unsigned long last_candle_close_time;

            /**
             * @brief Candle duraction in milliseconds
             */
            unsigned long candle_duration;

        public:
            /**
             * @brief Construct a new Candle_Watcher object
             * 
             * @param pair Pair (symbol)
             * @param interval Interval (timeframe)
             * @param data_dir Data directory with candles
             */
            Candle_Watcher(
                const string &pair,
                const string &interval,
                const string &data_dir
            ) : pair(pair), interval(interval)
            { 
                std::remove_copy(
                    this->pair.begin(),
                    this->pair.end(),
                    std::back_inserter(this->erased_pair),
                    '/'
                );

                this->data_file = std::make_unique<io::CSVReader<11>>(
                    data_dir + this->erased_pair + '_' + this->interval + ".csv"
                );

                if (interval == "1d") {
                    this->candle_duration = CANDLE_1D_DURATION;
                } else if (interval == "12h") {
                    this->candle_duration = CANDLE_12H_DURATION;
                } else if (interval == "8h") {
                    this->candle_duration = CANDLE_8H_DURATION;
                } else if (interval == "6h") {
                    this->candle_duration = CANDLE_4H_DURATION;
                } else if (interval == "2h") {
                    this->candle_duration = CANDLE_2H_DURATION;
                } else if (interval == "1h") {
                    this->candle_duration = CANDLE_1H_DURATION;
                } else if (interval == "30m") {
                    this->candle_duration = CANDLE_30M_DURATION;
                } else if (interval == "15m") {
                    this->candle_duration = CANDLE_15M_DURATION;
                } else if (interval == "5m") {
                    this->candle_duration = CANDLE_5M_DURATION;
                } else if (interval == "1m") {
                    this->candle_duration = CANDLE_1M_DURATION;
                }
            }

            /**
             * @brief Destroy the Candle_Watcher object
             */
            ~Candle_Watcher() = default;

            /**
             * @brief Get the pair object
             * 
             * @return string 
             */
            string get_pair() { return this->pair; }

            /**
             * @brief Get the interval object
             * 
             * @return string 
             */
            string get_interval() { return this->interval; }

            /**
             * @brief Get the candle object
             * 
             * @return Candle_T 
             */
            Candle_T get_candle() { return this->candle; }

            /**
             * @brief Get the last candle close time object
             * 
             * @return unsigned long 
             */
            unsigned long get_last_candle_close_time() { return this->last_candle_close_time; }

            /**
             * @brief Get the candle duration object
             * 
             * @return unsigned long 
             */
            unsigned long get_candle_duration() { return this->candle_duration; }

            /**
             * @brief Set the last candle close time object
             * 
             * @param close_time Candle close time
             */
            void set_last_candle_close_time(unsigned long close_time) { this->last_candle_close_time = close_time; }

            /**
             * @brief Overload "==" operator
             * 
             * @param cw Candle Watcher to compare
             * @return true 
             * @return false 
             */
            bool operator ==(const Workers::Watchers::Candle_Watcher<Candle_T>& cw)
            {
                if (
                    cw.pair == this->pair && 
                    cw.interval == this->interval
                )
                    return true;
                return false;
            }

            /**
             * @brief Read file once to update candles
             */
            bool read_file_once()
            {
                string open_price, high_price, low_price, close_price,
                    volume, quote, trades_count, tbbav, tbqav,
                    open_time, close_time
                ;
                if (
                    this->data_file->read_row(
                        open_time, open_price, high_price, 
                        low_price, close_price, volume, close_time, 
                        quote, trades_count, tbbav, tbqav
                    )
                ) {
                    // TODO: Remake without try-catch
                    try {
                        if (this->prev_ha_close == 0 || prev_ha_open == 0)
                        {
                            this->candle.construct(
                                std::stoul(open_time),
                                std::stoul(close_time),
                                std::stod(open_price),
                                std::stod(high_price),
                                std::stod(low_price),
                                std::stod(close_price),
                                std::stod(volume),
                                std::stod(quote),
                                std::stod(trades_count),
                                std::stod(tbbav),
                                std::stod(tbqav),
                                std::stod(close_price),
                                std::stod(open_price)
                            );
                        } else {
                            this->candle.construct(
                                std::stoul(open_time),
                                std::stoul(close_time),
                                std::stod(open_price),
                                std::stod(high_price),
                                std::stod(low_price),
                                std::stod(close_price),
                                std::stod(volume),
                                std::stod(quote),
                                std::stod(trades_count),
                                std::stod(tbbav),
                                std::stod(tbqav),
                                this->prev_ha_close,
                                this->prev_ha_open
                            );
                        }

                        this->prev_ha_close = this->candle.get_close_price();
                        this->prev_ha_open = this->candle.get_open_price();

                        this->last_candle_close_time = this->candle.get_close_time();

                        return true;
                    } catch (std::invalid_argument& exp) {
                        return false;
                    }
                } else {
                    return false;
                }
            }

            /**
             * @brief Get the new candles for interval
             * 
             * @param startTime Start time in milliseconds linux epoch
             * @param endTime End time in miliseconds linux epoch
             */
            vector<Candle_T> get_historical_klines(unsigned long startTime, unsigned long endTime)
            {
                map<string, string> params;

                params["symbol"] = this->erased_pair;
                params["interval"] = this->interval;
                params["startTime"] = std::to_string(startTime);
                params["endTime"] = std::to_string(endTime);
                params["limit"] = "1000";

                Request::Simple::JSON_Curl json_curl("https://api.binance.com/api/v3/klines");

                json_curl.construct_request(params);

                nlohmann::json ret = json_curl.request();

                vector<Candle_T> candles;

                for (auto& [key, val] : ret.items())
                {
                    unsigned long open_time = val[0];
                    double open = std::stod((string)val[1]);
                    double high = std::stod((string)val[2]);
                    double low = std::stod((string)val[3]);
                    double close = std::stod((string)val[4]);
                    double volume = std::stod((string)val[5]);
                    unsigned long close_time = val[6];
                    double quote = std::stod((string)val[7]);
                    double trades = val[8];
                    double tbbav = std::stod((string)val[9]);
                    double tbqav = std::stod((string)val[10]);

                    Candle_T candle;
                    candle.construct(
                        open_time,
                        close_time,
                        open,
                        high,
                        low,
                        close,
                        volume,
                        quote,
                        trades,
                        tbbav,
                        tbqav,
                        this->prev_ha_close,
                        this->prev_ha_open                       
                    );
                    candles.push_back(candle);
                }

                candles.pop_back();

                return candles;
            }

            /**
             * @brief Get the current price of cryptocurrency
             * 
             * @return double 
             */
            double get_current_price_online()
            {
                map<string, string> params;

                params["symbol"] = this->erased_pair;

                Request::Simple::JSON_Curl json_curl("https://api.binance.com/api/v3/ticker/price");

                json_curl.construct_request(params);

                nlohmann::json res = json_curl.request();

                if (res.contains("price"))
                {
                    string price = res["price"];
                    return std::stod(price);
                }
                
                throw Exceptions::Panic::Panic_Exception("Something went wront in trader!", 1, 0);
            }
    };
}

/**
 * @brief All implementors to construct new Worker
 */
namespace Workers::Implementors
{
    /**
     * @brief Trader object for worker backtesting
     */
    class Backtest_Trader : virtual public Workers::Abstract::Trader_Abstract
    {
        private:
            /**
             * @brief CSV File with backtest statistic
             */
            std::ofstream backtest_file;

            /**
             * @brief Current working trades
             */
            vector<Trade> current_trades;

            /**
             * @brief Executed trades
             */
            vector<Trade> executed_trades;

            /**
             * @brief Absolute profit of long trades
             */
            double abs_lng_prft = 0;

            /**
             * @brief Percentage profit of long trades
             */
            double prcntg_lng_prft = 0;

            /**
             * @brief Average absolute profit per long trades
             */
            double avrg_abs_prft_pr_lng_trd = 0;

            /**
             * @brief Average percentage profit per long trades
             */
            double avrg_prcntg_prft_pr_lng_trd = 0;

            /**
             * @brief Absolute profit of short trades
             */
            double abs_shrt_prft = 0;

            /**
             * @brief Percentage profit of short trades
             */
            double prcntg_shrt_prft = 0;

            /**
             * @brief Average absolute profit per short trades
             */
            double avrg_abs_prft_pr_shrt_trd = 0;

            /**
             * @brief Average percentage profit per short trades
             */
            double avrg_prcntg_prft_pr_shrt_trd = 0;

            /**
             * @brief Percent of wins long trades
             */
            double prcnt_wins_long = 0;

            /**
             * @brief Percent of wins short trades
             */
            double prcnt_wins_short = 0;

            /**
             * @brief The best long trade
             */
            double best_long_trade = 0.0;

            /**
             * @brief The worst long trade
             */
            double worst_long_trade = 0.0;

            /**
             * @brief The best short trade
             * 
             */
            double best_short_trade = 0.0;

            /**
             * @brief The worst short trade
             */
            double worst_short_trade = 0.0;

            /**
             * @brief Minimal long balance
             */
            double mnml_lng_blnc = 0;

            /**
             * @brief Maximum long balance
             */
            double mxmm_lng_blnc = 0;

            /**
             * @brief Minimal short balance
             */
            double mnml_shrt_blnc = 0;

            /**
             * @brief Maximum short balance
             */
            double mxmm_shrt_blnc = 0;

            /**
             * @brief Total long trades
             */
            int ttl_lng_trds = 0;

            /**
             * @brief Total wins long trades
             */
            int wins_long = 0;

            /**
             * @brief Total loses long trades
             */
            int loses_long = 0;

            /**
             * @brief Total short trades
             */
            int ttl_shrt_trds = 0;

            /**
             * @brief Total wins short trades
             */
            int wins_short = 0;

            /**
             * @brief Total loses short trades
             */
            int loses_short = 0;

            /**
             * @brief Initialize Trade as opened
             * 
             * @param trade Trade object
             * @param position Trade position (long/short)
             * @param price Open price
             */
            void open_trade(
                Trade &trade,
                const string& position,
                double price
            )
            {
                // TODO: Unique ID for each Trade
                trade.set_open_time(
                    1, 
                    this->pair, 
                    position, 
                    this->long_amount, 
                    this->short_amount, 
                    price 
                );
            }

            /**
             * @brief Initialize trade as closed
             * 
             * @param trade Trade object
             * @param price Close price
             */
            void close_trade(
                Trade &trade,
                double price
            )
            {
                if (trade.get_position() == "short")
                    trade.set_close_time(
                        price
                    );
                if (trade.get_position() == "long")
                    trade.set_close_time(
                        price
                    );
            }

            /**
             * @brief Update all balances and statistic
             * 
             * @param trade Trade object
             * @param verbose More information about trades
             */
            void update_statistic(Trade& trade)
            {
                if (trade.get_position() == "long")
                {
                    this->long_balance += trade.get_abs_profit();

                    if (this->best_long_trade == 0.0)
                    {
                        this->best_long_trade = trade.get_per_profit();
                    } else {
                        if (this->best_long_trade <= trade.get_per_profit())
                            this->best_long_trade = trade.get_per_profit();
                    }

                    if (this->worst_long_trade == 0.0)
                    {
                        this->worst_long_trade = trade.get_per_profit();
                    } else {
                        if (this->worst_long_trade >= trade.get_per_profit())
                            this->worst_long_trade = trade.get_per_profit();
                    }

                    if (this->long_balance > this->mxmm_lng_blnc)
                        this->mxmm_lng_blnc = this->long_balance;
                    if (this->long_balance < this->mnml_lng_blnc)
                        this->mnml_lng_blnc = this->long_balance;

                    this->avrg_abs_prft_pr_lng_trd += trade.get_abs_profit();
                    this->avrg_prcntg_prft_pr_lng_trd += trade.get_per_profit();

                    this->ttl_lng_trds++;

                    if ((trade.get_per_profit() + 100) / 100 >= 1)
                    {
                        this->wins_long++;
                    } else {
                        this->loses_long++;
                    }
                } else if (trade.get_position() == "short") {
                    this->short_balance += trade.get_abs_profit();

                    if (this->short_balance > this->mxmm_shrt_blnc)
                        this->mxmm_shrt_blnc = short_balance;

                    if (this->short_balance < this->mnml_shrt_blnc)
                        this->mnml_shrt_blnc = this->short_balance;

                    if (this->best_short_trade == 0.0)
                    {
                        this->best_short_trade = trade.get_per_profit();
                    } else {
                        if (this->best_short_trade <= trade.get_per_profit())
                            this->best_short_trade = trade.get_per_profit();
                    }

                    if (this->worst_short_trade == 0.0) 
                    {
                        this->worst_short_trade = trade.get_per_profit();
                    } else {
                        if (this->worst_short_trade >= trade.get_per_profit())
                            this->worst_short_trade = trade.get_per_profit();
                    }

                    this->avrg_abs_prft_pr_shrt_trd += trade.get_abs_profit();
                    this->avrg_prcntg_prft_pr_shrt_trd += trade.get_per_profit();

                    this->ttl_shrt_trds++;

                    if ((trade.get_per_profit() + 100) / 100 >= 1)
                    {
                        this->wins_short++;
                    } else {
                        this->loses_short++;
                    }
                }
                this->long_amount = this->long_balance / this->max_open_trade;
                this->short_amount = this->short_balance / this->max_open_trade;

                this->backtest_file << this->long_balance << ',' << this->short_balance << endl;

                if (trade.get_position() == "long")
                    cout 
                        << "Position: " << trade.get_position() << endl
                        << "Absolute profit: " << trade.get_abs_profit() << " " + this->long_symbol << endl
                        << "Percentage profit: " << trade.get_per_profit() << " %" << endl
                        << "Open price: " << trade.get_open_price() << " $" << endl
                        << "Close price: " << trade.get_close_price() << " $" << endl
                        << "Long amount: " << trade.get_stake_amount() << " " + this->long_symbol << endl
                        << "Current long balance: " << this->long_balance << " " + this->long_symbol << endl
                        << "Current short balance: " << this->short_balance << " " + this->short_symbol << endl
                    << endl;
                if (trade.get_position() == "short")
                    cout 
                        << "Position: " << trade.get_position() << endl
                        << "Absolute profit: " << trade.get_abs_profit() << " " + this->short_symbol << endl
                        << "Percentage profit: " << trade.get_per_profit() << " %" << endl
                        << "Open price: " << trade.get_open_price() << " $" << endl
                        << "Close price: " << trade.get_close_price() << " $" << endl
                        << "Short amount: " << trade.get_stake_amount() << " " + this->short_symbol << endl
                        << "Current long balance: " << this->long_balance << " " + this->long_symbol << endl
                        << "Current short balance: " << this->short_balance << " " + this->short_symbol << endl
                    << endl;
            }

        public:
            /**
             * @brief Construct a new Backtest_Trader object
             * 
             * @param trader_params Parameters for trader
             * @param interval Interval
             * @param long_balance Long start balance (typically USDT)
             * @param short_balance Short start balance (typically Symbol)
             */
            Backtest_Trader(
                nlohmann::json &trader_params,
                string &interval,
                double long_balance,
                double short_balance
            ) : Trader_Abstract(trader_params)
            { 
                string filename = "../backtest_results/" + this->erased_pair + '_' + interval + ".csv";
                this->backtest_file.open(filename);

                this->long_balance = long_balance;
                this->long_start_balance = long_balance;
                this->mnml_lng_blnc = long_balance;
                this->mxmm_lng_blnc = long_balance;

                this->short_balance = short_balance;
                this->short_start_balance = short_balance;
                this->mnml_shrt_blnc = short_balance;
                this->mxmm_shrt_blnc = short_balance;
            }

            /**
             * @brief Destroy the Backtest_Trader object
             */
            virtual ~Backtest_Trader()
            {
                this->backtest_file.close();
            }

            /**
             * @brief Set the stake amount 
             * 
             * @param amount Stake amount
             */
            void set_long_amount(double amount) { this->long_amount = amount; }

            /**
             * @brief Set the symbol abount object
             * 
             * @param amount Stake amount
             */
            void set_short_abount(double amount) { this->short_amount = amount; }

            /**
             * @brief Decide what to do with trade
             * 
             * @param signals 
             * @param price Current price
             */
            void resolve(
                map<string, bool> &signals,
                double price
            )
            {
                if (this->type == "position")
                {
                    if (
                        signals["long_open"] &&
                        (
                            this->positions == "both" ||
                            this->positions == "long"
                        )
                    ) {
                        if (
                            this->current_trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade, 
                                (string)"long",
                                price
                            );
                            this->current_trades.push_back(trade);
                        }
                    }
                    if (signals["long_close"])
                    {
                        for (Trade& trade : this->current_trades)
                            if (trade.get_position() == "long")
                            {
                                this->close_trade(trade, price);

                                this->update_statistic(trade);

                                this->executed_trades.push_back(trade);

                                this->current_trades.erase(
                                    std::remove(
                                        this->current_trades.begin(),
                                        this->current_trades.end(),
                                        trade
                                    ),
                                    this->current_trades.end()
                                );
                            }
                    }
                    if (
                        signals["short_open"] &&
                        (
                            this->positions == "both" ||
                            this->positions == "short"
                        )
                    ) {
                        if (
                            this->current_trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade,
                                (string)"short",
                                price
                            );
                            this->current_trades.push_back(trade);
                        }
                    }
                    if (signals["short_close"])
                    {
                        for (Trade& trade : this->current_trades)
                            if (trade.get_position() == "short")
                            {
                                this->close_trade(trade, price);

                                this->update_statistic(trade);

                                this->executed_trades.push_back(trade);

                                this->current_trades.erase(
                                    std::remove(
                                        this->current_trades.begin(),
                                        this->current_trades.end(),
                                        trade
                                    ),
                                    this->current_trades.end()
                                );
                            }
                    }
                } else if (this->type == "scalping") {
                    double opened_price;
                    double percent;
                    for (Trade& trade : this->current_trades)
                    {
                        opened_price = trade.get_open_price();
                        if (trade.get_position() == "long")
                        {
                            percent = (price / opened_price - 1) * 100;
                            if (
                                this->target <= percent ||
                                this->stop_loss >= percent
                            ) {
                                this->close_trade(trade, price);

                                this->update_statistic(trade);

                                this->executed_trades.push_back(trade);

                                this->current_trades.erase(
                                    std::remove(
                                        this->current_trades.begin(),
                                        this->current_trades.end(),
                                        trade
                                    ),
                                    this->current_trades.end()
                                );
                            }
                        } else if (trade.get_position() == "short") {
                            percent = (opened_price / price - 1) * 100;
                            if (
                                this->target <= percent ||
                                this->stop_loss >= percent
                            ) {
                                this->close_trade(trade, price);

                                this->update_statistic(trade);

                                this->executed_trades.push_back(trade);

                                this->current_trades.erase(
                                    std::remove(
                                        this->current_trades.begin(),
                                        this->current_trades.end(),
                                        trade
                                    ),
                                    this->current_trades.end()
                                );
                            }
                        }
                    }
                    if (
                        signals["long_open"] &&
                        (
                            this->positions == "both" ||
                            this->positions == "long"
                        )
                    ) {
                        if (
                            this->current_trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade,
                                (string)"long",
                                price
                            );
                            this->current_trades.push_back(trade);
                        }
                    }
                    if (
                        signals["short_open"] &&
                        (
                            this->positions == "both" ||
                            this->positions == "short"
                        )
                    ) {
                        if (
                            this->current_trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade, 
                                (string)"short",
                                price
                            );
                            this->current_trades.push_back(trade);
                        }
                    }
                }
            }

            /**
             * @brief Prepare statistic
             */
            void initialize_statistic()
            {
                this->prcnt_wins_long = (double)this->wins_long / (double)this->ttl_lng_trds * 100;
                this->prcnt_wins_short = (double)this->wins_short / (double)this->ttl_shrt_trds * 100;

                this->abs_lng_prft = this->long_balance - this->long_start_balance;
                this->abs_shrt_prft = this->short_balance - this->short_start_balance;

                this->prcntg_lng_prft = this->long_balance / this->long_start_balance * 100 - 100;
                this->prcntg_shrt_prft = this->short_balance / this->short_start_balance * 100 - 100;

                this->avrg_abs_prft_pr_lng_trd /= this->ttl_lng_trds;
                this->avrg_prcntg_prft_pr_lng_trd /= this->ttl_lng_trds;

                this->avrg_abs_prft_pr_shrt_trd /= this->ttl_shrt_trds;
                this->avrg_prcntg_prft_pr_shrt_trd /= this->ttl_shrt_trds;
            }

            /**
             * @brief Print statistic
             */
            void print_statistic()
            {
                cout 
                    << "----------------------------------" << endl
                    << "LONG POSITION STATISTIC:" << endl
                    << endl
                    << "BALANCE:" << endl
                    << "Start Balance: " << this->long_start_balance << " USDT" << endl
                    << "Final Balance: " << this->long_balance << " USDT" << endl 
                    << "Maximum balance: " << this->mxmm_lng_blnc << " USDT"  << endl
                    << "Minimal balance: " << this->mnml_lng_blnc << " USDT" << endl
                    << endl
                    << "PROFIT:" << endl
                    << "Absolute profit: " << this->abs_lng_prft << " USDT" << endl
                    << "Percentage profit: " << this->prcntg_lng_prft << " %" << endl
                    << "Average absolute profit per trades: " << this->avrg_abs_prft_pr_lng_trd << " USDT" << endl
                    << "Average percentage profit per trades: " << this->avrg_prcntg_prft_pr_lng_trd << " %" << endl
                    << endl
                    << "TRADE" << endl
                    << "Total trades: " << this->ttl_lng_trds << endl
                    << "Wins: " << this->wins_long << endl
                    << "Loses: " << this->loses_long << endl
                    << "Percent wins: " << this->prcnt_wins_long << " %" << endl
                    << "Best long trade: " << this->best_long_trade << " %" << endl
                    << "Worst long trade: " << this->worst_long_trade << " %" << endl
                    << "----------------------------------" << endl
                    << "SHORT POSITION STATISTIC:" << endl
                    << endl
                    << "BALANCE:" << endl
                    << "Start Balance: " << this->short_start_balance << " Symbols" << endl
                    << "Final Balance: " << this->short_balance << " Symbols" << endl 
                    << "Maximum balance: " << this->mxmm_shrt_blnc << " Symbols"  << endl
                    << "Minimal balance: " << this->mnml_shrt_blnc << " Symbols" << endl
                    << endl
                    << "PROFIT:" << endl
                    << "Average absolute profit per trades: " << this->avrg_abs_prft_pr_shrt_trd << " Symbols" << endl
                    << "Average percentage profit per trades: " << this->avrg_prcntg_prft_pr_shrt_trd << " %" << endl
                    << endl
                    << "TRADE" << endl
                    << "Total trades: " << this->ttl_shrt_trds << endl
                    << "Wins: " << this->wins_short << endl
                    << "Loses: " << this->loses_short << endl
                    << "Percent wins: " << this->prcnt_wins_short << " %" << endl
                    << "Best long trade: " << this->best_short_trade << " %" << endl
                    << "Worst long trade: " << this->worst_short_trade << " %" << endl
                    << "----------------------------------"
                << endl;
            }
    };

    /**
     * @brief Trader decides when open and close order
     */
    class Online_Trader : virtual public Workers::Abstract::Trader_Abstract
    {
        private:
            /**
             * @brief Pointer to Binance_API Object
             */
            std::unique_ptr<Exchanges::Binance::Binance_API> binapi;

            /**
             * @brief Response from Stock Market
             */
            nlohmann::json response;

            /**
             * @brief Quantities of long trade (in Symbol)
             */
            vector<double> long_quantities;

            /**
             * @brief Quantities of short trade (in USDT)
             */
            vector<double> short_quantities;

            /**
             * @brief Quantity in string
             */
            string str_quantity;

            /**
             * @brief Quantity in order
             */
            int quantity;

            /**
             * @brief Step Size for open Trade (LOT_SIZE Binance API Error)
             */
            double stepSize;

            /**
             * @brief Minimal Notional for open Trade (MIN_NOTIONAL Binance API Error)
             */
            double minNotional;

            /**
             * @brief Set the up amounts to open Trade
             */
            void binapi_setup_amounts()
            {
                cout << "[+] Start setup amounts" << endl;

                string fs_balance = this->binapi->get_balance(this->long_symbol)["free"];
                string ss_balance = this->binapi->get_balance(this->short_symbol)["free"];

                this->long_balance = std::stod(fs_balance);
                this->short_balance = std::stod(ss_balance);

                if (this->long_balance >= this->minNotional)
                {
                    this->long_amount = (int)(this->long_balance / this->max_open_trade);
                    if (this->long_amount <= this->minNotional)
                        this->long_amount = this->minNotional;
                } else {
                    this->long_amount = 0;
                }
                if (this->short_balance >= this->minNotional)
                {
                    this->short_amount = (int)(this->short_balance / this->max_open_trade);
                    if (this->short_amount <= this->minNotional)
                        this->short_amount = this->minNotional;
                } else {
                    this->short_amount = 0;
                }

                cout
                    << "    Long amount: " << this->long_amount << endl
                    << "    Short amount: " << this->short_amount
                << endl;
            }

            /**
             * @brief Initialize Binapi
             */
            void binapi_initialize()
            {
                cout << "[+] Start initializing trading parameters" << endl;

                nlohmann::json ex_in = this->binapi->exchange_info(
                    this->erased_pair
                )["symbols"][0]["filters"];

                this->stepSize = std::stod((string)ex_in[2]["stepSize"]);
                this->minNotional = std::stod((string)ex_in[3]["minNotional"]);

                this->binapi_setup_amounts();

                cout
                    << "[+] Binance status" << endl
                    << "    Long balance: " << this->long_balance << ' ' << this->long_symbol << endl
                    << "    Short: " << this->short_balance << ' ' << this->short_symbol << endl
                    << "    stepSize: " << this->stepSize << endl
                    << "    minNotional: " << this->minNotional
                << endl;
            }

        public:
            /**
             * @brief Construct a new ema cross trader object
             * 
             * @param trader_params Parameters for Trader
             * @param exchange_params Parameters for Exchanges
             */
            Online_Trader(
                nlohmann::json &trader_params,
                nlohmann::json &exchange_params
            ) : Trader_Abstract(trader_params)
            {
                if (this->exchange == "binance")
                {
                    this->binapi = std::make_unique<Exchanges::Binance::Binance_API>(
                        (string)exchange_params["binance"]["private_key"],
                        (string)exchange_params["binance"]["secret_key"]
                    );
                    this->binapi_initialize();
                }
            }

            /**
             * @brief Destroy the Trader object
             */
            virtual ~Online_Trader() = default;

            /**
             * @brief Resolve for real money
             * 
             * @param signals Signals
             */
            void resolve(
                map<string, bool> &signals
            ) {
                // TODO: scalping
                if (
                    signals["long_open"] &&
                    (
                        this->positions == "both" ||
                        this->positions == "long"
                    ) &&
                    this->long_amount > this->minNotional
                ) {
                    this->response = this->binapi->open_new_order(
                        this->erased_pair,
                        "BUY",
                        std::to_string(this->long_amount)
                    );

                    cout
                        << "[+] New executed order" << endl
                        << this->response << endl
                    << endl;

                    this->str_quantity = response["executedQty"];
                    this->quantity = (int)std::stod(this->str_quantity);
                    this->long_quantities.push_back(this->quantity);
                }
                if (signals["long_close"])
                {
                    for (auto& key : this->long_quantities)
                    {
                        this->response = this->binapi->open_new_order(
                            this->erased_pair,
                            "SELL",
                            std::to_string(key)
                        );
                        cout << this->response << endl;
                    }
                    this->long_quantities.clear();
                    this->binapi_setup_amounts();
                }
                if (
                    signals["short_open"] &&
                    (
                        this->positions == "both" ||
                        this->positions == "short"
                    ) &&
                    this->short_amount > this->minNotional
                ) {
                    this->response = this->binapi->open_new_order(
                        this->erased_pair,
                        "SELL",
                        std::to_string(this->short_amount)
                    );

                    cout
                        << "[+] New executed order" << endl
                        << this->response << endl
                    << endl;

                    this->str_quantity = response["cummulativeQuoteQty"];
                    this->quantity = (int)std::stod(this->str_quantity);
                    this->short_quantities.push_back(this->quantity);
                }
                if (signals["short_close"])
                {
                    for (auto& key : this->short_quantities)
                    {
                        response = this->binapi->open_new_order(
                            this->erased_pair,
                            "BUY",
                            std::to_string(key)
                        );
                        cout << response << endl;
                    }
                    this->short_quantities.clear();
                    this->binapi_setup_amounts();
                }
            }
    };

    /**
     * @brief Solves the Strategy
     */
    template <class Strategy, class Candle_T>
    class Strateger
    {
        private:
            /**
             * @brief Strategy
             */
            Strategy strategy;

            /**
             * @brief Params for Strategy
             */
            nlohmann::json strategy_params;

            /**
             * @brief Resolved Indicators for strategy
             */
            nlohmann::json params;

            /**
             * @brief Data container for resolved signals
             */
            map<string, bool> signals{
                {"long_open", false}, 
                {"long_close", false},
                {"short_open", false},
                {"short_close", false}
            };

            /**
             * @brief Indicators for strategy
             */
            std::unique_ptr<Workers::Watchers::Indicator_Watcher<Candle_T>> indicator_watcher;

        public:
            /**
             * @brief Construct a new Solver object
             * 
             * @param strategy_params Params for Strategy
             */
            Strateger(
                nlohmann::json &strategy_params
            ) : strategy_params(strategy_params)
            { 
                this->indicator_watcher = std::make_unique<Workers::Watchers::Indicator_Watcher<Candle_T>>(strategy_params, "");

                this->indicator_watcher->initialize_indicators();
            }

            /**
             * @brief Destroy the ema cross 15m object
             */
            ~Strateger() = default;

            /**
             * @brief Get the strategy params object
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_strategy_params() { return this->strategy_params; }

            /**
             * @brief Get the Resolved indicators
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_params() { return this->params; }

            /**
             * @brief Get the signals object
             * 
             * @return map<string, bool> 
             */
            map<string, bool> get_signals() { return this->signals; }

            /**
             * @brief Resolve the strategy in worker
             */
            void resolve(Candle_T &candle)
            {
                this->indicator_watcher->resolve(candle);
                this->params = this->indicator_watcher->get(
                    this->strategy_params,
                    ""
                );
                this->params["close"] = (double)candle.get_close_price();
                this->strategy.resolve(
                    this->params,
                    this->signals
                );
            }
    };
}

/**
 * @brief All Workers
 */
namespace Workers::Customs
{
    /**
     * @brief Trading Worker for custom strategies
     * 
     * @tparam Strategy Strategy type
     * @tparam Candle_T Candle type
     */
    template <class Strategy, class Candle_T>
    class Worker : virtual public Workers::Abstract::Worker_Abstract
    {
        private:            
            /**
             * @brief Online trader
             */
            std::unique_ptr<Workers::Implementors::Online_Trader> online_trader;

            /**
             * @brief Strateger object
             */
            std::unique_ptr<Workers::Implementors::Strateger<Strategy, Candle_T>> strateger;

            /**
             * @brief Candle Watcher object
             */
            std::unique_ptr<Workers::Watchers::Candle_Watcher<Candle_T>> candle_watcher;

            /**
             * @brief Initialize indicators with data from file
             * 
             * @param strat Strateger
             * @param can_wtch Candle watcher
             */
            void file_initialize(
                std::unique_ptr<Workers::Implementors::Strateger<Strategy, Candle_T>> &strat,
                std::unique_ptr<Workers::Watchers::Candle_Watcher<Candle_T>> &can_wtch
            )
            {
                cout << "[+] Start initializing from file" << endl;

                Candle_T candle;

                can_wtch->read_file_once();
                while(can_wtch->read_file_once())
                {
                    candle = can_wtch->get_candle();
                    strat->resolve(candle);
                }
            }

            /**
             * @brief Initialize indicators with the newest candles
             */
            void newest_initialize(
                std::unique_ptr<Workers::Implementors::Strateger<Strategy, Candle_T>> &strat,
                std::unique_ptr<Workers::Watchers::Candle_Watcher<Candle_T>> &can_wtch
            )
            {
                cout << "[+] Start initializing with the newest candle data" << endl;

                uint64_t now_epoch;

                vector<Candle_T> candles;

                unsigned long candle_close_time = can_wtch->get_last_candle_close_time();
                unsigned long candle_duration = can_wtch->get_candle_duration();

                while(now_epoch - candle_close_time >= candle_duration) 
                {
                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    candles = can_wtch->get_historical_klines(
                        can_wtch->get_last_candle_close_time(),
                        now_epoch
                    );
                    for (Candle_T &candle : candles)
                    {
                        candle_close_time = candle.get_close_time();

                        can_wtch->set_last_candle_close_time(
                            candle_close_time
                        );

                        strat->resolve(candle);

                        if (now_epoch - candle.get_open_time() <= can_wtch->get_candle_duration())
                            return;
                    }
                }
            }

        public:
            /**
             * @brief Construct a new Worker object
             * 
             * @param worker_configuration JSON Worker Configuration
             * @param exchange Exchanges to trade
             * @param data_dir Path to data directory
             */
            Worker(
                nlohmann::json &worker_configuration,
                nlohmann::json &exchange,
                const string &data_dir
            ) : Worker_Abstract(worker_configuration, data_dir)
            { 
                this->online_trader 
                = std::make_unique<Workers::Implementors::Online_Trader>
                (
                    worker_configuration["trader_params"],
                    exchange
                );
                this->strateger
                = std::make_unique<Workers::Implementors::Strateger<Strategy, Candle_T>>
                (
                    this->worker_configuration["strategy_params"]
                );
                this->candle_watcher
                = std::make_unique<Workers::Watchers::Candle_Watcher<Candle_T>>
                (
                    this->worker_configuration["trader_params"]["pair"],
                    this->worker_configuration["interval"],
                    this->data_dir
                );

                this->file_initialize(
                    this->strateger,
                    this->candle_watcher
                );
            }

            /**
             * @brief Destroy the Worker object
             */
            virtual ~Worker() = default;

            /**
             * @brief Online backtesting
             * 
             * @param long_balance Start balance for long trades
             * @param short_balance Start balance for short trades
             */
            void online_backtest(
                double long_balance,
                double short_balance
            )
            {
                cout << "[+] Start online backtesting" << endl;

                std::unique_ptr<Workers::Implementors::Backtest_Trader> backtest_trader 
                = std::make_unique<Workers::Implementors::Backtest_Trader>
                (
                    worker_configuration["trader_params"],
                    this->interval,
                    long_balance,
                    short_balance
                );

                std::unique_ptr<Workers::Implementors::Strateger<Strategy, Candle_T>> backtest_strateger
                = std::make_unique<Workers::Implementors::Strateger<Strategy, Candle_T>>
                (
                    this->worker_configuration["strategy_params"]
                );

                std::unique_ptr<Workers::Watchers::Candle_Watcher<Candle_T>> backtest_candle_watcher
                = std::make_unique<Workers::Watchers::Candle_Watcher<Candle_T>>
                (
                    this->worker_configuration["trader_params"]["pair"],
                    this->worker_configuration["interval"],
                    this->data_dir
                );

                unsigned long candle_close_time;

                uint64_t now_epoch;

                vector<Candle_T> candles;

                map<string, bool> signals;

                this->file_initialize(
                    backtest_strateger,
                    backtest_candle_watcher
                ); cout << "[+] Finished initializing from file" << endl;
                this->newest_initialize(
                    backtest_strateger,
                    backtest_candle_watcher
                ); cout << "[+] Finished initializing with the newest candle data" << endl;

                cout << "[+] Online trading started" << endl;

                now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();

                std::this_thread::sleep_for(std::chrono::milliseconds(
                    backtest_candle_watcher->get_last_candle_close_time() + 1 +
                    backtest_candle_watcher->get_candle_duration() -
                    now_epoch
                ));

                while (1)
                {
                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    candles = backtest_candle_watcher->get_historical_klines(
                        backtest_candle_watcher->get_last_candle_close_time(), 
                        now_epoch
                    );

                    for (Candle_T& candle : candles)
                    {
                        candle_close_time = candle.get_close_time();

                        backtest_candle_watcher->set_last_candle_close_time(
                            candle_close_time
                        );

                        backtest_strateger->resolve(candle);

                        cout
                            << "[+] Got new Candle" << endl
                            << "    Candle open time: " << candle.get_open_time() << endl
                            << "    Candle open price: " << candle.get_open_price() << endl
                            << "    Candle close time: " << candle.get_close_time() << endl
                            << "    Candle close price: " << candle.get_close_price() << endl
                            << "    Params: " << backtest_strateger->get_params() << endl
                        << endl;
                    }

                    signals = backtest_strateger->get_signals();

                    backtest_trader->resolve(signals, backtest_candle_watcher->get_current_price_online());

                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    std::this_thread::sleep_for(std::chrono::milliseconds(
                        backtest_candle_watcher->get_last_candle_close_time() + 1501 + 
                        backtest_candle_watcher->get_candle_duration() - now_epoch
                    ));
                }
            }

            /**
             * @brief Backtest Worker
             * 
             * @param long_balance Start balance for long trades
             * @param short_balance Start balance for short trades
             */
            void file_backtest(
                double long_balance,
                double short_balance 
            ) {
                std::unique_ptr<Workers::Implementors::Backtest_Trader> backtest_trader 
                = std::make_unique<Workers::Implementors::Backtest_Trader>
                (
                    worker_configuration["trader_params"],
                    this->interval,
                    long_balance,
                    short_balance
                );

                std::unique_ptr<Workers::Implementors::Strateger<Strategy, Candle_T>> backtest_strateger
                = std::make_unique<Workers::Implementors::Strateger<Strategy, Candle_T>>
                (
                    this->worker_configuration["strategy_params"]
                );

                std::unique_ptr<Workers::Watchers::Candle_Watcher<Candle_T>> backtest_candle_watcher
                = std::make_unique<Workers::Watchers::Candle_Watcher<Candle_T>>
                (
                    this->worker_configuration["trader_params"]["pair"],
                    this->worker_configuration["interval"],
                    this->data_dir
                );

                map<string, bool> signals;

                Candle_T candle;

                double candle_close_price;

                auto start_time = std::chrono::high_resolution_clock::now();

                backtest_candle_watcher->read_file_once();
                while(backtest_candle_watcher->read_file_once())
                {
                    candle = backtest_candle_watcher->get_candle();

                    backtest_strateger->resolve(candle);

                    signals = backtest_strateger->get_signals();

                    candle_close_price = candle.get_close_price();

                    backtest_trader->resolve(
                        signals,
                        candle_close_price
                    );
                }

                backtest_trader->initialize_statistic();

                backtest_trader->print_statistic();
            
                auto end_time = std::chrono::high_resolution_clock::now();
                auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

                cout << "Test time (ms): " << exec_time.count() << endl;
                cout << "Test time (s): " << (double)exec_time.count() / 1000 << endl;
                cout << "Test time (m): " << (double)exec_time.count() / 60000 << endl;
            }

            /**
             * @brief Start online trading for Real money
             */
            void start()
            {
                unsigned long candle_close_time;

                uint64_t now_epoch;

                vector<Candle_T> candles;

                map<string, bool> signals;

                this->newest_initialize(
                    this->strateger,
                    this->candle_watcher
                ); cout << "[+] Finished initializing with the newest candle data" << endl;

                cout << "[+] Online trading started" << endl;

                now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();

                std::this_thread::sleep_for(std::chrono::milliseconds(
                    this->candle_watcher->get_last_candle_close_time() + 1 +
                    this->candle_watcher->get_candle_duration() -
                    now_epoch
                ));

                while (1)
                {
                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    candles = this->candle_watcher->get_historical_klines(
                        this->candle_watcher->get_last_candle_close_time(), 
                        now_epoch
                    );

                    for (Candle_T& candle : candles)
                    {
                        candle_close_time = candle.get_close_time();

                        this->candle_watcher->set_last_candle_close_time(
                            candle_close_time
                        );

                        this->strateger->resolve(candle);

                        cout
                            << "[+] Got new Candle" << endl
                            << "    Candle open time: " << candle.get_open_time() << endl
                            << "    Candle open price: " << candle.get_open_price() << endl
                            << "    Candle close time: " << candle.get_close_time() << endl
                            << "    Candle close price: " << candle.get_close_price() << endl
                            << "    Params: " << this->strateger->get_params() << endl
                        << endl;
                    }

                    signals = this->strateger->get_signals();

                    this->online_trader->resolve(signals, this->candle_watcher->get_current_price_online());

                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    std::this_thread::sleep_for(std::chrono::milliseconds(
                        this->candle_watcher->get_last_candle_close_time() + 1501 + 
                        this->candle_watcher->get_candle_duration() - now_epoch
                    ));
                }
            }
    };
}

namespace Workers::TradingView
{

}















#endif