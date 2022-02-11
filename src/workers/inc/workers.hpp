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
#include "../../candles/inc/candles.hpp"
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
             * @brief What positions are traded
             */
            string positions;

            /**
             * @brief Goal of short trade (USDT or symbol)
             */
            string short_goal;

            /**
             * @brief Maximum opened trades
             */
            double max_open_trade;

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
                const string& position,
                double price = 0
            )
            {
                // TODO: Unique ID for each Trade
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
             */
            Trader(
                nlohmann::json &trader_params
            )
            { 
                this->symbol = (string)trader_params["symbol"];
                this->interval = (string)trader_params["interval"];
                this->name = (string)trader_params["name"];
                this->exchange = (string)trader_params["exchange"];
                this->positions = (string)trader_params["strategy"]["positions"];
                this->short_goal = (string)trader_params["strategy"]["short_goal"];

                this->stake_amount = (double)trader_params["strategy"]["stake_amount"];
                this->symbol_amount = (double)trader_params["strategy"]["symbol_amount"];
                this->max_open_trade = (double)trader_params["strategy"]["max_open_trade"];

                if ((string)trader_params["strategy"]["type"] == "scalping")
                {
                    this->type = (string)trader_params["strategy"]["type"];
                    this->stop_loss = (double)trader_params["strategy"]["stop-loss"];
                    this->target = (double)trader_params["strategy"]["target"];
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
                this->symbol = (string)trader_params["symbol"];
                this->interval = (string)trader_params["interval"];
                this->name = (string)trader_params["name"];
                this->exchange = (string)trader_params["exchange"];
                this->positions = (string)trader_params["strategy"]["positions"];
                this->short_goal = (string)trader_params["strategy"]["short_goal"];

                this->stake_amount = (double)trader_params["strategy"]["stake_amount"];
                this->symbol_amount = (double)trader_params["strategy"]["symbol_amount"];
                this->max_open_trade = (double)trader_params["strategy"]["max_open_trade"];

                if ((string)trader_params["strategy"]["type"] == "scalping")
                {
                    this->type = (string)trader_params["strategy"]["type"];
                    this->stop_loss = (double)trader_params["strategy"]["stop-loss"];
                    this->target = (double)trader_params["strategy"]["target"];
                } else if ((string)trader_params["strategy"]["type"] == "position") {
                    this->type = (string)trader_params["strategy"]["type"];
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
                            cout << "[+] Opened short trade" << endl;
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
                            cout << "[+] Opened long trade" << endl;
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
                {"long_open", false}, 
                {"long_close", false},
                {"short_open", false},
                {"short_close", false}
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
             * @brief Get the signals object
             * 
             * @return map<string, bool> 
             */
            map<string, bool> get_signals() { return this->signals; }

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
    template <class Candle_T>
    class Indicator_Watcher
    {
        private:
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

            /**
             * @brief Initialize indicator
             * 
             * @param strategy_params Parameters of indicator
             */
            void initialize_indicator(nlohmann::json &strategy_params)
            {
                // TODO: Create instance from config. Without if-else
                for (auto& [key, val] : strategy_params.items())
                {
                    if (val["indicator"] == "EMA")
                        this->all_ema.push_back(
                            Indicators::Integral::EMA<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "WMA")
                        this->all_wma.push_back(
                            Indicators::Integral::WMA<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "SMA")
                        this->all_sma.push_back(
                            Indicators::Integral::SMA<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "SSMA")
                        this->all_ssma.push_back(
                            Indicators::Integral::SSMA<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "RSI")
                        this->all_rsi.push_back(
                            Indicators::Integral::RSI<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "TR")
                        this->all_tr.push_back(
                            Indicators::Integral::TR<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "ATR")
                        this->all_atr.push_back(
                            Indicators::Integral::ATR<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "DMI")
                        this->all_dmi.push_back(
                            Indicators::Integral::DMI<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "ADX")
                        this->all_adx.push_back(
                            Indicators::Integral::ADX<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "Normalized_MACD")
                        this->all_normalized_macd.push_back(
                            Indicators::TradingView::Normalized_MACD<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                    if (val["indicator"] == "RSXC_LB")
                        this->all_rsxc_lb.push_back(
                            Indicators::TradingView::RSXC_LB<Candle_T>(
                                val["indicator_params"]
                            )
                        );
                }
            }

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
             * @brief Initialize indicators 
             */
            void initialize_indicators()
            {
                // TODO: Remake without try-catch. We pass "strategies" or "strategies.strategy.1.strategy_params" from config
                try {
                    for (auto& [key1, val1] : this->strategies.items())
                        for (auto& [key2, val2] : val1.items())
                            this->initialize_indicator(val2["strategy_params"]);
                } catch(nlohmann::detail::type_error& exp) {
                    cout << exp.what() << endl;
                    this->initialize_indicator(this->strategies);
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
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &ema : this->all_ema)
                                    if (ema.get_description() == val["indicator_params"])
                                        params[key] = ema.get();
                        if (indicator == "SMA")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &sma : this->all_sma)
                                    if (sma.get_description() == val["indicator_params"])
                                        params[key] = sma.get();
                        if (indicator == "SSMA")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &ssma : this->all_ssma)
                                    if (ssma.get_description() == val["indicator_params"])
                                        params[key] = ssma.get();
                        if (indicator == "WMA")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &wma : this->all_wma)
                                    if (wma.get_description() == val["indicator_params"])
                                        params[key] = wma.get();
                        if (indicator == "RSI")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &rsi : this->all_rsi)
                                    if (rsi.get_description() == val["indicator_params"])
                                        params[key] = rsi.get();
                        if (indicator == "TR")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &tr : this->all_tr)
                                    if (tr.get_description() == val["indicator_params"])
                                        params[key] = tr.get();
                        if (indicator == "ATR")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &atr : this->all_atr)
                                    if (atr.get_description() == val["indicator_params"])
                                        params[key] = atr.get();
                        if (indicator == "DMI")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &dmi : this->all_dmi)
                                    if (dmi.get_description() == val["indicator_params"])
                                        params[key] = dmi.get();
                        if (indicator == "ADX")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &adx : this->all_adx)
                                    if (adx.get_description() == val["indicator_params"])
                                        params[key] = adx.get();
                    }
                    if (type == "Indicators::TradingView" || backtest)
                    {
                        if (indicator == "Normalized_MACD")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &n_macd : this->all_normalized_macd)
                                    if (n_macd.get_description() == val["indicator_params"])
                                        params[key] = n_macd.get();
                        if (indicator == "RSXC_LB")
                            for (auto& [key, val] : strategy_params.items())
                                for (auto &rsx : this->all_rsxc_lb)
                                    if (rsx.get_description() == val["indicator_params"])
                                        params[key] = rsx.get();
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
            Candles::Candle candle;

            /**
             * @brief Heikin Ashi Candle
             */
            Candles::Heikin_Ashi heikin_ashi;

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
            bool operator ==(const Workers::Candle_Watcher& cw)
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
             * @param candle_type Type of Candle
             * @return Candle_T 
             */
            Candles::Candle get_candle() { return this->candle; }

            /**
             * @brief Get the heikin ashi object
             * 
             * @return Heikin_Ashi 
             */
            Candles::Heikin_Ashi get_heikin_ashi() { return this->heikin_ashi; }

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