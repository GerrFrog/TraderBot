#pragma once

#ifndef INDICATORS_HEADER
#define INDICATORS_HEADER

#include <string>
#include <nlohmann/json.hpp>
#include <map>
#include <iostream>
#include <vector>

#include "../../exceptions/inc/exceptions.hpp"
#include "../../request/inc/request.hpp"
#include "../../candle/inc/candle.hpp"

using std::string; 
using std::map; 
using std::cout; 
using std::endl;
using std::vector;

// TODO: Group indicators by "trend", "oscillator", "volatility" and "support/resistance"

/**
 * @brief Requests to taapi.io for indicators values
 */
namespace Indicators::TAAPI
{
    /**
     * @brief Get the taapi url object
     * 
     * @return std::string 
     */
    std::string get_taapi_url();

    /**
     * @brief Exponential Moving Average
     */
    class EMA
    {
        private:

        public:
            /**
             * @brief Construct a new EMA object
             */
            EMA() = default;

            /**
             * @brief Destroy the EMA object
             */
            ~EMA() = default;

            /**
             * @brief Get EMA value
             * 
             * @param key API Key for taapi.io
             * @param symbol Symbol (pair)
             * @param interval Interval
             * @param indicator_params Params for Indicator
             * @return double 
             */
            nlohmann::json get(
                const std::string &url,
                const std::string &key,
                const std::string &symbol, 
                const std::string &interval, 
                nlohmann::json &indicator_params
            );
    };
}

/**
 * @brief Integral indicators. Pass candle to calculate current value
 */
namespace Indicators::Integral
{
    // TODO: EMA with different sources (close, low, high...)
    /**
     * @brief Exponential Moving Average
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class EMA
    {
        private:
            /**
             * @brief Description of indicator
             */
            nlohmann::json description;

            /**
             * @brief Period of EMA
             */
            int period;

            /**
             * @brief Last EMA value
             */
            double last_ema;

            /**
             * @brief Current EMA for Last passed Candle
             */
            double ema = 0.0;

            /**
             * @brief Last Candles
             */
            vector<Candle_T> last_candles;

            /**
             * @brief Return JSON
             */
            nlohmann::json ret;

        public:
            /**
             * @brief Construct a new EMA object
             */
            EMA()
            { }

            /**
             * @brief Construct a new EMA object
             * 
             * @param indicator_params Params for EMA
             */
            EMA(
                nlohmann::json &indicator_params
            ) : description(indicator_params)
            { 
                this->period = indicator_params["period"];
            }

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;

