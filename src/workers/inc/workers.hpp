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

#include "../../strategies/inc/strategies.hpp"
#include "../../indicators/inc/indicators.hpp"
#include "../../candle/inc/candle.hpp"
#include "../../exceptions/inc/exceptions.hpp"
#include "../../trade/inc/trade.hpp"
#include "../../request/inc/request.hpp"

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
        // TODO: Open several trades
        // TODO: Short position for scalping type of trade (requires several trades)
        private:
            /**
             * @brief Active Trade for Worker
             */
            Trade trade;

            /**
             * @brief Symbol (pair) to trade
             */
            string symbol;

            /**
             * @brief Timeframe (interval) of candles
             */
            string timeframe;

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
             * @brief Work Trade Status
             */
            bool work = false;

            /**
             * @brief Get the current price of cryptocurrency
             * 
             * @return double 
             */
            double get_current_price()
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

            /**
             * @brief Initialize new Trade
             */
            void initialize_trade(
                double price = 0,
                bool buy_signal = false,
                bool sell_signal = false
            )
            {
                // TODO: Unique ID for each Trade
                this->trade.set_id(1);
                this->trade.set_active(true);
                this->trade.set_open_time();
                this->trade.set_symbol(
                    this->symbol
                );
                if (buy_signal)
                    this->trade.set_position("long");
                if (sell_signal)
                    this->trade.set_position("short");
                this->trade.set_stake_amount(
                    this->stake_amount
                );
                this->trade.set_symbol_amount(
                    this->symbol_amount
                );
                this->trade.set_open_price(
                    price
                );
                this->trade.set_interval(
                    this->timeframe
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
                double open_price,
                bool buy_signal = false,
                bool sell_signal = false
            )
            {
                if (!buy_signal && !sell_signal)
                    return;
                this->initialize_trade(
                    open_price, 
                    buy_signal,
                    sell_signal
                );
                this->work = true;
                // TODO: Open trade in binance
            }

            /**
             * @brief Close current Trade
             */
            void close_trade(double close_price)
            {
                this->trade.set_close_time(
                    close_price
                );
                this->work = false;
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
             * @param trader_params Parameters of Trader
             */
            Trader(
                nlohmann::json &trader_params
            )
            { 
                this->symbol = (string)trader_params["symbol"];
                this->timeframe = (string)trader_params["timeframe"];
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
                this->timeframe = trader_params["timeframe"];
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
            string get_timeframe() { return this->timeframe; }

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
             * @brief Is Trade working?
             * 
             * @return true 
             * @return false 
             */
            bool is_work() { return this->work; }

            /**
             * @brief Decide what to do with trade
             * 
             * @param buy_signal Buy signal
             * @param sell_signal Sell signal
             * @param ret_trade Trade object to return
             * @param candle Pointer to last candle
             */
            void resolve(
                bool buy_signal, 
                bool sell_signal, 
                Trade &ret_trade,
                Candle* candle = NULL
            )
            {
                double price;
                if (candle != NULL)
                    price = candle->get_close_price();
                else
                    price = this->get_current_price();
                if (this->type == "position")
                {
                    if (this->work)
                    {
                        if (sell_signal)
                        {
                            if (this->trade.get_position() == "long")
                            {
                                this->close_trade(price);

                                // Do something with old trade
                                ret_trade = this->trade;

                                this->open_trade(
                                    price,
                                    buy_signal,
                                    sell_signal
                                );
                                cout << "[+] Long trade is closed" << endl;
                                cout << "[+] Open new short trade" << endl;
                                // this->trade.describe_trade();
                                // cout << endl;                               
                            }
                            return;
                        }
                        if (buy_signal)
                        {
                            if (this->trade.get_position() == "short")
                            {
                                this->close_trade(price);

                                // Do something with old trade
                                ret_trade = this->trade;

                                this->open_trade(
                                    price,
                                    buy_signal,
                                    sell_signal
                                );
                                cout << "[+] Short trade is closed" << endl;
                                cout << "[+] Open new long trade" << endl;
                                // this->trade.describe_trade();
                                // cout << endl;
                            }
                            return;
                        }
                        return;
                    } else {
                        this->open_trade(
                            price,
                            buy_signal,
                            sell_signal
                        );
                        if (buy_signal)
                            cout << "[+] Open new long trade" << endl;
                        else if (sell_signal)
                            cout << "[+] Open new short trade" << endl;
                        // this->trade.describe_trade();
                        // cout << endl;
                        return;
                    }
                }
                if (
                    this->type == "scalping"
                )
                {
                    double current_price = candle->get_close_price();
                    double opened_price = this->trade.get_open_price();
                    double percent = (current_price / opened_price - 1) * 100;
                    if (this->work)
                    {
                        if (
                            this->target <= percent || 
                            this->stop_loss >= percent
                        ) {
                            this->close_trade(price);
                            // Do something with Trade
                            ret_trade = this->trade;

                            cout << "[+] Trade is closed" << endl;
                            this->clear_trade(); // Reset options
                        }
                        return;
                    } else {
                        if (buy_signal)
                        {
                            this->open_trade(
                                price,
                                buy_signal,
                                sell_signal
                            );
                            // Do something with Trade
                            cout << "[+] Trade is opened" << endl;
                        }
                        return;
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
     */
    class Watcher
    {
        private:
            /**
             * @brief All type of EMA indicators
             */
            vector<Indicators::Integral::EMA> emas;

            /**
             * @brief All type of WMA indicators
             */
            vector<Indicators::Integral::WMA> wmas;

            /**
             * @brief All type of SMA indicators
             */
            vector<Indicators::Integral::SMA> smas;

            /**
             * @brief All type of Normalized MACD indicators
             */
            vector<Indicators::TradingView::Normalized_MACD> normalized_macds;

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
             * @brief Construct a new Watcher object
             */
            Watcher() = default;

            /**
             * @brief Construct a new Watcher object
             * 
             * @param strategies All strategies in config
             * @param taapi_key API Key for taapi.io
             */
            Watcher(
                nlohmann::json &strategies,
                const string &taapi_key
            ) : taapi_key(taapi_key), strategies(strategies)
            { }

            /**
             * @brief Destroy the Watcher object
             */
            ~Watcher() = default;

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
             */
            void describe_indicators()
            {
                cout << "EMA indicators:" << endl;
                for (Indicators::Integral::EMA& ema : this->emas)
                {
                    nlohmann::json desc = ema.get_description();
                    cout << desc << endl;
                }
                cout << "Normalized MACD indicators:" << endl;
                for (Indicators::TradingView::Normalized_MACD& n_macd : this->normalized_macds)
                {
                    nlohmann::json desc = n_macd.get_description();
                    cout << desc << endl;
                }
            }

            /**
             * @brief Initialize the Watcher
             */
            void initialize()
            {
                // TODO: Remake without try-catch. We pass "strategies" or "strategies.strategy.1.strategy_params" from config
                try {
                    for (auto& [key1, val1] : this->strategies.items())
                        for (auto& [key2, val2] : val1.items())
                        {
                            for (auto& [key3, val3] : val2["strategy_params"].items())
                            {
                                if (val3["indicator"] == "EMA")
                                    this->emas.push_back(
                                        Indicators::Integral::EMA(
                                            val3["indicator_params"]
                                        )
                                    );
                                if (val3["indicator"] == "Normalized_MACD")
                                    this->normalized_macds.push_back(
                                        Indicators::TradingView::Normalized_MACD(
                                            val3["indicator_params"]
                                        )
                                    );
                                if (val3["indicator"] == "WMA")
                                    this->wmas.push_back(
                                        Indicators::Integral::WMA(
                                            val3["indicator_params"]
                                        )
                                    );
                                if (val3["indicator"] == "SMA")
                                    this->smas.push_back(
                                        Indicators::Integral::SMA(
                                            val3["indicator_params"]
                                        )
                                    );
                                // if () // Other indicator
                            }
                        }
                } catch(nlohmann::detail::type_error& exp) {
                    cout << exp.what() << endl;
                    for (auto& [key, val] : this->strategies.items())
                    {
                        if (val["indicator"] == "EMA")
                            this->emas.push_back(
                                Indicators::Integral::EMA(
                                    val["indicator_params"]
                                )
                            );
                        if (val["indicator"] == "WMA")
                            this->wmas.push_back(
                                Indicators::Integral::WMA(
                                    val["indicator_params"]
                                )
                            );
                        if (val["indicator"] == "SMA")
                            this->smas.push_back(
                                Indicators::Integral::SMA(
                                    val["indicator_params"]
                                )
                            );
                        if (val["indicator"] == "Normalized_MACD")
                            this->normalized_macds.push_back(
                                Indicators::TradingView::Normalized_MACD(
                                    val["indicator_params"]
                                )
                            );
                        // if () // Other indicator
                    }
                }
                this->describe_indicators();
            }

            /**
             * @brief When next candle is set Watcher should be resolved
             */
            void resolve(Candle &candle)
            {
                for (Indicators::Integral::EMA& ema : this->emas)
                    ema.resolve(candle);
                for (Indicators::TradingView::Normalized_MACD& n_macd : this->normalized_macds)
                    n_macd.resolve(candle);
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
                                for (Indicators::Integral::EMA &ema : this->emas)
                                {
                                    if (ema.get_description() == val["indicator_params"])
                                        params[key] = ema.get();
                                }
                            }
                        }
                    }
                    if (
                        type == "Indicators::TradingView" || 
                        backtest
                    )
                    {
                        if (indicator == "Normalized_MACD")
                        {
                            for (Indicators::TradingView::Normalized_MACD &n_macd : this->normalized_macds)
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
}


















#endif