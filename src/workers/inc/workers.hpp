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
             */
            Indicator_Watcher() = default;

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
             * @brief Set the strategies 
             * 
             * @param strat Strategies in config
             */
            void set_strategy_params(nlohmann::json &strat) { this->strategy_params = strat; }

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
             * @brief Candle type
             */
            Candle_T candle;

            /**
             * @brief Symbol (pair)
             */
            string symbol;

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

        public:
            /**
             * @brief Construct a new Candle_Watcher object
             */
            Candle_Watcher() = default;

            /**
             * @brief Construct a new Candle_Watcher object
             * 
             * @param symbol Symbol (pair)
             * @param interval Interval (timeframe)
             */
            Candle_Watcher(
                const string &symbol,
                const string &interval
            ) : symbol(symbol), interval(interval)
            { }

            /**
             * @brief Destroy the Candle_Watcher object
             */
            ~Candle_Watcher() = default;

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
                    cw.symbol == this->symbol && 
                    cw.interval == this->interval
                )
                    return true;
                return false;
            }

            /**
             * @brief Initialize Candle Watcher
             * 
             * @param symbol Symbol (pair)
             * @param interval Interval (timeframe)
             */
            void initialize(
                const string &symbol,
                const string &interval
            )
            {
                this->symbol = symbol;
                this->interval = interval;
            }

            /**
             * @brief Read file once to update candles
             */
            bool read_file_once(
                io::CSVReader<11> &data_file
            )
            {
                string open_price, high_price, low_price, close_price,
                    volume, quote, trades_count, tbbav, tbqav,
                    open_time, close_time
                ;
                if (
                    data_file.read_row(
                    // this->data_file->read_row(
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

                        return true;
                    } catch (std::invalid_argument& exp) {
                        return false;
                    }
                } else {
                    return false;
                }
            }

            /**
             * @brief Get the symbol object
             * 
             * @return string 
             */
            string get_symbol() { return this->symbol; }

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
             * @brief Get the new candles 
             * 
             * @param startTime 
             * @param endTime 
             */
            vector<Candle_T> get_new_candles(unsigned long startTime, unsigned long endTime)
            {
                map<string, string> params;

                string sym;
                string symbol = this->symbol;

                std::remove_copy(
                    symbol.begin(),
                    symbol.end(),
                    std::back_inserter(sym),
                    '/'
                );

                params["symbol"] = sym;
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

                string sym;
                string symbol = this->symbol;

                std::remove_copy(
                    symbol.begin(),
                    symbol.end(),
                    std::back_inserter(sym),
                    '/'
                );

                params["symbol"] = sym;

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

namespace Workers::Solvers
{
    /**
     * @brief Trader decides when open and close order
     */
    class Trader
    {
        private:
            /**
             * @brief Params for trader
             */
            nlohmann::json trader_params;

            /**
             * @brief Pointer to Binance_API Object
             */
            Exchanges::Binance::Binance_API *binapi;

            /**
             * @brief All trades
             */
            vector<Trade> trades;

            /**
             * @brief Symbol (pair) to trade
             */
            string symbol;

            /**
             * @brief Exchange
             */
            string exchange;

            /**
             * @brief Trader type
             */
            string type;

            /**
             * @brief What positions are traded
             */
            string positions;

            /**
             * @brief Goal of short trade (USDT or symbol)
             */
            string short_goal;

            /**
             * @brief Symbol balance
             */
            double symbol_balance;

            /**
             * @brief USDT balance
             */
            double usdt_balance;

            /**
             * @brief Step Size for open Trade (LOT_SIZE Binance API Error)
             */
            double stepSize;

            /**
             * @brief Minimal Notional for open Trade (MIN_NOTIONAL Binance API Error)
             */
            double minNotional;

            /**
             * @brief Quantities of long trade (in Symbol)
             */
            vector<double> long_quantities;

            /**
             * @brief Quantities of short trade (in USDT)
             */
            vector<double> short_quantities;

            /**
             * @brief Maximum opened trades
             */
            double max_open_trade;

            /**
             * @brief Stake amount to "long" trade
             */
            double usdt_amount = 0.0;

            /**
             * @brief Stake amount to "short" trade
             */
            double symbol_amount = 0.0;

            /**
             * @brief Stop-loss percent
             */
            double stop_loss = 0.0;

            /**
             * @brief Target percent
             */
            double target = 0.0;

            /**
             * @brief Set the up amounts to open Trade
             */
            void setup_amounts()
            {
                std::stringstream ss(this->symbol);
                vector<string> symbols;
                string segment;
                while(std::getline(ss, segment, '/'))
                    symbols.push_back(segment);
                this->symbol_balance = std::stod((string)this->binapi->get_balance(symbols[0])["free"]);
                this->usdt_balance = std::stod((string)this->binapi->get_balance(symbols[1])["free"]);

                if (this->symbol_balance >= this->minNotional)
                {
                    this->symbol_amount = (int)(this->symbol_balance / this->max_open_trade);
                    if (this->symbol_amount <= this->minNotional)
                        this->symbol_amount = this->minNotional;
                }
                if (this->usdt_balance >= this->minNotional)
                {
                    this->usdt_amount = (int)(this->usdt_balance / this->max_open_trade);
                    if (this->usdt_amount <= this->minNotional)
                        this->usdt_amount = this->minNotional;
                }
            }

            /**
             * @brief Initialize new Trade
             */
            void initialize_trade(
                Trade &trade,
                const string& position,
                double price = 0
            )
            {
                // TODO: Unique ID for each Trade
                trade.set_open_time(
                    1, this->symbol, position, 
                    this->usdt_amount, 
                    this->symbol_amount, price 
                );
            }

            /**
             * @brief Clear current Trade to open new one
             */
            void clear_trade()
            {
                // TODO: Clear Trade object
            }

            /**
             * @brief Open new Trade
             */
            void open_trade(
                Trade &trade,
                const string& position,
                double open_price
            )
            {
                this->initialize_trade(
                    trade,
                    position,
                    open_price
                );
                // TODO: Open trade in binance
            }

            /**
             * @brief Close current Trade
             */
            void close_trade(
                Trade &trade,
                double close_price
            )
            {
                if (trade.get_position() == "short")
                {
                    trade.set_close_time(
                        close_price,
                        this->short_goal
                    );
                } else if (trade.get_position() == "long")
                {
                    trade.set_close_time(
                        close_price
                    );
                }

                // TODO: Close trade in binance
            }

        public:
            /**
             * @brief Construct a new Trader object
             */
            Trader() = default;

            /**
             * @brief Construct a new ema cross trader object
             * 
             * @param trader_params Parameters for Trader
             * @param exchange_params Parameters for Exchanges
             */
            Trader(
                nlohmann::json &trader_params,
                nlohmann::json &exchange_params
            ) : trader_params(trader_params)
            { 
                this->symbol = (string)trader_params["symbol"];
                this->exchange = (string)trader_params["exchange"];
                this->positions = (string)trader_params["positions"];
                this->short_goal = (string)trader_params["short_goal"];

                this->usdt_amount = (double)trader_params["stake_amount"];
                this->symbol_amount = (double)trader_params["symbol_amount"];
                this->max_open_trade = (double)trader_params["max_open_trade"];

                if ((string)trader_params["type"] == "scalping")
                {
                    this->type = (string)trader_params["type"];
                    this->stop_loss = (double)trader_params["stop-loss"];
                    this->target = (double)trader_params["target"];
                } else if ((string)trader_params["type"] == "position") {
                    this->type = (string)trader_params["type"];
                }

                if (this->exchange == "binance")
                    this->binapi = new Exchanges::Binance::Binance_API(
                        (string)exchange_params["binance"]["private_key"],
                        (string)exchange_params["binance"]["secret_key"]
                    );
            }

            /**
             * @brief Copy construct of Trader object
             * 
             * @param tr_in Trade object
             */
            Trader(const Trader& tr_in)
                : binapi(new Exchanges::Binance::Binance_API("", ""))
            { }

            /**
             * @brief Destroy the Trader object
             */
            // ~Trader() = default;
            ~Trader()
            {
                delete this->binapi;
            }

            /**
             * @brief Assignment operator
             * 
             * @param tr_in Trade object
             * @return Trader& 
             */
            Trader& operator = (const Trader& tr_in)
            {
                if (this != &tr_in)
                {

                }
                return *this;
            }

            /**
             * @brief Set the trader params via JSON Object
             * 
             * @param trader_params Params for Trader
             * @param exchange_params Parameters for Exchanges
             */
            void set_trader_params(
                nlohmann::json &trader_params,
                nlohmann::json &exchange_params
            ) 
            { 
                this->trader_params = trader_params;

                this->symbol = (string)trader_params["symbol"];
                this->exchange = (string)trader_params["exchange"];
                this->positions = (string)trader_params["positions"];
                this->short_goal = (string)trader_params["short_goal"];

                this->usdt_amount = (double)trader_params["stake_amount"];
                this->symbol_amount = (double)trader_params["symbol_amount"];
                this->max_open_trade = (double)trader_params["max_open_trade"];

                if ((string)trader_params["type"] == "scalping")
                {
                    this->type = (string)trader_params["type"];
                    this->stop_loss = (double)trader_params["stop-loss"];
                    this->target = (double)trader_params["target"];
                } else if ((string)trader_params["type"] == "position") {
                    this->type = (string)trader_params["type"];
                }

                if (this->exchange == "binance")
                    this->binapi = new Exchanges::Binance::Binance_API(
                        (string)exchange_params["binance"]["private_key"],
                        (string)exchange_params["binance"]["secret_key"]
                    );
            }

            /**
             * @brief Set the stake amount 
             * 
             * @param amount Stake amount
             */
            void set_usdt_amount(double amount) { this->usdt_amount = amount; }

            /**
             * @brief Set the symbol abount object
             * 
             * @param amount Stake amount
             */
            void set_symbol_abount(double amount) { this->symbol_amount = amount; }

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
            string get_symbol() { return this->symbol; }

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
            double get_usdt_amount() { return this->usdt_amount; }

            /**
             * @brief Initialize Trader
             */
            void initialize()
            {
                string sym;

                std::remove_copy(
                    this->symbol.begin(),
                    this->symbol.end(),
                    std::back_inserter(sym),
                    '/'
                );

                nlohmann::json ex_in = this->binapi->exchange_info(sym)["symbols"][0]["filters"];

                this->stepSize = std::stod((string)ex_in[2]["stepSize"]);
                this->minNotional = std::stod((string)ex_in[3]["minNotional"]);

                cout 
                    << "    LOT_SIZE for " << this->symbol << " is " << this->stepSize << endl
                    << "    MIN_NOTIONAL for " << this->symbol << " is " << this->minNotional << endl
                << endl;

                this->setup_amounts();

                cout
                    << "[+] Start balance:" << endl
                    << "    Symbols: " << this->symbol_balance << endl
                    << "    USDT: " << this->usdt_balance << endl
                << endl;
            }

            /**
             * @brief Decide what to do with trade
             * 
             * @param signals 
             * @param ret_trades Array of returning trades
             * @param price Current price
             */
            void resolve(
                map<string, bool> &signals,
                vector<Trade> &ret_trades,
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
                            this->trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade, 
                                (string)"long",
                                price
                            );
                            this->trades.push_back(trade);
                            cout << "[+] Opened long trade" << endl;
                        }
                    }
                    if (signals["long_close"])
                    {
                        for (Trade& trade : this->trades)
                            if (trade.get_position() == "long")
                            {
                                this->close_trade(trade, price);
                                ret_trades.push_back(trade);
                                this->trades.erase(
                                    std::remove(
                                        this->trades.begin(),
                                        this->trades.end(),
                                        trade
                                    ),
                                    this->trades.end()
                                );
                                cout << "[+] Closed long trade" << endl;
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
                            this->trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade,
                                (string)"short",
                                price
                            );
                            this->trades.push_back(trade);
                            cout << "[+] Opened short trade" << endl;
                        }
                    }
                    if (signals["short_close"])
                    {
                        for (Trade& trade : this->trades)
                            if (trade.get_position() == "short")
                            {
                                this->close_trade(trade, price);
                                ret_trades.push_back(trade);
                                this->trades.erase(
                                    std::remove(
                                        this->trades.begin(),
                                        this->trades.end(),
                                        trade
                                    ),
                                    this->trades.end()
                                );
                                cout << "[+] Closed short trade" << endl;
                            }
                    }
                } else if (this->type == "scalping") {
                    double opened_price;
                    double percent;
                    for (Trade& trade : this->trades)
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
                                ret_trades.push_back(trade);
                                this->trades.erase(
                                    std::remove(
                                        this->trades.begin(),
                                        this->trades.end(),
                                        trade
                                    ),
                                    this->trades.end()
                                );
                                cout << "[+] Closed long trade" << endl;
                            }
                        } else if (trade.get_position() == "short") {
                            percent = (opened_price / price - 1) * 100;
                            if (
                                this->target <= percent ||
                                this->stop_loss >= percent
                            ) {
                                this->close_trade(trade, price);
                                ret_trades.push_back(trade);
                                this->trades.erase(
                                    std::remove(
                                        this->trades.begin(),
                                        this->trades.end(),
                                        trade
                                    ),
                                    this->trades.end()
                                );
                                cout << "[+] Closed short trade" << endl;
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
                            this->trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade,
                                (string)"long",
                                price
                            );
                            this->trades.push_back(trade);
                            cout << "[+] Opened long trade" << endl;
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
                            this->trades.size() <= this->max_open_trade ||
                            this->max_open_trade == -1.0
                        ) {
                            Trade trade;
                            this->open_trade(
                                trade, 
                                (string)"short",
                                price
                            );
                            this->trades.push_back(trade);
                            cout << "[+] Opened short trade" << endl;
                        }
                    }
                }
            }

            /**
             * @brief Resolve for real money
             * 
             * @param signals Signals
             */
            void resolve_real(
                map<string, bool> &signals
            ) {
                // TODO: scalping
                cout << usdt_amount << ' ' << symbol_amount << endl;
                if (
                    signals["long_open"] &&
                    (
                        this->positions == "both" ||
                        this->positions == "long"
                    )
                ) {
                    nlohmann::json response = this->binapi->open_new_order(
                        this->symbol,
                        "BUY",
                        std::to_string(this->usdt_amount)
                    );
                    cout << response << endl;
                    double qty = (double)response["fills"][0]["qty"];
                    this->long_quantities.push_back(qty);
                }
                if (signals["long_close"])
                {
                    for (auto& key : this->long_quantities)
                        cout << this->binapi->open_new_order(
                            this->symbol,
                            "SELL",
                            std::to_string(key)
                        ) << endl;
                    this->setup_amounts();
                }
                if (
                    signals["short_open"] &&
                    (
                        this->positions == "both" ||
                        this->positions == "short"
                    )
                ) {
                    nlohmann::json response = this->binapi->open_new_order(
                        this->symbol,
                        "SELL",
                        std::to_string(this->usdt_amount)
                    );
                    cout << response << endl;
                    double qty = (double)response["fills"][0]["qty"];
                    this->short_quantities.push_back(qty);
                }
                if (signals["short_close"])
                {
                    for (auto& key : this->long_quantities)
                        cout << this->binapi->open_new_order(
                            this->symbol,
                            "BUY",
                            std::to_string(key)
                        ) << endl;
                    this->setup_amounts();
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
            Workers::Watchers::Indicator_Watcher<Candle_T> indicator_watcher;

        public:
            /**
             * @brief Construct a new Solver 
             */
            Strateger() = default;

            /**
             * @brief Construct a new Solver object
             * 
             * @param strategy_params Params for Strategy
             */
            Strateger(
                nlohmann::json &strategy_params
            ) : strategy_params(strategy_params)
            { 
                this->indicator_watcher.set_strategy_params(strategy_params);
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
             * @brief Set strategy parameters
             * 
             * @param strategy_params Strategy parameters
             */
            void set_strategy_params(nlohmann::json &strategy_params)
            {
                this->strategy_params = strategy_params;
                this->indicator_watcher.set_strategy_params(strategy_params);
            }

            /**
             * @brief Initialize Strateger
             */
            void initialize()
            {
                this->indicator_watcher.initialize_indicators();
            }

            /**
             * @brief Resolve the strategy in worker
             */
            void resolve(Candle_T &candle)
            {
                this->indicator_watcher.resolve(candle);
                this->params = this->indicator_watcher.get(
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
namespace Workers
{
    /**
     * @brief Trading Worker
     * 
     * @tparam Strategy Strategy type
     * @tparam Candle_T Candle type
     */
    template <class Strategy, class Candle_T>
    class Worker
    {
        private:
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
             * @brief Worker symbol
             */
            string symbol;

            /**
             * @brief Last candle close time
             */
            unsigned long last_candle_close_time;

            /**
             * @brief Candle duraction in milliseconds
             */
            unsigned long candle_duration;

            /**
             * @brief JSON Worker configuration
             */
            nlohmann::json worker_configuration;

            /**
             * @brief Trader object
             */
            Workers::Solvers::Trader trader;

            /**
             * @brief Strateger object
             */
            Workers::Solvers::Strateger<Strategy, Candle_T> strateger;

            /**
             * @brief Candle Watcher object
             */
            Workers::Watchers::Candle_Watcher<Candle_T> candle_watcher;

            /**
             * @brief Initialize worker by data file
             * 
             * @param data_dir Path to data directory
             */
            void initialize_indicators(
                const string& data_dir
            )
            {
                string symbol = this->worker_configuration["trader_params"]["symbol"];
                string interval = this->worker_configuration["interval"];
                string sym;

                Candle_T candle;

                std::remove_copy(
                    symbol.begin(),
                    symbol.end(),
                    std::back_inserter(sym),
                    '/'
                );
    
                io::CSVReader<11> data_file(
                    data_dir + sym + '_' + interval +".csv"               
                );

                cout << this->name << " : [+] Start initializing from file" << endl;
                this->candle_watcher.read_file_once(data_file);
                while(candle_watcher.read_file_once(data_file))
                {
                    candle = this->candle_watcher.get_candle();
                    this->strateger.resolve(candle);
                }
                this->last_candle_close_time = candle.get_close_time();
            }

        public:
            /**
             * @brief Construct a new Worker object
             * 
             * @param worker_configuration JSON Worker Configuration
             */
            Worker(
                nlohmann::json &worker_configuration
            ) : worker_configuration(worker_configuration)
            { 
                this->name = worker_configuration["name"];
                this->strategy = worker_configuration["strategy"];
                this->candle_type = worker_configuration["candle"];
                this->interval = worker_configuration["interval"];
                this->symbol = worker_configuration["trader_params"]["symbol"];
            }

            /**
             * @brief Destroy the Worker object
             */
            ~Worker() = default;

            /**
             * @brief Initialize Worker
             * 
             * @param exchange_params Parameters for Exchange
             */
            void configurate(
                nlohmann::json &exchange_params
            )
            {
                cout << this->name << " : [+] Configurate bot" << endl;

                this->trader.set_trader_params(
                    this->worker_configuration["trader_params"],
                    exchange_params
                ); this->trader.initialize();

                this->strateger.set_strategy_params(
                    this->worker_configuration["strategy_params"]
                ); this->strateger.initialize();

                this->candle_watcher.initialize(
                    this->worker_configuration["trader_params"]["symbol"],
                    this->worker_configuration["interval"]
                );

                string interval = this->worker_configuration["interval"];
                if (interval == "1d")
                {
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
                    << "Symbol: " << this->symbol << endl 
                << endl;
            }

            /**
             * @brief Online backtesting
             * 
             * @param start_balance Start balance in USDT
             * @param start_symbols Start balance in Symbol
             * @param data_dir Path to data directory
             */
            void online_backtest(
                double start_balance,
                double start_symbols,
                const string &data_dir
            )
            {
                double max_open_trades = this->worker_configuration["trader_params"]["max_open_trade"];

                string candle_type = this->worker_configuration["candle"];
                string short_goal = this->worker_configuration["trader_params"]["short_goal"];

                unsigned long current_candle_open_time;
                unsigned long previous_candle_open_time;
                unsigned long previous_candle_close_time;

                double candle_close_price;
                double stake_amount_trade;
                double symbol_amount_trade;
                double symbol_balance_usdt = 0;
                double minimal_symbol_balance_usdt;
                double maximum_symbol_balance_usdt;
                double abs_profit_long = 0;
                double per_profit_long = 0;
                double average_abs_profit_per_trade_long = 0;
                double average_per_profit_per_trade_long = 0;
                double abs_profit_short = 0;
                double per_profit_short = 0;
                double average_abs_profit_per_trade_short = 0;
                double average_per_profit_per_trade_short = 0;
                int total_trades_long = 0;
                int wins_long = 0;
                int loses_long = 0;
                double per_wins_long;
                int total_trades_short = 0;
                int wins_short = 0;
                int loses_short = 0;
                double per_wins_short;
                double best_long_trade = 0.0;
                double worst_long_trade = 0.0;
                double best_short_trade = 0.0;
                double worst_short_trade = 0.0;

                double balance = start_balance;
                double minimal_balance = start_balance;
                double maximum_balance = start_balance;

                double symbol_balance = start_symbols;
                double minimal_symbol_balance = start_symbols;
                double maximum_symbol_balance = start_symbols;

                double last_candle_close_price;

                uint64_t now_epoch;

                Candle_T candle;

                vector<Candle_T> candles;
                vector<Trade> trades;

                map<string, bool> signals;

                this->initialize_indicators(data_dir);

                cout << this->name << " : [+] Start online backtesting" << endl;

                now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();

                // Get new data and initialize
                cout << this->name << " : [+] Initialize with the newest candle data" << endl;
                while(now_epoch - this->last_candle_close_time >= this->candle_duration) 
                {
                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    candles = this->candle_watcher.get_new_candles(this->last_candle_close_time, now_epoch);
                    for (int i = 0; i < candles.size(); i++)
                    {
                        if (i != 0)
                        {
                            previous_candle_close_time = candles[i - 1].get_close_time();
                            previous_candle_open_time = candles[i - 1].get_open_time();
                        }
                        current_candle_open_time = candles[i].get_open_time();
                        this->last_candle_close_time = candles[i].get_close_time();
                        if (now_epoch - this->last_candle_close_time >= 18000000000000000000)
                            goto finish_getting_candles;
                        this->strateger.resolve(candles[i]);
                        last_candle_close_price = candles[i].get_close_price();
                    }
                }

                // Start online trading
            finish_getting_candles:

                std::this_thread::sleep_for(std::chrono::milliseconds(this->last_candle_close_time - now_epoch));

                cout << this->name << " : [+] Online trading started" << endl;
                while (1)
                {
                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    candles = this->candle_watcher.get_new_candles(previous_candle_close_time, now_epoch - this->candle_duration);

                    for (Candle_T& cd : candles)
                    {
                        previous_candle_close_time = cd.get_close_time();
                        this->strateger.resolve(cd);
                        cout
                            << this->name 
                            << " : [+] Got new Candle!" << endl
                            << "    Candle open time: " << cd.get_open_time() << endl
                            << "    Candle open price: " << cd.get_open_price() << endl
                            << "    Candle close time: " << cd.get_close_time() << endl
                            << "    Candle close price: " << cd.get_close_price() << endl
                            << "    Params: " << this->strateger.get_params() << endl
                        << endl;
                    }

                    cout 
                        << this->name << " : [+] Status" << endl
                        << "    Balance: " << balance << endl
                        << "    Symbols: " << symbol_balance << endl
                    << endl;

                    signals = this->strateger.get_signals();

                    stake_amount_trade = balance / max_open_trades;
                    symbol_amount_trade = symbol_balance / max_open_trades;

                    this->trader.set_usdt_amount(stake_amount_trade);
                    this->trader.set_symbol_abount(symbol_amount_trade);

                    this->trader.resolve(signals, trades, this->candle_watcher.get_current_price_online());

                    for (Trade& trade : trades) 
                    {
                        if (trade.is_completed())
                        {
                            if (trade.get_position() == "long")
                            {
                                balance += trade.get_abs_profit();

                                if (best_long_trade == 0.0)
                                {
                                    best_long_trade = trade.get_per_profit();
                                } else {
                                    if (best_long_trade <= trade.get_per_profit())
                                        best_long_trade = trade.get_per_profit();
                                }

                                if (worst_long_trade == 0.0)
                                {
                                    worst_long_trade = trade.get_per_profit();
                                } else {
                                    if (worst_long_trade >= trade.get_per_profit())
                                        worst_long_trade = trade.get_per_profit();
                                }

                                if (balance > maximum_balance)
                                    maximum_balance = balance;
                                if (balance < minimal_balance)
                                    minimal_balance = balance;

                                average_abs_profit_per_trade_long += trade.get_abs_profit();
                                average_per_profit_per_trade_long += trade.get_per_profit();

                                total_trades_long++;

                                if ((trade.get_per_profit() + 100) / 100 >= 1)
                                {
                                    wins_long++;

                                } else {
                                    loses_long++;
                                }
                            } else if (trade.get_position() == "short") {
                                if (short_goal == "symbol")
                                {
                                    symbol_balance +=trade.get_abs_profit();

                                    if (symbol_balance > maximum_symbol_balance)
                                        maximum_symbol_balance = symbol_balance;

                                    if (symbol_balance < minimal_symbol_balance)
                                        minimal_symbol_balance = symbol_balance;
                                }
                                if (short_goal == "USDT")
                                {
                                    symbol_balance_usdt +=trade.get_abs_profit();

                                    if (symbol_balance_usdt > maximum_symbol_balance_usdt)
                                        maximum_symbol_balance_usdt = symbol_balance_usdt;

                                    if (symbol_balance_usdt < minimal_symbol_balance_usdt)
                                        minimal_symbol_balance_usdt = symbol_balance_usdt;
                                }

                                if (best_short_trade == 0.0)
                                {
                                    best_short_trade = trade.get_per_profit();
                                } else {
                                    if (best_short_trade <= trade.get_per_profit())
                                        best_short_trade = trade.get_per_profit();
                                }

                                if (worst_short_trade == 0.0) 
                                {
                                    worst_short_trade = trade.get_per_profit();
                                } else {
                                    if (worst_short_trade >= trade.get_per_profit())
                                        worst_short_trade = trade.get_per_profit();
                                }

                                average_abs_profit_per_trade_short += trade.get_abs_profit();
                                average_per_profit_per_trade_short += trade.get_per_profit();

                                total_trades_short++;

                                if ((trade.get_per_profit() + 100) / 100 >= 1)
                                {
                                    wins_short++;
                                } else {
                                    loses_short++;
                                }
                            }

                            cout 
                                << this->name << endl
                                << "Position: " << trade.get_position() << endl
                                << "Absolute profit: " << trade.get_abs_profit() << endl
                                << "Percentage profit: " << trade.get_per_profit() << endl
                                << "Open price: " << trade.get_open_price() << endl
                                << "Close price: " << trade.get_close_price() << endl
                                << "Stake amount: " << trade.get_stake_amount() << endl
                                << "Symbol amount: " << trade.get_symbol_amount() << endl
                                << "Current balance: " << balance << endl
                                << "Current symbol balance: " << symbol_balance << endl
                            << endl;
                        }
                    }

                    trades.clear();

                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    // TODO: Remake delay
                    std::this_thread::sleep_for(std::chrono::milliseconds(
                        previous_candle_close_time + 1501 + this->candle_duration - now_epoch
                        // previous_candle_close_time + 1 + this->candle_duration - now_epoch
                    ));
                }
            }

            /**
             * @brief Backtest Worker
             * 
             * @param data_dir Root path to data directory
             * @param start_balance Start USDT balance (for long trades)
             * @param start_symbols Start Symbol balance (for short trades)
             */
            void file_backtest(
                const string& data_dir,
                double start_balance,
                double start_symbols 
            ) {
                double max_open_trades = this->worker_configuration["trader_params"]["max_open_trade"];

                string symbol = this->worker_configuration["trader_params"]["symbol"];
                string interval = this->worker_configuration["interval"];
                string candle_type = this->worker_configuration["candle"];
                string short_goal = this->worker_configuration["trader_params"]["short_goal"];
                string sym;

                map<string, bool> signals;

                Candle_T candle;

                std::remove_copy(
                    symbol.begin(),
                    symbol.end(),
                    std::back_inserter(sym),
                    '/'
                );

                io::CSVReader<11> data_file(
                    data_dir + sym + '_' + interval +".csv"               
                );

                double candle_close_price;
                double stake_amount_trade;
                double symbol_amount_trade;
                double symbol_balance_usdt = 0;
                double minimal_symbol_balance_usdt;
                double maximum_symbol_balance_usdt;
                double abs_profit_long = 0;
                double per_profit_long = 0;
                double average_abs_profit_per_trade_long = 0;
                double average_per_profit_per_trade_long = 0;
                double abs_profit_short = 0;
                double per_profit_short = 0;
                double average_abs_profit_per_trade_short = 0;
                double average_per_profit_per_trade_short = 0;
                int total_trades_long = 0;
                int wins_long = 0;
                int loses_long = 0;
                double per_wins_long;
                int total_trades_short = 0;
                int wins_short = 0;
                int loses_short = 0;
                double per_wins_short;
                double best_long_trade = 0.0;
                double worst_long_trade = 0.0;
                double best_short_trade = 0.0;
                double worst_short_trade = 0.0;

                double balance = start_balance;
                double minimal_balance = start_balance;
                double maximum_balance = start_balance;

                double symbol_balance = start_symbols;
                double minimal_symbol_balance = start_symbols;
                double maximum_symbol_balance = start_symbols;

                auto start_time = std::chrono::high_resolution_clock::now();

                this->candle_watcher.read_file_once(data_file);
                while(candle_watcher.read_file_once(data_file))
                {
                    vector<Trade> trades;

                    candle = this->candle_watcher.get_candle();
                    this->strateger.resolve(candle);
                    signals = this->strateger.get_signals();

                    stake_amount_trade = balance / max_open_trades;
                    symbol_amount_trade = symbol_balance / max_open_trades;

                    this->trader.set_usdt_amount(stake_amount_trade);
                    this->trader.set_symbol_abount(symbol_amount_trade);

                    candle_close_price = candle.get_close_price();

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
                                balance += trade.get_abs_profit();

                                if (best_long_trade == 0.0)
                                {
                                    best_long_trade = trade.get_per_profit();
                                } else {
                                    if (best_long_trade <= trade.get_per_profit())
                                        best_long_trade = trade.get_per_profit();
                                }

                                if (worst_long_trade == 0.0)
                                {
                                    worst_long_trade = trade.get_per_profit();
                                } else {
                                    if (worst_long_trade >= trade.get_per_profit())
                                        worst_long_trade = trade.get_per_profit();
                                }

                                if (balance > maximum_balance)
                                    maximum_balance = balance;
                                if (balance < minimal_balance)
                                    minimal_balance = balance;

                                average_abs_profit_per_trade_long += trade.get_abs_profit();
                                average_per_profit_per_trade_long += trade.get_per_profit();

                                total_trades_long++;

                                if ((trade.get_per_profit() + 100) / 100 >= 1)
                                {
                                    wins_long++;

                                } else {
                                    loses_long++;
                                }
                            } else if (trade.get_position() == "short") {
                                if (short_goal == "symbol")
                                {
                                    symbol_balance +=trade.get_abs_profit();

                                    if (symbol_balance > maximum_symbol_balance)
                                        maximum_symbol_balance = symbol_balance;

                                    if (symbol_balance < minimal_symbol_balance)
                                        minimal_symbol_balance = symbol_balance;
                                }
                                if (short_goal == "USDT")
                                {
                                    symbol_balance_usdt +=trade.get_abs_profit();

                                    if (symbol_balance_usdt > maximum_symbol_balance_usdt)
                                        maximum_symbol_balance_usdt = symbol_balance_usdt;

                                    if (symbol_balance_usdt < minimal_symbol_balance_usdt)
                                        minimal_symbol_balance_usdt = symbol_balance_usdt;
                                }

                                if (best_short_trade == 0.0)
                                {
                                    best_short_trade = trade.get_per_profit();
                                } else {
                                    if (best_short_trade <= trade.get_per_profit())
                                        best_short_trade = trade.get_per_profit();
                                }

                                if (worst_short_trade == 0.0) 
                                {
                                    worst_short_trade = trade.get_per_profit();
                                } else {
                                    if (worst_short_trade >= trade.get_per_profit())
                                        worst_short_trade = trade.get_per_profit();
                                }

                                average_abs_profit_per_trade_short += trade.get_abs_profit();
                                average_per_profit_per_trade_short += trade.get_per_profit();

                                total_trades_short++;

                                if ((trade.get_per_profit() + 100) / 100 >= 1)
                                {
                                    wins_short++;
                                } else {
                                    loses_short++;
                                }
                            }

                            cout 
                                << "Position: " << trade.get_position() << endl
                                << "Absolute profit: " << trade.get_abs_profit() << endl
                                << "Percentage profit: " << trade.get_per_profit() << endl
                                << "Open price: " << trade.get_open_price() << endl
                                << "Close price: " << trade.get_close_price() << endl
                                << "Stake amount: " << trade.get_stake_amount() << endl
                                << "Symbol amount: " << trade.get_symbol_amount() << endl
                                << "Current balance: " << balance << endl
                                << "Current symbol balance: " << symbol_balance << endl
                            << endl;
                        }
                    }

                    cout << "Candle close: " << candle_close_price << endl;
                    for (auto& [key, val] : signals)
                        cout << key << " : " << val << endl;
                    cout << endl;
                }
            
                auto end_time = std::chrono::high_resolution_clock::now();
                auto exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

                abs_profit_long = balance - start_balance;
                per_profit_long = balance / start_balance * 100 - 100;
                average_abs_profit_per_trade_long /= total_trades_long;
                average_per_profit_per_trade_long /= total_trades_long;

                average_abs_profit_per_trade_short /= total_trades_short;
                average_per_profit_per_trade_short /= total_trades_short;

                per_wins_long = (double)wins_long / (double)total_trades_long * 100;
                per_wins_short = (double)wins_short / (double)total_trades_short * 100;

                cout 
                    << "----------------------------------" << endl
                    << "LONG POSITION STATISTIC:" << endl
                    << endl
                    << "BALANCE:" << endl
                    << "Start Balance: " << start_balance << " USDT" << endl
                    << "Final Balance: " << balance << " USDT" << endl 
                    << "Maximum balance: " << maximum_balance<< " USDT"  << endl
                    << "Minimal balance: " << minimal_balance << " USDT" << endl
                    << endl
                    << "PROFIT:" << endl
                    << "Absolute profit: " << abs_profit_long << " USDT" << endl
                    << "Percentage profit: " << per_profit_long << " %" << endl
                    << "Average absolute profit per trades: " << average_abs_profit_per_trade_long << " USDT" << endl
                    << "Average percentage profit per trades: " << average_per_profit_per_trade_long << " %" << endl
                    << endl
                    << "TRADE" << endl
                    << "Total trades: " << total_trades_long << endl
                    << "Wins: " << wins_long << endl
                    << "Loses: " << loses_long << endl
                    << "Percent wins: " << per_wins_long << " %" << endl
                    << "Best long trade: " << best_long_trade << " %" << endl
                    << "Worst long trade: " << worst_long_trade << " %" << endl
                    << "----------------------------------" << endl
                    << "SHORT POSITION STATISTIC:" << endl
                    << endl
                    << "BALANCE:" << endl;
                if (this->worker_configuration["trader_params"]["short_goal"] == "USDT")
                {
                    cout 
                        << "Start Balance: " << 0 << " USDT" << endl
                        << "Final Balance: " << symbol_balance_usdt << " USDT" << endl 
                        << "Maximum balance: " << maximum_symbol_balance_usdt << " USDT"  << endl
                        << "Minimal balance: " << minimal_symbol_balance_usdt << " USDT" << endl
                        << endl
                        << "PROFIT:" << endl
                        << "Average absolute profit per trades: " << average_abs_profit_per_trade_short << " USDT" << endl
                        << "Average percentage profit per trades: " << average_per_profit_per_trade_short << " %" << endl
                        << endl;
                } else if (this->worker_configuration["trader_params"]["short_goal"] == "symbol") {
                    cout 
                        << "Start Balance: " << start_symbols << " Symbols" << endl
                        << "Final Balance: " << symbol_balance << " Symbols" << endl 
                        << "Maximum balance: " << maximum_symbol_balance << " Symbols"  << endl
                        << "Minimal balance: " << minimal_symbol_balance << " Symbols" << endl
                        << endl
                        << "PROFIT:" << endl
                        << "Average absolute profit per trades: " << average_abs_profit_per_trade_short << " Symbols" << endl
                        << "Average percentage profit per trades: " << average_per_profit_per_trade_short << " %" << endl
                        << endl;
                }
                cout 
                    << "TRADE" << endl
                    << "Total trades: " << total_trades_short << endl
                    << "Wins: " << wins_short << endl
                    << "Loses: " << loses_short << endl
                    << "Percent wins: " << per_wins_short << " %" << endl
                    << "Best long trade: " << best_short_trade << " %" << endl
                    << "Worst long trade: " << worst_short_trade << " %" << endl
                    << "----------------------------------" << endl
                    << "Test time (ms): " << exec_time.count() << endl
                ;
            }

            /**
             * @brief Start online trading for real money
             * 
             * @param data_dir 
             */
            void start(
                const string &data_dir
            ) {
                // TODO: Remake with config settings

                unsigned long current_candle_open_time;
                unsigned long previous_candle_open_time;
                unsigned long previous_candle_close_time;

                double last_candle_close_price;

                uint64_t now_epoch;

                Candle_T candle;

                vector<Candle_T> candles;
                vector<Trade> trades;

                map<string, bool> signals;

                this->initialize_indicators(data_dir);

                now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();

                // Get new data and initialize
                cout << this->name << " : [+] Initialize with the newest candle data" << endl;
                while(now_epoch - this->last_candle_close_time >= this->candle_duration) 
                {
                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    candles = this->candle_watcher.get_new_candles(this->last_candle_close_time, now_epoch);
                    for (int i = 0; i < candles.size(); i++)
                    {
                        if (i != 0)
                        {
                            previous_candle_close_time = candles[i - 1].get_close_time();
                            previous_candle_open_time = candles[i - 1].get_open_time();
                        }
                        current_candle_open_time = candles[i].get_open_time();
                        this->last_candle_close_time = candles[i].get_close_time();
                        if (now_epoch - this->last_candle_close_time >= 18000000000000000000)
                            goto finish_getting_candles;
                        this->strateger.resolve(candles[i]);
                        last_candle_close_price = candles[i].get_close_price();
                    }
                }

                // Start online trading
            finish_getting_candles:

                std::this_thread::sleep_for(std::chrono::milliseconds(this->last_candle_close_time - now_epoch));

                cout << this->name << " : [+] Online trading started" << endl;
                while (1)
                {
                    now_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count();

                    candles = this->candle_watcher.get_new_candles(previous_candle_close_time, now_epoch - this->candle_duration);

                    for (Candle_T& cd : candles)
                    {
                        previous_candle_close_time = cd.get_close_time();
                        this->strateger.resolve(cd);
                        cout
                            << this->name 
                            << " : [+] Got new Candle!" << endl
                            << "    Candle open time: " << cd.get_open_time() << endl
                            << "    Candle open price: " << cd.get_open_price() << endl
                            << "    Candle close time: " << cd.get_close_time() << endl
                            << "    Candle close price: " << cd.get_close_price() << endl
                            << "    Params: " << this->strateger.get_params() << endl
                        << endl;
                    }

                    signals = this->strateger.get_signals();
                    this->trader.resolve_real(signals);
                }
            }
    };
}

















#endif