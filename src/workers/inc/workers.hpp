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

#include "../../strategies/inc/strategies.hpp"
#include "../../indicators/inc/indicators.hpp"
#include "../../candle/inc/candle.hpp"
#include "../../exceptions/inc/exceptions.hpp"
#include "../../trade/inc/trade.hpp"
#include "../../request/inc/request.hpp"
#include "../../libs/csv/csv.hpp"

using std::string; 
using std::cout; 
using std::endl; 
using std::map; 
using std::vector;

/**
 * @brief All Workers which create Trader
 */
namespace Workers
{
    /**
     * @brief Trader decides when open and close order
     */
    class Trader
    {
        private:
            /**
             * @brief All trades
             */
            vector<Trade> trades;

            /**
             * @brief Symbol (pair) to trade
             */
            string symbol;

            /**
             * @brief Interval of candles
             */
            string interval;

            /**
             * @brief Name of Trader
             */
            string name;

            /**
             * @brief Exchange
             */
            string exchange;

            /**
             * @brief Trader type
             */
            string type;

            /**
             * @brief Stake amount to "long" trade
             */
            double stake_amount = 0.0;

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
             * @brief Initialize new Trade
             */
            void initialize_trade(
                Trade &trade,
                double price = 0,
                bool buy_signal = false,
                bool sell_signal = false
            )
            {
                // TODO: Unique ID for each Trade
                string position;
                if (buy_signal)
                    position = "long";
                if (sell_signal)
                    position = "short";
                trade.set_open_time(
                    1, this->symbol, position, 
                    this->interval, this->stake_amount, 
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
                double open_price,
                bool buy_signal = false,
                bool sell_signal = false
            )
            {
                if (!buy_signal && !sell_signal)
                    return;
                this->initialize_trade(
                    trade,
                    open_price, 
                    buy_signal,
                    sell_signal
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
                trade.set_close_time(
                    close_price
                );
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
             */
            Trader(
                nlohmann::json &trader_params
            )
            { 
                this->symbol = (string)trader_params["symbol"];
                this->interval = (string)trader_params["interval"];
                if ((string)trader_params["strategy"]["stake_amount"] != "all")
                    this->stake_amount = (double)trader_params["strategy"]["stake_amount"];
                if ((string)trader_params["strategy"]["symbol_amount"] != "all")
                    this->symbol_amount = (double)trader_params["strategy"]["symbol_amount"];
                this->name = (string)trader_params["name"];
                this->exchange = (string)trader_params["exchange"];
                if ((string)trader_params["strategy"]["type"] == "scalping")
                {
                    this->stop_loss = (double)trader_params["strategy"]["stop-loss"];
                    this->target = (double)trader_params["strategy"]["target"];
                    this->type = (string)trader_params["strategy"]["type"];
                } else if ((string)trader_params["strategy"]["type"] == "position") {
                    this->type = (string)trader_params["strategy"]["type"];
                }
            }

            /**
             * @brief Destroy the Trader object
             */
            ~Trader() = default;

            /**
             * @brief Set the trader params via JSON Object
             * 
             * @param trader_params Params for Trader
             */
            void set_trader_params(nlohmann::json &trader_params) 
            { 
                this->symbol = trader_params["symbol"];
                this->interval = trader_params["interval"];
                if ((string)trader_params["strategy"]["stake_amount"] != "all")
                    this->stake_amount = (double)trader_params["strategy"]["stake_amount"];
                if ((string)trader_params["strategy"]["symbol_amount"] != "all")
                    this->symbol_amount = (double)trader_params["strategy"]["symbol_amount"];
                this->name = trader_params["name"];
                this->exchange = trader_params["exchange"];
                if ((string)trader_params["strategy"]["type"] == "scalping")
                {
                    this->stop_loss = (double)trader_params["strategy"]["stop-loss"];
                    this->target = (double)trader_params["strategy"]["target"];
                    this->name = (string)trader_params["strategy"]["type"];
                } else if ((string)trader_params["strategy"]["type"] == "position") {
                    this->name = (string)trader_params["strategy"]["type"];
                }
            }

            /**
             * @brief Set the stake amount 
             * 
             * @param amount Stake amount
             */
            void set_stake_amount(double amount) { this->stake_amount = amount; }

            /**
             * @brief Set the symbol abount object
             * 
             * @param amount Stake amount
             */
            void set_symbol_abount(double amount) { this->symbol_amount = amount; }

            /**
             * @brief Get the symbol
             * 
             * @return string 
             */
            string get_symbol() { return this->symbol; }

            /**
             * @brief Get the interval 
             * 
             * @return string 
             */
            string get_interval() { return this->interval; }

            /**
             * @brief Get the name of Trader
             * 
             * @return string 
             */
            string get_name() { return this->name; }

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
            double get_stake_amount() { return this->stake_amount; }

            /**
             * @brief Decide what to do with trade
             * 
             * @param buy_signal Buy signal
             * @param sell_signal Sell signal
             * @param ret_trades Array of returning trades
             * @param price Current price
             */
            void resolve(
                bool buy_signal, 
                bool sell_signal, 
                vector<Trade> &ret_trades,
                double price
            )
            {
                if (this->type == "position")
                {
                    if (sell_signal)
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
                            }
                        Trade trade;
                        this->open_trade(
                            trade, price,
                            buy_signal, sell_signal
                        );
                        this->trades.push_back(trade);
                    }
                    if (buy_signal)
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
                            }
                        Trade trade;
                        this->open_trade(
                            trade, price,
                            buy_signal, sell_signal
                        );
                        this->trades.push_back(trade);
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
                            }
                        }
                    }
                    if (buy_signal || sell_signal)
                    {
                        Trade trade;
                        this->open_trade(
                            trade, price,
                            buy_signal, sell_signal
                        );
                        this->trades.push_back(trade);
                    }
                }


                // if (this->type == "position")
                // {
                //     if (this->work)
                //     {
                //         if (sell_signal)
                //         {
                //             if (this->trade.get_position() == "long")
                //             {
                //                 this->close_trade(price);

                //                 // Do something with old trade
                //                 ret_trade = this->trade;

                //                 this->open_trade(
                //                     price,
                //                     buy_signal,
                //                     sell_signal
                //                 );
                //                 cout << "[+] Long trade is closed" << endl;
                //                 cout << "[+] Open new short trade" << endl;
                //                 // this->trade.describe_trade();
                //                 // cout << endl;                               
                //             }
                //             return;
                //         }
                //         if (buy_signal)
                //         {
                //             if (this->trade.get_position() == "short")
                //             {
                //                 this->close_trade(price);

                //                 // Do something with old trade
                //                 ret_trade = this->trade;

                //                 this->open_trade(
                //                     price,
                //                     buy_signal,
                //                     sell_signal
                //                 );
                //                 cout << "[+] Short trade is closed" << endl;
                //                 cout << "[+] Open new long trade" << endl;
                //                 // this->trade.describe_trade();
                //                 // cout << endl;
                //             }
                //             return;
                //         }
                //         return;
                //     } else {
                //         this->open_trade(
                //             price,
                //             buy_signal,
                //             sell_signal
                //         );
                //         if (buy_signal)
                //             cout << "[+] Open new long trade" << endl;
                //         else if (sell_signal)
                //             cout << "[+] Open new short trade" << endl;
                //         // this->trade.describe_trade();
                //         // cout << endl;
                //         return;
                //     }
                // }
                // if (
                //     this->type == "scalping"
                // )
                // {
                //     double current_price = price;
                //     double opened_price = this->trade.get_open_price();
                //     double percent = (current_price / opened_price - 1) * 100;
                //     if (this->work)
                //     {
                //         if (
                //             this->target <= percent || 
                //             this->stop_loss >= percent
                //         ) {
                //             this->close_trade(price);
                //             // Do something with Trade
                //             ret_trade = this->trade;

                //             cout << "[+] Trade is closed" << endl;
                //             this->clear_trade(); // Reset options
                //         }
                //         return;
                //     } else {
                //         if (buy_signal)
                //         {
                //             this->open_trade(
                //                 price,
                //                 buy_signal,
                //                 sell_signal
                //             );
                //             // Do something with Trade
                //             cout << "[+] Trade is opened" << endl;
                //         }
                //         return;
                //     }
                // }
            }
    };

    /**
     * @brief Solves the Strategy
     */
    template <class Strategy>
    class Solver
    {
        private:
            /**
             * @brief Params for Strategy
             */
            nlohmann::json strategy_params;

            /**
             * @brief Data container for resolved signals
             */
            map<string, bool> signals{
                {"buy", false}, {"sell", false}
            };

            /**
             * @brief EMA Cross Strategy object
             */
            Strategy strategy;

        public:
            /**
             * @brief Construct a new Solver 
             */
            Solver() = default;

            /**
             * @brief Construct a new Solver object
             * 
             * @param strategy_params Params for Strategy
             */
            Solver(
                nlohmann::json &strategy_params
            ) : strategy_params(strategy_params)
            { }

            /**
             * @brief Destroy the ema cross 15m object
             */
            ~Solver() = default;

            /**
             * @brief Get the strategy params object
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_strategy_params() { return this->strategy_params; }

            /**
             * @brief Get the buy signal 
             * 
             * @return true 
             * @return false 
             */
            bool get_buy_signal() { return signals["buy"]; }

            /**
             * @brief Get the sell signal 
             * 
             * @return true 
             * @return false 
             */
            bool get_sell_signal() { return signals["sell"]; }

            /**
             * @brief Resolve the strategy in worker
             */
            void resolve(nlohmann::json &params)
            {
                this->strategy.resolve(
                    params,
                    this->signals
                );
            }
    };

    /**
     * @brief Responsable for Indicators
     * 
     * @tparam Candle_T Type of Candle
     */
    class Indicator_Watcher
    {
        private:
            /**
             * @brief All type of EMA indicators (Candle)
             */
            vector<Indicators::Integral::EMA<Candle>> emas_cd;

            /**
             * @brief All type of EMA indicators (Heikin Ashi)
             */
            vector<Indicators::Integral::EMA<Heikin_Ashi>> emas_ha;

            /**
             * @brief All type of WMA indicators (Candle)
             */
            vector<Indicators::Integral::WMA<Candle>> wmas_cd;

            /**
             * @brief All type of WMA indicators (Heikin Ashi)
             */
            vector<Indicators::Integral::WMA<Heikin_Ashi>> wmas_ha;

            /**
             * @brief All type of SMA indicators (Candle)
             */
            vector<Indicators::Integral::SMA<Candle>> smas_cd;

            /**
             * @brief All type of SMA indicators (Heikin Ashi)
             */
            vector<Indicators::Integral::SMA<Heikin_Ashi>> smas_ha;

            /**
             * @brief All type of RSI indicators (Candle)
             */
            vector<Indicators::Integral::RSI<Candle>> rsis_cd;

            /**
             * @brief All type of RSI indicators (Heikin Ashi)
             */
            vector<Indicators::Integral::RSI<Heikin_Ashi>> rsis_ha;

            /**
             * @brief All type of Normalized MACD indicators (Candle)
             */
            vector<Indicators::TradingView::Normalized_MACD<Candle>> normalized_macds_cd;

            /**
             * @brief All type of Normalized MACD indicators (Heikin Ashi)
             */
            vector<Indicators::TradingView::Normalized_MACD<Heikin_Ashi>> normalized_macds_ha;

            /**
             * @brief All strategies in config
             */
            nlohmann::json strategies;

            /**
             * @brief Base URL of taapi.io
             */
            string taapi_url = "https://api.taapi.io/";

            /**
             * @brief API Key for taapi.io
             */
            string taapi_key;

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
                nlohmann::json &strategies,
                const string &taapi_key
            ) : taapi_key(taapi_key), strategies(strategies)
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
            void set_strategies(nlohmann::json &strat) { this->strategies = strat; }

            /**
             * @brief Set the taapi key 
             * 
             * @param key API Key for taapi.io
             */
            void set_taapi_key(const string &key) { this->taapi_key = key; }

            /**
             * @brief Describe all configured indicators
             * 
             * @tparam Candle_T Type of Candle
             */
            // template <class Candle_T>
            // void describe_indicators()
            // {
            //     cout << "EMA indicators:" << endl;
            //     for (Indicators::Integral::EMA<Candle_T>& ema : this->emas)
            //     {
            //         nlohmann::json desc = ema.get_description();
            //         cout << desc << endl;
            //     }
            //     cout << "Normalized MACD indicators:" << endl;
            //     for (Indicators::TradingView::Normalized_MACD<Candle_T>& n_macd : this->normalized_macds)
            //     {
            //         nlohmann::json desc = n_macd.get_description();
            //         cout << desc << endl;
            //     }
            // }

            /**
             * @brief Initialize indicators 
             */
            void initialize_indicators()
            {
                // TODO: Remake without try-catch. We pass "strategies" or "strategies.strategy.1.strategy_params" from config
                try {
                    for (auto& [key1, val1] : this->strategies.items())
                        for (auto& [key2, val2] : val1.items())
                        {
                            for (auto& [key3, val3] : val2["strategy_params"].items())
                            {
                                if (val3["indicator"] == "EMA")
                                {
                                    if (val3["candle"] == "Candle")
                                        this->emas_cd.push_back(
                                            Indicators::Integral::EMA<Candle>(
                                                val3["indicator_params"]
                                            )
                                        );
                                    if (val3["candle"] == "Heikin_Ashi")
                                        this->emas_ha.push_back(
                                            Indicators::Integral::EMA<Heikin_Ashi>(
                                                val3["indicator_params"]
                                            )
                                        );
                                }
                                if (val3["indicator"] == "Normalized_MACD")
                                {
                                    if (val3["candle"] == "Candle")
                                        this->normalized_macds_cd.push_back(
                                            Indicators::TradingView::Normalized_MACD<Candle>(
                                                val3["indicator_params"]
                                            )
                                        );
                                    if (val3["candle"] == "Heikin_Ashi")
                                        this->normalized_macds_ha.push_back(
                                            Indicators::TradingView::Normalized_MACD<Heikin_Ashi>(
                                                val3["indicator_params"]
                                            )
                                        );
                                }
                                if (val3["indicator"] == "WMA")
                                {
                                    if (val3["candle"] == "Candle")
                                        this->wmas_cd.push_back(
                                            Indicators::Integral::WMA<Candle>(
                                                val3["indicator_params"]
                                            )
                                        );
                                    if (val3["candle"] == "Heikin_Ashi")
                                        this->wmas_ha.push_back(
                                            Indicators::Integral::WMA<Heikin_Ashi>(
                                                val3["indicator_params"]
                                            )
                                        );
                                }
                                if (val3["indicator"] == "SMA")
                                {
                                    if (val3["candle"] == "Candle")
                                        this->smas_cd.push_back(
                                            Indicators::Integral::SMA<Candle>(
                                                val3["indicator_params"]
                                            )
                                        );
                                    if (val3["candle"] == "Heikin_Ashi")
                                        this->smas_ha.push_back(
                                            Indicators::Integral::SMA<Heikin_Ashi>(
                                                val3["indicator_params"]
                                            )
                                        );
                                }
                                if (val3["indicator"] == "RSI")
                                {
                                    if (val3["candle"] == "Candle")
                                        this->rsis_cd.push_back(
                                            Indicators::Integral::RSI<Candle>(
                                                val3["indicator_params"]
                                            )
                                        );
                                    if (val3["candle"] == "Heikin_Ashi")
                                        this->rsis_ha.push_back(
                                            Indicators::Integral::RSI<Heikin_Ashi>(
                                                val3["indicator_params"]
                                            )
                                        );
                                }
                                // if () // Other indicator
                            }
                        }
                } catch(nlohmann::detail::type_error& exp) {
                    cout << exp.what() << endl;
                    for (auto& [key, val] : this->strategies.items())
                    {
                        if (val["indicator"] == "EMA")
                        {
                            if (val["candle"] == "Candle")
                                this->emas_cd.push_back(
                                    Indicators::Integral::EMA<Candle>(
                                        val["indicator_params"]
                                    )
                                );
                            if (val["candle"] == "Heikin_Ashi")
                                this->emas_ha.push_back(
                                    Indicators::Integral::EMA<Heikin_Ashi>(
                                        val["indicator_params"]
                                    )
                                );
                        }
                        if (val["indicator"] == "WMA")
                        {
                            if (val["candle"] == "Candle")
                                this->wmas_cd.push_back(
                                    Indicators::Integral::WMA<Candle>(
                                        val["indicator_params"]
                                    )
                                );
                            if (val["candle"] == "Heikin_Ashi")
                                this->wmas_ha.push_back(
                                    Indicators::Integral::WMA<Heikin_Ashi>(
                                        val["indicator_params"]
                                    )
                                );
                        }
                        if (val["indicator"] == "SMA")
                        {
                            if (val["candle"] == "Candle")
                                this->smas_cd.push_back(
                                    Indicators::Integral::SMA<Candle>(
                                        val["indicator_params"]
                                    )
                                );
                            if (val["candle"] == "Heikin_Ashi")
                                this->smas_ha.push_back(
                                    Indicators::Integral::SMA<Heikin_Ashi>(
                                        val["indicator_params"]
                                    )
                                );
                        }
                        if (val["indicator"] == "RSI")
                        {
                            if (val["candle"] == "Candle")
                                this->rsis_cd.push_back(
                                    Indicators::Integral::RSI<Candle>(
                                        val["indicator_params"]
                                    )
                                );
                            if (val["candle"] == "Heikin_Ashi")
                                this->rsis_ha.push_back(
                                    Indicators::Integral::RSI<Heikin_Ashi>(
                                        val["indicator_params"]
                                    )
                                );
                        }
                        if (val["indicator"] == "Normalized_MACD")
                        {
                            if (val["candle"] == "Candle")
                                this->normalized_macds_cd.push_back(
                                    Indicators::TradingView::Normalized_MACD<Candle>(
                                        val["indicator_params"]
                                    )
                                );
                            if (val["candle"] == "Heikin_Ashi")
                                this->normalized_macds_ha.push_back(
                                    Indicators::TradingView::Normalized_MACD<Heikin_Ashi>(
                                        val["indicator_params"]
                                    )
                                );
                        }
                        // if () // Other indicator
                    }
                }
            }

            /**
             * @brief When next candle is set Watcher should be resolved
             * 
             * @param candle Candle object
             * @param heikin_ashi Heikin Ashi candle object
             */
            void resolve(Candle &candle, Heikin_Ashi &heikin_ashi)
            {
                for (auto& ema : this->emas_cd)
                    ema.resolve(candle);
                for (auto& wma : this->wmas_cd)
                    wma.resolve(candle);
                for (auto& sma : this->smas_cd)
                    sma.resolve(candle);
                for (auto& rsi : this->rsis_cd)
                    rsi.resolve(candle);
                for (auto& n_macd : this->normalized_macds_cd)
                    n_macd.resolve(candle);

                for (auto& ema : this->emas_ha)
                    ema.resolve(heikin_ashi);
                for (auto& wma : this->wmas_ha)
                    wma.resolve(heikin_ashi);
                for (auto& sma : this->smas_ha)
                    sma.resolve(heikin_ashi);
                for (auto& rsi : this->rsis_ha)
                    rsi.resolve(heikin_ashi);
                for (auto& n_macd : this->normalized_macds_ha)
                    n_macd.resolve(heikin_ashi);
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

                    // TODO: Create instance from config. Without if-else
                    if (type == "Indicators::TAAPI" && !backtest)
                    {
                        if (indicator == "EMA")
                        {
                        again:
                            try {
                                Indicators::TAAPI::EMA ema;
                                params[key] = ema.get(
                                    this->taapi_url,
                                    this->taapi_key, symbol,
                                    val["indicator_params"]["interval"], 
                                    val["indicator_params"]
                                );
                            } catch (Exceptions::TAAPI::Rate_Limit& exp) {
                                cout << exp.what() << endl;
                                // TODO: Pass delay from config
                                std::this_thread::sleep_for(
                                    std::chrono::seconds(
                                        15
                                    )
                                );
                                goto again;
                            }
                        }
                    }
                    if (
                        type == "Indicators::Integral" || 
                        backtest
                    )
                    {
                        if (indicator == "EMA")
                        {
                            for (auto& [key, val] : strategy_params.items())
                            {
                                if (val["candle"] == "Candle")
                                    for (auto &ema : this->emas_cd)
                                    {
                                        if (ema.get_description() == val["indicator_params"])
                                            params[key] = ema.get();
                                    }
                                if (val["candle"] == "Heikin_Ashi")
                                    for (auto &ema : this->emas_ha)
                                    {
                                        if (ema.get_description() == val["indicator_params"])
                                            params[key] = ema.get();
                                    }
                            }
                        }
                        if (indicator == "WMA")
                        {
                            for (auto& [key, val] : strategy_params.items())
                            {
                                if (val["candle"] == "Candle")
                                    for (auto &wma : this->wmas_cd)
                                    {
                                        if (wma.get_description() == val["indicator_params"])
                                            params[key] = wma.get();
                                    }
                                if (val["candle"] == "Heikin_Ashi")
                                    for (auto &wma : this->wmas_ha)
                                    {
                                        if (wma.get_description() == val["indicator_params"])
                                            params[key] = wma.get();
                                    }
                            }
                        }
                        if (indicator == "SMA")
                        {
                            for (auto& [key, val] : strategy_params.items())
                            {
                                if (val["candle"] == "Candle")
                                    for (auto &sma : this->smas_cd)
                                    {
                                        if (sma.get_description() == val["indicator_params"])
                                            params[key] = sma.get();
                                    }
                                if (val["candle"] == "Heikin_Ashi")
                                    for (auto &sma : this->smas_ha)
                                    {
                                        if (sma.get_description() == val["indicator_params"])
                                            params[key] = sma.get();
                                    }
                            }
                        }
                        if (indicator == "RSI")
                        {
                            for (auto& [key, val] : strategy_params.items())
                            {
                                if (val["candle"] == "Candle")
                                    for (auto &rsi : this->rsis_cd)
                                    {
                                        if (rsi.get_description() == val["indicator_params"])
                                            params[key] = rsi.get();
                                    }
                                if (val["candle"] == "Heikin_Ashi")
                                    for (auto &rsi : this->rsis_ha)
                                    {
                                        if (rsi.get_description() == val["indicator_params"])
                                            params[key] = rsi.get();
                                    }
                            }
                        }
                    }
                    if (type == "Indicators::TradingView" || backtest)
                    {
                        if (indicator == "Normalized_MACD")
                        {
                            if (val["candle"] == "Candle")
                                for (auto &n_macd : this->normalized_macds_cd)
                                {
                                    if (n_macd.get_description() == val["indicator_params"])
                                        params[key] = n_macd.get();
                                }
                            if (val["candle"] == "Heikin_Ashi")
                                for (auto &n_macd : this->normalized_macds_ha)
                                {
                                    if (n_macd.get_description() == val["indicator_params"])
                                        params[key] = n_macd.get();
                                }
                        }
                    }
                }

                return params;
            }
    };

    /**
     * @brief Responsable for Candles
     */
    class Candle_Watcher
    {
        private:
            /**
             * @brief Standard Candle
             */
            Candle candle;

            /**
             * @brief Heikin Ashi Candle
             */
            Heikin_Ashi heikin_ashi;

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
                        this->candle.construct(
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

                        if (this->prev_ha_close == 0 || prev_ha_open == 0)
                            this->heikin_ashi.construct(this->candle, this->candle.get_close_price(), this->candle.get_open_price());
                        else
                            this->heikin_ashi.construct(this->candle, this->prev_ha_close, this->prev_ha_open);
                        this->prev_ha_close = this->heikin_ashi.get_close_price();
                        this->prev_ha_open = this->heikin_ashi.get_open_price();

                        return true;
                    } catch (std::invalid_argument& exp) {
                        cout << exp.what() << endl;
                        return false;
                    }
                } else {
                    return false;
                }
            }

            // template <class Candle_T>
            // Candle_T get_candle() 
            // {
            //     if (std::is_same<Candle_T, Candle>::value)
            //         return this->candle;
            //     if (std::is_same<Candle_T, Heikin_Ashi>::value)
            //         return this->heikin_ashi;
            // }

            /**
             * @brief Get the candle object
             * 
             * @param candle_type Type of Candle
             * @return Candle_T 
             */
            Candle get_candle() { return this->candle; }

            /**
             * @brief Get the heikin ashi object
             * 
             * @return Heikin_Ashi 
             */
            Heikin_Ashi get_heikin_ashi() { return this->heikin_ashi; }

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


















#endif