                this->period = indicator_params["period"];
            }

            /**
             * @brief Get the description 
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->description; }

            /**
             * @brief Get the EMA for last passed Candle
             * 
             * @return nlohmann::json
             */
            nlohmann::json get() { return ret; }

            /**
             * @brief Resolve the EMA Indicator for new Candle
             * 
             * @param candle Candle object
             */
            void resolve(Candle_T &candle)
            {
                if (this->last_candles.size() < this->period)
                {
                    double curr = 0;

                    last_candles.push_back(candle);

                    for (Candle_T &last_candle : this->last_candles)
                        curr += last_candle.get_close_price() / this->last_candles.size();

                    this->ema = curr;
                    this->last_ema = curr;
                } else {
                    this->ema = 
                        (candle.get_close_price() - this->last_ema) * 2.0
                        / (this->period + 1.0) + this->last_ema
                    ;
                    this->last_ema = this->ema;
                }
                ret["value"] = this->ema;
            }
    };

    /**
     * @brief Weighted Moving Average
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class WMA
    {
        // TODO: WMA with different source (close, low, high...)
        private:
            /**
             * @brief Description of WMA
             */
            nlohmann::json description;

            /**
             * @brief Period of WMA
             */
            int period;

            /**
             * @brief WMA for last passed Candle
             */
            double wma = 0.0;

            /**
             * @brief Last Candles
             */
            vector<Candle_T> last_candles;

            /**
             * @brief Return JSON
             */
            nlohmann::json ret;

        public:
            /**
             * @brief Construct a new WMA object
             */
            WMA() = default;

            /**
             * @brief Construct a new WMA object
             * 
             * @param indicator_params Parameters for indicator
             */
            WMA(
                nlohmann::json &indicator_params
            ) : description(indicator_params)
            {
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the WMA object
             */
            ~WMA() = default;

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for Indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;

                this->period = indicator_params["period"];
            }

            /**
             * @brief Get the description 
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->description; }

            /**
             * @brief Get the WMA for last passed Candle
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get() { return this->ret; }

            /**
             * @brief Resolve the WMA
             * 
             * @param candle Candle
             */
            void resolve(Candle_T &candle)
            {
                if (this->last_candles.size() < this->period)
                {
                    this->last_candles.push_back(candle);
                } else {
                    this->last_candles.push_back(candle);
                    this->last_candles.erase(this->last_candles.begin());
                }
                int length = this->last_candles.size();
                for (int i = 0; i < length; i++)
                    this->wma += this->last_candles[i].get_close_price() * (i + 1);
                this->wma /= length * (length + 1) / 2;
                ret["value"] = this->wma;
                this->wma = 0;
            }       
    };

    /**
     * @brief Simple Moving Average
     * 
     * @tparam Candle_T Typeo of Candle
     */
    template <class Candle_T>
    class SMA
    {
        // TODO: SMA with different source (close, high, low...)
        private:
            /**
             * @brief Description of SMA
             */
            nlohmann::json description;

            /**
             * @brief SMA for last passed Candle
             */
            double sma = 0.0;

            /**
             * @brief Period of SMA
             */
            int period;

            /**
             * @brief Last Candles
             */
            vector<Candle_T> last_candles;

            /**
             * @brief Return JSON
             */
            nlohmann::json ret;

        public:
            /**
             * @brief Construct a new SMA object
             */
            SMA() = default;

            /**
             * @brief Construct a new SMA object
             * 
             * @param indicator_params Parameters for indicator
             */
            SMA(
                nlohmann::json &indicator_params
            ) : description(indicator_params)
            {
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the SMA object
             */
            ~SMA() = default;

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for Indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;

                this->period = indicator_params["period"];
            }

            /**
             * @brief Get the description 
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->description; }

            /**
             * @brief Get the SMA for last passed Candle
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get() { return ret; }

            /**
             * @brief Resolve the SMA
             * 
             * @param candle Candle
             */
            void resolve(Candle_T &candle)
            {
                if (this->last_candles.size() < period)
                {
                    this->last_candles.push_back(candle);
                } else {
                    this->last_candles.push_back(candle);
                    this->last_candles.erase(this->last_candles.begin());
                }
                int length = this->last_candles.size();
                for (Candle_T& last_candle : this->last_candles)
                    this->sma += last_candle.get_close_price();
                this->sma /= length;
                this->ret["value"] = this->sma;
                this->sma = 0;
            }       
    };

    /**
     * @brief Running Moving Average/Modified Moving Average/Smoothed Moving Average
     * 
     * @param Candle_T Type of Candle
     */
    template <class Candle_T>
    class SSMA
    {
        private:
            /**
             * @brief Description of indicator
             */
            nlohmann::json description;

            /**
             * @brief Period of EMA
             */
            int period;

            /**
             * @brief Last RMA value
             */
            double last_ssma;

            /**
             * @brief Current RMA for Last passed Candle
             */
            double ssma = 0.0;

            /**
             * @brief Last Candles
             */
            vector<Candle_T> last_candles;

            /**
             * @brief Return JSON
             */
            nlohmann::json ret;

        public:
            /**
             * @brief Construct a new RMA object
             */
            SSMA() = default;

            /**
             * @brief Construct a new RMA object
             * 
             * @param indicator_params Params for RMA
             */
            SSMA(
                nlohmann::json &indicator_params
            ) : description(indicator_params)
            { 
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the SSMA object
             */
            ~SSMA() = default;

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;

                this->period = indicator_params["period"];
            }

            /**
             * @brief Get the description 
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->description; }

            /**
             * @brief Get the EMA for last passed Candle
             * 
             * @return nlohmann::json
             */
            nlohmann::json get() { return ret; }

            /**
             * @brief Resolve the RMA Indicator for new Candle
             * 
             * @param candle Candle object
             */
            void resolve(Candle_T &candle)
            {
                if (this->last_candles.size() < this->period)
                {
                    double curr = 0;

                    last_candles.push_back(candle);

                    for (Candle_T &last_candle : this->last_candles)
                        curr += last_candle.get_close_price() / this->last_candles.size();

                    this->ssma = curr;
                    this->last_ssma = curr;
                } else {
                    this->ssma = 
                        ((this->last_ssma * this->period) - this->last_ssma + candle.get_close_price())
                        / this->period
                    ;
                    this->last_ssma = this->ssma;
                }
                ret["value"] = this->ssma;
            }
    };

    /**
     * @brief Relative Strength Indicator
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class RSI
    {
        // TODO: Remake RSI. Bad for Heikin_Ashi candle
        private:
            /**
             * @brief Description of SMA
             */
            nlohmann::json description;

            /**
             * @brief RSI for last passed Candle
             */
            double rsi = 0.0;

            /**
             * @brief Period of SMA
             */
            int period;

            /**
             * @brief Last Candles
             */
            vector<Candle_T> last_candles;

            /**
             * @brief Return JSON
             */
            nlohmann::json ret;

            /**
             * @brief Average of All up moves 
             */
            double avgGain = 0.0;

            /**
             * @brief Average of All down moves
             */
            double avgLoss = 0.0;

        public:
            /**
             * @brief Construct a new RSI object
             */
            RSI() = default;

            /**
             * @brief Construct a new RSI object
             * 
             * @param indicator_params Parameters for indicator
             */
            RSI(
                nlohmann::json &indicator_params
            ) : description(indicator_params)
            {
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the RSI object
             */
            ~RSI() = default;

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for Indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Get the description 
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->description; }

            /**
             * @brief Get the SMA for last passed Candle
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get() { return ret; }

            /**
             * @brief Resolve the RSI
             * 
             * @param candle Candle
             */
            void resolve(Candle_T& candle)
            {
                double rs;
                
                if (this->last_candles.size() < period)
                {
                    this->last_candles.push_back(candle);

                    for (Candle_T& candle : this->last_candles)
                    {
                        if (candle.is_green()) {
                            this->avgGain += candle.get_change_abs();
                        } else {
                            this->avgLoss += candle.get_change_abs();
                        }
                    }

                    this->avgGain /= this->last_candles.size();
                    this->avgLoss /= this->last_candles.size();

                    ret["value"] = NULL;                    
                } else {
                    if (candle.is_green())
                    {
                        this->avgGain = (this->avgGain * (this->period - 1) + candle.get_change_abs()) / this->period;
                        this->avgLoss = (this->avgLoss * (this->period - 1)) / this->period; 
                    } else {
                        this->avgLoss = (this->avgLoss * (this->period - 1) + candle.get_change_abs()) / this->period;
                        this->avgGain = (this->avgGain * (this->period - 1)) / this->period;
                    }
                }

                rs = this->avgGain / this->avgLoss;

                ret["value"] = 100 - (100 / (1 + rs));
            }
    };
}

