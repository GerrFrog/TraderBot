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
    // TODO: Trader opens several trades
    /**
     * @brief Trader decides when open and close order
     */
    class Trader
    {
        private:
            /**
             * @brief Parameters of Trader
             */
            nlohmann::json trader_params;

            /**
             * @brief Active Trade for Worker
             */
            Trade trade;

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
                string symbol = this->trader_params["symbol"];

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
            void initialize_trade()
            {
                string symbol = this->trader_params["symbol"];
                string timeframe = this->trader_params["timeframe"];

                double stake_amount = this->trader_params["stake_amount"];

                this->trade.set_open_time();
                // TODO: Unique ID for each Trade
                this->trade.set_id(1);
                this->trade.set_symbol(
                    symbol
                );
                this->trade.set_stake_amount(
                    stake_amount
                );
                this->trade.set_open_price(
                    this->get_current_price()
                );
                this->trade.set_interval(
                    timeframe
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
            void open_trade()
            {
                this->initialize_trade();
                this->work = true;
                // TODO: Open trade in binance
            }

            /**
             * @brief Close current Trade
             */
            void close_trade()
            {
                this->trade.set_close_time(
                    this->get_current_price()
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
            ) : trader_params(trader_params)
            { }

            /**
             * @brief Destroy the Trader object
             */
            ~Trader() = default;

            /**
             * @brief Set the trader params object
             * 
             * @param params Params for Trader
             */
            void set_trader_params(nlohmann::json &params) { this->trader_params = params; }

            /**
             * @brief Get the symbol
             * 
             * @return string 
             */
            string get_symbol() { return this->trader_params["symbol"]; }

            /**
             * @brief Get the interval 
             * 
             * @return string 
             */
            string get_timeframe() { return this->trader_params["timeframe"]; }

            /**
             * @brief Get the name of Trader
             * 
             * @return string 
             */
            string get_name() { return this->trader_params["name"]; }

            /**
             * @brief Get the exchange 
             * 
             * @return string 
             */
            string get_exchange() { return this->trader_params["exchange"]; }

            /**
             * @brief Get the stake amount
             * 
             * @return double 
             */
            double get_stake_amount() { return (double)this->trader_params["stake_amount"]; }

            /**
             * @brief Is Trade working?
             * 
             * @return true 
             * @return false 
             */
            bool is_work() { return this->work; }

            // TODO: Other work: with stop-loss and profit
            /**
             * @brief Decide what to do with trade
             * 
             * @param buy_signal Buy signal
             * @param sell_signal Sell signal
             */
            void resolve(bool buy_signal, bool sell_signal)
            {
                if (this->work)
                {
                    if (sell_signal) 
                    {
                        this->close_trade();
                        // Do something with Trade
                        cout << "[+] Trade is closed" << endl;
                        this->clear_trade(); // Reset options
                    }
                    return;
                } else {
                    if (buy_signal)
                    {
                        this->open_trade();
                        // Do something with Trade
                        cout << "[+] Trade is opened" << endl;
                    }
                    return;
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
            void resolve(map<string, double> &params)
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
             * @brief Describe all indicators
             */
            void describe_indicators()
            {
                for (Indicators::Integral::EMA& ema : this->emas)
                    cout << "EMA : " << ema.get_period() << endl;
            }

            /**
             * @brief Initialize the Watcher
             */
            void initialize()
            {
                cout << "HERE" << endl;
                // TODO: Remake without try-catch. We pass "strategies" or "strategies.strategy.1.strategy_params" from config
                try {
                    for (auto& [key1, val1] : this->strategies.items())
                        for (auto& [key2, val2] : val1.items())
                            for (auto& [key3, val3] : val2["strategy_params"].items())
                            {
                                if (val3["indicator"] == "EMA")
                                    this->emas.push_back(
                                        Indicators::Integral::EMA(
                                            val3["indicator_params"]
                                        )
                                    );
                                // if () // Other indicator
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
                        // if () // Other indicator
                    }
                }

            }

            /**
             * @brief When next candle is set Watcher should be resolved
             */
            void resolve(Candle &candle)
            {
                for (Indicators::Integral::EMA& ema : this->emas)
                    ema.resolve(candle);
            }

            /**
             * @brief Get values for specified indicators
             * 
             * @param strategy_params strategy_params in config
             * @param symbol Symbol (pair) to trade
             * @param backtest If backtest all Indicators will be calculated as Integral
             * @return map<string, double>
             */
            map<string, double> get(
                nlohmann::json &strategy_params, 
                const string &symbol,
                bool backtest = false
            )
            {
                map<string, double> params;

                for (auto& [key, val] : strategy_params.items())
                {
                    double period = val["indicator_params"]["period"];

                    string indicator = val["indicator"];
                    string type = val["type"];

                    // TODO: Create instance from config. Without if-else
                    if (type == "TAAPI" && !backtest)
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
                    if (type == "Integral" || backtest)
                    {
                        if (indicator == "EMA")
                        {
                            for (auto& [key, val] : strategy_params.items())
                            {
                                for (Indicators::Integral::EMA &ema : this->emas)
                                {
                                    double period = val["indicator_params"]["period"];

                                    string interval = val["indicator_params"]["interval"];

                                    if (ema.get_period() == period && ema.get_interval() == interval)
                                        params[key] = ema.get_ema();
                                }
                            }
                        }
                    }
                }

                return params;
            }
    };
}


















#endif