/**
 * @brief Community indicators from tradingview.com
 */
namespace Indicators::TradingView
{
    /**
     * @brief Normalized MACD Indicator
     * 
     * @note by "glaz"
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class Normalized_MACD
    {
        // TODO: Problems with Trigger
        private:
            /**
             * @brief Description of Indicator
             */
            nlohmann::json description;

            /**
             * @brief Fast MA
             */
            int sma;

            /**
             * @brief Slow MA
             */
            int lma;

            /**
             * @brief Trigger
             */
            int tsp;

            /**
             * @brief Normalize
             */
            int np;

            /**
             * @brief MacNorm2
             */
            double macnorm2;

            /**
             * @brief Trigger
             */
            double trigger;

            /**
             * @brief Type of Moving Average
             */
            string type;

            /**
             * @brief Interval
             */
            string interval;

            /**
             * @brief Macs array
             */
            vector<double> macs;

            /**
             * @brief MacNorm2s array
             */
            vector<double> macnorm2s;

            /**
             * @brief Exponential Moving Average
             */
            Indicators::Integral::EMA<Candle_T> ema_1;

            /**
             * @brief Exponential Moving Average
             */
            Indicators::Integral::EMA<Candle_T> ema_2;

            /**
             * @brief Weighted Moving Average
             */
            Indicators::Integral::WMA<Candle_T> wma_1;

            /**
             * @brief Weighted Moving Average
             */
            Indicators::Integral::WMA<Candle_T> wma_2;

            /**
             * @brief Simple Moving Average
             * 
             */
            Indicators::Integral::SMA<Candle_T> sma_1;

            /**
             * @brief Simple Moving Average
             * 
             */
            Indicators::Integral::SMA<Candle_T> sma_2;

            /**
             * @brief Get minimal value 
             * 
             * @param num1 First number
             * @param num2 Second number
             * @return double 
             */
            double min(double num1, double num2)
            {
                return num1 > num2 ? num2 : num1;
            }

            /**
             * @brief Get max value
             * 
             * @param num1 First number
             * @param num2 Second number
             * @return double 
             */
            double max(double num1, double num2)
            {
                return num1 > num2 ? num1 : num2;
            }

            /**
             * @brief The lowest value in vector
             * 
             * @param source Source vector
             * @return double 
             */
            double lowest(vector<double> &source)
            {
                double low = source[0];
                for (double& sc : source)
                    if (low > sc)
                        low = sc;
                return low;
            }

            /**
             * @brief The highest value in vector
             * 
             * @param source Source vector
             * @return double 
             */
            double highest(vector<double> &source)
            {
                double high = source[0];
                for (double& sc : source)
                    if (high < sc)
                        high = sc;
                return high;
            }

            /**
             * @brief Calculate Weighted Moving Average
             * 
             * @param source The Source array
             */
            double wma(vector<double> &source)
            {
                double norm = 0.0;
                double sum = 0.0;
                int length = source.size();
                for (int i = 0; i < length - 1; i++)
                {
                    double weight = (length - i) * length;
                    norm += weight;
                    sum += source[i] * weight;
                }
                return sum / norm;
            }

        public:
            /**
             * @brief Construct a new Normalized_MACD object
             */
            Normalized_MACD() = default;

            /**
             * @brief Construct a new Normalized_MACD object
             * 
             * @param indicator_params Parameters for indicator
             */
            Normalized_MACD(
                nlohmann::json &indicator_params
            ) : description(indicator_params)
            {
                this->sma = indicator_params["sma"];
                this->lma = indicator_params["lma"];
                this->tsp = indicator_params["tsp"];
                this->np = indicator_params["np"];
                this->type = (string)indicator_params["type"];
                this->interval = (string)indicator_params["interval"];

                nlohmann::json conf_1 = {
                    {"period", this->sma},
                    {"interval", this->interval}
                };
                nlohmann::json conf_2 = {
                    {"period", this->lma},
                    {"interval", this->interval}
                };

                this->ema_1.set_indicator_params(conf_1);
                this->ema_2.set_indicator_params(conf_2);

                this->wma_1.set_indicator_params(conf_1);
                this->wma_2.set_indicator_params(conf_2);

                this->sma_1.set_indicator_params(conf_1);
                this->sma_2.set_indicator_params(conf_2);
            }

            /**
             * @brief Destroy the Normalized_MACD object
             */
            ~Normalized_MACD() = default;

            /**
             * @brief Get the description 
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->description; }

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;

                this->sma = indicator_params["sma"];
                this->lma = indicator_params["lma"];
                this->tsp = indicator_params["tsp"];
                this->np = indicator_params["np"];
                this->type = (string)indicator_params["type"];
                this->interval = (string)indicator_params["interval"];

                nlohmann::json conf_1 = {
                    {"period", this->sma},
                    {"interval", this->interval}
                };
                nlohmann::json conf_2 = {
                    {"period", this->lma},
                    {"interval", this->interval}
                };

                this->ema_1.set_indicator_params(conf_1);
                this->ema_2.set_indicator_params(conf_2);

                this->wma_1.set_indicator_params(conf_1);
                this->wma_2.set_indicator_params(conf_2);

                this->sma_1.set_indicator_params(conf_1);
                this->sma_2.set_indicator_params(conf_2);
            }

            /**
             * @brief Resolve the Indicator
             */
            void resolve(Candle_T &candle)
            {
                double sh;
                double lon;

                if (this->type == "EMA")
                {
                    this->ema_1.resolve(candle);
                    this->ema_2.resolve(candle);

                    sh = ema_1.get()["value"];
                    lon = ema_2.get()["value"];
                } else if (this->type == "WMA") {
                    this->wma_1.resolve(candle);
                    this->wma_2.resolve(candle);

                    sh = wma_1.get()["value"];
                    lon = wma_2.get()["value"];
                } else if (this->type == "SMA") {
                    this->sma_1.resolve(candle);
                    this->sma_2.resolve(candle);

                    sh = sma_1.get()["value"];
                    lon = sma_2.get()["value"];
                }

                double ratio = this->min(sh, lon) / this->max(sh, lon);
                double mac = (sh > lon ? 2 - ratio : ratio) - 1;

                if (this->macs.size() < this->np)
                    this->macs.push_back(mac);
                else {
                    this->macs.erase(this->macs.begin());
                    this->macs.push_back(mac);
                }

                double macnorm = ((mac - this->lowest(this->macs))
                / (this->highest(this->macs) - this->lowest(this->macs) + 0.000001) * 2) -1
                ;

                this->macnorm2 = this->np < 2 ? mac : macnorm;
                
                if (this->macnorm2s.size() < this->tsp)
                    this->macnorm2s.push_back(this->macnorm2);
                else {
                    this->macnorm2s.erase(this->macnorm2s.begin());
                    this->macnorm2s.push_back(this->macnorm2);
                }

                this->trigger = this->wma(this->macnorm2s);
            }

            /**
             * @brief Get the Normalized MACD
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get()
            {
                nlohmann::json res;
                res["MacNorm"] = this->macnorm2;
                res["Trigger"] = this->trigger;
                return res;
            }
    };
}

















#endif