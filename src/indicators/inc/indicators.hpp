#pragma once

#ifndef INDICATORS_HEADER
#define INDICATORS_HEADER

#include <string>
#include <nlohmann/json.hpp>
#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#include "../../exceptions/inc/exceptions.hpp"
#include "../../request/inc/request.hpp"
#include "../../candles/inc/candles.hpp"

using std::string; 
using std::map; 
using std::cout; 
using std::endl;
using std::vector;

// TODO: Group indicators by "trend", "oscillator", "volatility" and "support/resistance"

/**
 * @brief Abstract classes for Indicators
 */
namespace Indicators
{
    /**
     * @brief Abstract class for Integral Indicators
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class Integral_Indicator
    {
        protected:
            /**
             * @brief Description of indicator
             */
            nlohmann::json description;

            /**
             * @brief Period of Indicator
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
             * @brief Construct a new Integral_Indicator object
             */
            Integral_Indicator() = default;

            /**
             * @brief Destroy the Integral_Indicator object
             */
            virtual ~Integral_Indicator() = default;

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
             * @brief Get the Indicator for last passed Candle
             * 
             * @return nlohmann::json
             */
            nlohmann::json get() { return this->ret; }

            /**
             * @brief Resolve the Indicator for new Candle
             * 
             * @param candle Candle object
             */
            virtual void resolve(Candle_T &candle) = 0;
    };

    /**
     * @brief Abstract class for TradingView Indicators
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class TradingView_Indicator
    {
        protected:
            /**
             * @brief Description of indicator
             */
            nlohmann::json description;

            /**
             * @brief Period of Indicator
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
             * @brief Construct a new Integral_Indicator object
             */
            TradingView_Indicator() = default;

            /**
             * @brief Destroy the Integral_Indicator object
             */
            virtual ~TradingView_Indicator() = default;

            /**
             * @brief Get the description 
             * 
             * @return nlohmann::json 
             */
            nlohmann::json get_description() { return this->description; }

            /**
             * @brief Get the Indicator for last passed Candle
             * 
             * @return nlohmann::json
             */
            nlohmann::json get() { return this->ret; }

            /**
             * @brief Resolve the Indicator for new Candle
             * 
             * @param candle Candle object
             */
            virtual void resolve(Candle_T &candle) = 0;

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for indicator
             */
            virtual void set_indicator_params(nlohmann::json &indicator_params) = 0;
    };
}

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
    /**
     * @brief Exponential Moving Average
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class EMA : public Indicators::Integral_Indicator<Candle_T>
    {
        // TODO: EMA with different sources (close, low, high...)
        private:
            /**
             * @brief Last EMA value
             */
            double last_ema;

            /**
             * @brief Current EMA for Last passed Candle
             */
            double ema = 0.0;

        public:
            /**
             * @brief Construct a new EMA object
             */
            EMA() = default;

            /**
             * @brief Construct a new EMA object
             * 
             * @param indicator_params Params for EMA
             */
            EMA(
                nlohmann::json &indicator_params
            )
            { 
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the EMA object
             */
            virtual ~EMA() = default;

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

                    this->last_candles.push_back(candle);

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
                this->ret["value"] = this->ema;
            }
    };

    /**
     * @brief Weighted Moving Average
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class WMA : public Indicators::Integral_Indicator<Candle_T>
    {
        // TODO: WMA with different source (close, low, high...)
        private:
            /**
             * @brief WMA for last passed Candle
             */
            double wma = 0.0;

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
            )
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the WMA object
             */
            virtual ~WMA() = default;

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
                this->ret["value"] = this->wma;
                this->wma = 0;
            }       
    };

    /**
     * @brief Simple Moving Average
     * 
     * @tparam Candle_T Typeo of Candle
     */
    template <class Candle_T>
    class SMA : public Indicators::Integral_Indicator<Candle_T>
    {
        // TODO: SMA with different source (close, high, low...)
        private:
            /**
             * @brief SMA for last passed Candle
             */
            double sma = 0.0;

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
            )
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the SMA object
             */
            virtual ~SMA() = default;

            /**
             * @brief Resolve the SMA
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
    class SSMA : public Indicators::Integral_Indicator<Candle_T>
    {
        private:
            /**
             * @brief Last RMA value
             */
            double last_ssma;

            /**
             * @brief Current RMA for Last passed Candle
             */
            double ssma = 0.0;

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
            )
            { 
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the SSMA object
             */
            virtual ~SSMA() = default;

            /**
             * @brief Resolve the SSMA Indicator for new Candle
             * 
             * @param candle Candle object
             */
            void resolve(Candle_T &candle)
            {
                if (this->last_candles.size() < this->period)
                {
                    double curr = 0;

                    this->last_candles.push_back(candle);

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
                this->ret["value"] = this->ssma;
            }
    };

    /**
     * @brief Relative Strength Indicator
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class RSI : public Indicators::Integral_Indicator<Candle_T>
    {
        // TODO: Bad for Heikin_Ashi candle
        private:
            /**
             * @brief RSI for last passed Candle
             */
            double rsi = 0.0;

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
            )
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the RSI object
             */
            virtual ~RSI() = default;

            /**
             * @brief Resolve the RSI
             * 
             * @param candle Candle
             */
            void resolve(Candle_T& candle)
            {
                double rs;
                
                if (this->last_candles.size() < this->period)
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

                    this->ret["value"] = NULL;                    
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

                this->ret["value"] = 100 - (100 / (1 + rs));
            }
    };

    /**
     * @brief True Range indicator
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class TR : public Indicators::Integral_Indicator<Candle_T>
    {
        private:
            /**
             * @brief True Range
             */
            double tr = 0.0;

            /**
             * @brief Find Maximum 3 number
             * 
             * @param param1 Number 1
             * @param param2 Number 2
             * @param param3 Number 3
             * @return double 
             */
            double max(double param1, double param2, double param3)
            {
                return (param1 > param2) ? 
                       ((param1 > param3) ? param1 : param3) :
                       ((param2 > param3) ? param2 : param3);

            }

        public:
            /**
             * @brief Construct a new TR object
             */
            TR() = default;

            /**
             * @brief Construct a new TR object
             * 
             * @param indicator_params Parameters for Indicator
             */
            TR(
                nlohmann::json &indicator_params
            )
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the TR object
             */
            virtual ~TR() = default;

            /**
             * @brief Resolve True Range indicator
             * 
             * @param candle Candle
             */
            void resolve(Candle_T &candle)
            {
                this->tr = this->max(
                    candle.get_high_price() - candle.get_low_price(),
                    abs(candle.get_high_price() - candle.get_close_price()),
                    abs(candle.get_low_price() - candle.get_close_price())
                );
                this->ret["value"] = this->tr;
            }
    };

    /**
     * @brief Average True Range
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class ATR : public Indicators::Integral_Indicator<Candle_T>
    {
        private:
            /**
             * @brief Average True Range
             */
            double atr = 0.0;

            /**
             * @brief Last RMA
             */
            double last_rma;

            /**
             * @brief Last EMA
             */
            double last_ema;

            /**
             * @brief Last RMA values
             */
            vector<double> last_rmas;

            /**
             * @brief Last EMAs values
             */
            vector<double> last_emas;

            /**
             * @brief Last SMAs values
             */
            vector<double> last_smas;

            /**
             * @brief Last WMAs values
             */
            vector<double> last_wmas;

            /**
             * @brief Smoothing type for ATR
             */
            string smoothed;

            /**
             * @brief True Range indicator
             */
            Indicators::Integral::TR<Candle_T> tr_ind;

            /**
             * @brief Calculate RMA
             * 
             * @param param Parameter
             * @return double 
             */
            void rma(double param)
            {
                if (this->last_rmas.size() < this->period)
                {
                    double curr = 0;

                    this->last_rmas.push_back(param);

                    for (double &par : this->last_rmas)
                        curr += par / this->last_rmas.size();

                    this->atr = curr;
                    this->last_rma = curr;
                } else {
                    this->atr = 
                        ((this->last_rma * this->period) - this->last_rma + param)
                        / this->period
                    ;
                    this->last_rma = this->atr;
                }
            }

            /**
             * @brief Calculate EMA
             * 
             * @param param Parameter
             * @return double 
             */
            void ema(double param)
            {
                this->last_emas.push_back(param);

                if (this->last_emas.size() < this->period)
                {
                    double curr = 0;

                    for (double &last_val : this->last_emas)
                        curr += last_val / this->last_emas.size();

                    this->atr = curr;
                    this->last_ema = curr;
                } else {
                    this->atr = 
                        (param - this->last_ema) * 2.0
                        / (this->period + 1.0) + this->last_ema
                    ;
                    this->last_ema = this->atr;
                }
            }

            /**
             * @brief Calculate SMA
             * 
             * @param param Parameter
             * @return double 
             */
            void sma(double param)
            {
                double calc = 0.0;

                this->last_smas.push_back(param);
                if (this->last_smas.size() > this->period)
                    this->last_smas.erase(this->last_smas.begin());

                for (double& sma : this->last_smas)
                    calc += sma / this->last_smas.size();
                
                this->atr = calc;
            }

            /**
             * @brief Calculate WMA
             * 
             * @param param 
             * @return double 
             */
            void wma(double param)
            {
                double calc = 0.0;
                int length = this->last_wmas.size();

                this->last_wmas.push_back(param);
                if (this->last_wmas.size() > this->period)
                    this->last_wmas.erase(this->last_wmas.begin());

                for (int i = length - 1; i >= 0; i--) 
                    calc += this->last_wmas[i] * (i + 1);
                this->atr = calc / (length * (length + 1) / 2);
            }

        public:
            /**
             * @brief Construct a new ATR object
             */
            ATR() = default;

            /**
             * @brief Construct a new ATR object
             * 
             * @param indicator_params Parameters for indicator
             */
            ATR(
                nlohmann::json &indicator_params
            )
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
                this->smoothed = indicator_params["smoothed"];
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
                this->smoothed = indicator_params["smoothed"];
            }

            /**
             * @brief Resolve Average True Range indicator
             * 
             * @param candle Candle
             */
            void resolve(Candle_T &candle)
            {
                // TODO: Remake with function pointer
                this->tr_ind.set_indicator_params(this->description);
                double curr_tr;

                this->last_candles.push_back(candle);
                if (this->last_candles.size() > this->period)
                    this->last_candles.erase(
                        this->last_candles.begin()
                    );

                if (this->smoothed == "SMA")
                    for (Candle_T& can : this->last_candles)
                    {
                        this->tr_ind.resolve(can);
                        curr_tr = tr_ind.get()["value"];
                        this->sma(curr_tr);
                    }
                if (this->smoothed == "WMA")
                    for (Candle_T& can : this->last_candles)
                    {
                        this->tr_ind.resolve(can);
                        curr_tr = tr_ind.get()["value"];
                        this->wma(curr_tr);
                    }
                if (this->smoothed == "EMA")
                {
                    this->tr_ind.resolve(candle);
                    curr_tr = tr_ind.get()["value"];
                    this->ema(curr_tr);
                }

                if (this->smoothed == "RMA")
                {
                    this->tr_ind.resolve(candle);
                    curr_tr = tr_ind.get()["value"];
                    this->rma(curr_tr);
                }


                this->ret["value"] = this->atr;
                this->atr = 0.0;
            }
    };

    /**
     * @brief Directional Movement Index
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class DMI : public Indicators::Integral_Indicator<Candle_T>
    {
        private:
            /**
             * @brief Type of smoothing for DMI
             */
            string smoothed;

            /**
             * @brief Last value of +DM for smoothing
             */
            double last_dm_plus = 0;

            /**
             * @brief Last value of -DM for smoothing
             */
            double last_dm_minus = 0;

            /**
             * @brief Array of UpMoves
             */
            vector<double> upmoves;

            /**
             * @brief Array of DownMoves
             */
            vector<double> downmoves;

            /**
             * @brief Array of ATRs
             */
            vector<double> atrs;

            /**
             * @brief Average True Range indicator
             */
            Indicators::Integral::ATR<Candle_T> atr;

            /**
             * @brief Calculate EMA
             * 
             * @param last_emas Last EMAs values
             * @param last_ema Last EMA value
             * @param param Parameter
             * @return double 
             */
            double ema(vector<double> &last_emas, double &last_ema, double param)
            {
                double curr = 0;

                if (last_emas.size() < this->period)
                {
                    for (double &last_val : last_emas)
                        curr += last_val / last_emas.size();

                    last_ema = curr;
                    
                    return curr;
                } else {
                    curr = 
                        (param - last_ema) * 2.0
                        / (this->period + 1.0) + last_ema
                    ;
                    last_ema = curr;

                    return curr;
                }
            }

            /**
             * @brief Calculate Smoothed
             * 
             * @param values Array of values
             * @param param Parameter
             * @return double 
             */
            double smooth(vector<double> &values, double param)
            {
                double smoothed = 0;
                for (double& val : values)
                    smoothed += val;
                return (smoothed - smoothed / values.size() + param);
            }
        
            /**
             * @brief Calculate RMA
             * 
             * @param param Parameter
             * @return double 
             */
            double rma(vector<double> &last_rmas, double &last_rma, double param)
            {
                double curr = 0;
                if (last_rmas.size() < this->period)
                {
                    last_rmas.push_back(param);

                    for (double &par : last_rmas)
                        curr += par / last_rmas.size();

                    last_rma = curr;
                    return curr;
                } else {
                    curr = 
                        ((last_rma * this->period) - last_rma + param)
                        / this->period
                    ;
                    last_rma = curr;
                    return curr;
                }
            } 

        public:
            /**
             * @brief Construct a new DMI object
             */
            DMI() = default;

            /**
             * @brief Construct a new DMI object
             * 
             * @param indicator_params 
             */
            DMI(
                nlohmann::json &indicator_params
            )
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
                this->smoothed = indicator_params["smoothed"];

                nlohmann::json atr_params = {
                    {"period", indicator_params["atr_period"]},
                    {"smoothed", indicator_params["atr_smoothed"]}
                };
                this->atr.set_indicator_params(atr_params);
            }

            /**
             * @brief Destroy the DM object
             */
            ~DMI() = default;

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];
                this->smoothed = indicator_params["smoothed"];

                nlohmann::json atr_params = {
                    {"period", indicator_params["period"]},
                    {"smoothed", indicator_params["atr_smoothed"]}
                };
                this->atr.set_indicator_params(atr_params);
            }

            /**
             * @brief Resolve DMI Indicator
             * 
             * @param candle Candle
             */
            void resolve(Candle_T &candle)
            {
                double upmove;
                double downmove;
                double atr_value;

                if (this->last_candles.size() == 0)
                {
                    this->last_candles.push_back(candle);
                    this->ret["+DI"] = NULL;
                    this->ret["-DI"] = NULL;
                    return;
                }

                this->atr.resolve(candle);
                atr_value = atr.get()["value"];

                upmove = candle.get_high_price() - this->last_candles.back().get_high_price();
                downmove = this->last_candles.back().get_low_price() - candle.get_low_price();

                if (upmove < 0 && downmove < 0)
                {
                    upmove = 0;
                    downmove = 0;
                } else if (upmove > downmove) {
                    upmove = upmove;
                    downmove = 0;
                } else if (downmove > upmove) {
                    downmove = downmove;
                    upmove = 0;
                }

                if (this->upmoves.size() < this->period)
                {
                    this->upmoves.push_back(upmove);
                    this->downmoves.push_back(downmove);
                    this->atrs.push_back(atr_value);
                } else {
                    this->upmoves.push_back(upmove);
                    this->downmoves.push_back(downmove);
                    this->atrs.push_back(atr_value);
                    
                    this->upmoves.erase(this->upmoves.begin());
                    this->downmoves.erase(this->downmoves.begin());
                    this->atrs.erase(this->atrs.begin());
                }

                // TODO: Remake with function pointer
                if (this->smoothed == "RMA")
                {
                    this->ret["+DI"] = this->rma(this->upmoves, this->last_dm_plus, upmove) / 
                        atr_value * 100;
                    this->ret["-DI"] = this->rma(this->downmoves, this->last_dm_minus, downmove) / 
                        atr_value * 100;
                } else if (this->smoothed == "EMA") {
                    this->ret["+DI"] = this->ema(this->upmoves, this->last_dm_plus, upmove) / 
                        atr_value * 100;
                    this->ret["-DI"] = this->ema(this->downmoves, this->last_dm_minus, downmove) / 
                        atr_value * 100;
                } else if (this->smoothed == "Smooth") {
                    this->ret["+DI"] = this->smooth(this->upmoves, upmove) / 
                        atr_value * 100;
                    this->ret["-DI"] = this->smooth(this->downmoves, downmove) / 
                        atr_value * 100;
                }

                this->last_candles.push_back(candle);

                this->last_candles.erase(
                    this->last_candles.begin()
                );
            }
    };

    /**
     * @brief Average Directional Index
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class ADX : public Indicators::Integral_Indicator<Candle_T>
    {
        private:
            /**
             * @brief Current ADX value
             */
            double adx = 0.0;

            /**
             * @brief DMI Indicator
             */
            Indicators::Integral::DMI<Candle_T> dmi;

            /**
             * @brief Last RMAs values
             */
            vector<double> last_rmas;

            /**
             * @brief Last RMA value
             */
            double last_rma = 0.0;

            /**
             * @brief Calculate RMA
             * 
             * @param param Parameter
             * @return double 
             */
            double rma(vector<double> &last_rmas, double &last_rma, double param)
            {
                double curr = 0;
                if (last_rmas.size() < this->period)
                {
                    last_rmas.push_back(param);

                    for (double &par : last_rmas)
                        curr += par / last_rmas.size();

                    last_rma = curr;
                    return curr;
                } else {
                    curr = 
                        ((last_rma * this->period) - last_rma + param)
                        / this->period
                    ;
                    last_rma = curr;
                    return curr;
                }
            }

        public:
            /**
             * @brief Construct a new ADX object
             */
            ADX() = default; 

            /**
             * @brief Construct a new ADX object
             * 
             * @param indicator_params Parameters for Indicator
             */
            ADX(
                nlohmann::json &indicator_params
            )
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];

                nlohmann::json dmi_params = {
                    {"period", indicator_params["dmi_period"]},
                    {"smoothed", indicator_params["dmi_smoothed"]},
                    {"atr_smoothed", indicator_params["dmi_atr_smoothed"]}
                };
                this->dmi.set_indicator_params(dmi_params);
            }
            
            /**
             * @brief Destroy the ADX object
             */
            virtual ~ADX() = default;

            /**
             * @brief Set the indicator params 
             * 
             * @param indicator_params Parameters for indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;
                this->period = indicator_params["period"];

                nlohmann::json dmi_params = {
                    {"period", indicator_params["dmi_period"]},
                    {"smoothed", indicator_params["dmi_smoothed"]},
                    {"atr_smoothed", indicator_params["dmi_atr_smoothed"]}
                };
                this->dmi.set_indicator_params(dmi_params);
            }

            /**
             * @brief Resolve ADX Indicator
             * 
             * @param candle Candle object
             */
            void resolve(Candle_T &candle)
            {
                this->dmi.resolve(candle);
                nlohmann::json dmi_ret = dmi.get();
                double di_plus = dmi_ret["+DI"];
                double di_minus = dmi_ret["-DI"];
                if (di_plus == NULL || di_minus == NULL)
                {
                    this->ret["value"] = NULL;
                    return;
                }

                double sum = di_plus + di_minus;
                sum = sum == 0.0 ? 1 : sum;

                double param = abs(di_plus - di_minus) / sum;
                this->last_rmas.push_back(param);
                if (this->last_rmas.size() > this->period)
                    this->last_rmas.erase(
                        this->last_rmas.begin()
                    );

                this->adx = 100 * this->rma(this->last_rmas, this->last_rma, param);

                this->ret["value"] = this->adx;
            }
    };

    /**
     * @brief Hull Moving Average
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class HMA : public Indicators::Integral_Indicator<Candle_T>
    {
        private:
            /**
             * @brief Last candles for WMA 1
             */
            vector<Candle_T> wma_1_candles;

            /**
             * @brief Last candles for WMA 2
             */
            vector<Candle_T> wma_2_candles;

            /**
             * @brief Values for Hull calculation
             */
            vector<double> values;

            /**
             * @brief Calculate WMA for candles array
             * 
             * @param candles Vector of candles
             */
            double calc_wma(vector<Candle_T> &candles)
            {
                double wma = 0;
                int length = candles.size();

                for (int i = 0; i < length; i++)
                    wma += candles[i].get_close_price() * (i + 1);
                wma /= length * (length + 1) / 2;

                return wma;
            }

            /**
             * @brief Calculate WMA for values array
             * 
             * @param values Vector of values
             */
            double calc_wma(vector<double> &values)
            {
                double wma = 0;
                int length = values.size();

                for (int i = 0; i < length; i++)
                    wma += values[i] * (i + 1);
                wma /= length * (length + 1) / 2;

                return wma;
            }

        public:
            /**
             * @brief Construct a new Hull object
             */
            HMA() = default;

            /**
             * @brief Construct a new Hull object
             * 
             * @param indicator_params Params for Hull
             */
            HMA(
                nlohmann::json &indicator_params
            ) {
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the Hull object
             */
            virtual ~HMA() = default;

            /**
             * @brief Resolve Hull indicator for Candle
             * 
             * @param candle Candle object
             */
            void resolve(Candle_T &candle)
            {
                int wma_1_period = this->period / 2;
                int values_period = round(sqrt(this->period));

                if (this->wma_1_candles.size() < wma_1_period) {
                    this->wma_1_candles.push_back(candle);
                } else {
                    this->wma_1_candles.push_back(candle);
                    this->wma_1_candles.erase(this->wma_1_candles.begin());
                }

                if (this->wma_2_candles.size() < this->period) {
                    this->wma_2_candles.push_back(candle);
                } else {
                    this->wma_2_candles.push_back(candle);
                    this->wma_2_candles.erase(this->wma_2_candles.begin());
                }

                double wma_1 = this->calc_wma(this->wma_1_candles);
                double wma_2 = this->calc_wma(this->wma_2_candles);

                double value = 2 * wma_1 - wma_2;

                if (this->values.size() < values_period) {
                    this->values.push_back(value);
                } else {
                    this->values.push_back(value);
                    this->values.erase(this->values.begin());
                }

                this->ret["value"] = this->calc_wma(this->values);
            }
    };

    /**
     * @brief Commodity Channel Index
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class CCI : public Indicators::Integral_Indicator<Candle_T>
    {
        private:
            /**
             * @brief Calculate Simple Moving Average
             * 
             * @param candles Array of Candles
             * @return double 
             */
            double calc_sma(vector<Candle_T> &candles)
            {
                double sum = 0;
                double size = candles.size();

                for (auto& candle : candles)
                    sum += candle.get_typical_price() / size;

                return sum;
            }
            
            /**
             * @brief Calculate Mean Deviation
             * 
             * @param candles Array of Candles
             * @param ma Moving Average value
             * @return double 
             */
            double calc_md(vector<Candle_T> &candles, double ma)
            {
                double sum = 0;
                double size = candles.size();

                for (auto& candle : candles)
                    sum += (abs(candle.get_typical_price() - ma)) / size;

                return sum;
            }

        public:
            /**
             * @brief Construct a new CCI object
             */
            CCI() = default;

            /**
             * @brief Construct a new CCI object
             * 
             * @param indicator_params Parameters for indicator
             */
            CCI(
                nlohmann::json &indicator_params
            ) {
                this->description = indicator_params;
                this->period = indicator_params["period"];
            }

            /**
             * @brief Destroy the CCI object
             */
            virtual ~CCI() = default;

            /**
             * @brief Resolve the CCI indicator
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

                double ma = this->calc_sma(this->last_candles);
                double cci = (candle.get_typical_price() - ma) /
                    (0.015 * this->calc_md(this->last_candles, ma));
                
                this->ret["value"] = cci;
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
    class Normalized_MACD : public Indicators::TradingView_Indicator<Candle_T>
    {
        // TODO: Bad for Heikin_Ashi candle
        // TODO: Rewrite with pointer function
        private:
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
                int length = source.size();
                double norm = 0.0;
                for (int i = length - 1; i >= 0; i--)
                    norm += source[i] * (i + 1);
                return norm / (length * (length + 1) / 2);
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
            )
            {
                this->description = indicator_params;

                this->sma = indicator_params["sma"];
                this->lma = indicator_params["lma"];
                this->tsp = indicator_params["tsp"];
                this->np = indicator_params["np"];
                this->type = (string)indicator_params["type"];

                nlohmann::json conf_1 = {
                    {"period", this->sma},
                    {"interval", indicator_params["interval"]}
                };
                nlohmann::json conf_2 = {
                    {"period", this->lma},
                    {"interval", indicator_params["interval"]}
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
            virtual ~Normalized_MACD() = default;

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

                nlohmann::json conf_1 = {
                    {"period", this->sma},
                    {"interval", indicator_params["interval"]}
                };
                nlohmann::json conf_2 = {
                    {"period", this->lma},
                    {"interval", indicator_params["interval"]}
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

                this->ret["Trigger"] = this->trigger;
                this->ret["MacNorm"] = this->macnorm2;
            }
    };

    /**
     * @brief JMA RSX Clone
     * 
     * @note by "LazyBear"
     * 
     * @tparam Candle_T Type of Candle
     */
    template <class Candle_T>
    class RSXC_LB : public Indicators::TradingView_Indicator<Candle_T>
    {
        private:
            /**
             * @brief First candle status (for initializing paramters)
             */
            bool first = true;

            /**
             * @brief Length (Period)
             */
            int length;

            /**
             * @brief Source value of candle (close, high...)
             */
            string src;

            double last_f90_ = 0.0;

            double last_f88 = 0.0;

            double last_f8 = 0.0;;

            double last_f28 = 0.0;

            double last_f30 = 0.0;

            double last_f38 = 0.0;

            double last_f40 = 0.0;

            double last_f48 = 0.0;

            double last_f50 = 0.0;

            double last_f58 = 0.0;

            double last_f60 = 0.0;

            double last_f68 = 0.0;

            double last_f70 = 0.0;

            double last_f78 = 0.0;

            double last_f80 = 0.0;

        public:
            /**
             * @brief Construct a new rsxl lb object
             */
            RSXC_LB() = default;

            /**
             * @brief Construct a new rsxl lb object
             * 
             * @param indicator_params Paramters of indicator
             */
            RSXC_LB(
                nlohmann::json &indicator_params
            )
            {
                this->description = indicator_params;

                this->length = indicator_params["length"];
                this->src = indicator_params["src"];
            }

            /**
             * @brief Destroy the rsxl lb object
             */
            virtual ~RSXC_LB() = default;

            /**
             * @brief Set the indicator params object
             * 
             * @param indicator_params Parameters of indicator
             */
            void set_indicator_params(nlohmann::json &indicator_params)
            {
                this->description = indicator_params;

                this->length = indicator_params["length"];
                this->src = indicator_params["src"];
            }

            /**
             * @brief Resolve RSXL_LB indicator
             * 
             * @param candle Candle
             */
            void resolve(Candle_T &candle)
            {
                double f8 = 0, f18 = 0, f20 = 0, f10 = 0, 
                       f90_ = 0, f88 = 0, v8 = 0, f28 = 0, 
                       f30 = 0, vC = 0, f38 = 0, f40 = 0, 
                       v10 = 0, f48 = 0, f50 = 0, v14 = 0, 
                       f58 = 0, f60 = 0, v18 = 0, f68 = 0, 
                       f70 = 0, v1C = 0, f78 = 0, f80 = 0, 
                       v20 = 0, f0 = 0, f90 = 0, v4_ = 0, 
                       rsx = 0
                ;

                if (first) {
                    this->last_f90_ = this->last_f90_ == 0.0 ? 
                        1.0 : this->last_f88 <= this->last_f90_ ? 
                        this->last_f88 + 1 : this->last_f90_ + 1;

                    this->last_f88 = (this->last_f90_ == 0.0) && 
                        (this->length - 1 >= 5) ?
                        this->length - 1.0 : 5.0;
                    
                    if (this->src == "Close")
                        this->last_f8 = 100 * candle.get_close_price();
                    if (this->src == "Open")
                        this->last_f8 = 100 * candle.get_open_price();
                    if (this->src == "High")
                        this->last_f8 = 100 * candle.get_high_price();
                    if (this->src == "Low")
                        this->last_f8 = 100 * candle.get_low_price();

                    f18 = 3.0 / (this->length + 2.0);
                    f20 = 1.0 - f18;
                    f10 = this->last_f8;
                    v8 = this->last_f8 - f10;                   

                    this->last_f28 = f20 * this->last_f28 + f18 * v8;
                    this->last_f30 = f18 * this->last_f28 + f20 * this->last_f30;

                    vC = f28 * 1.5 - f30 * 0.5;

                    this->last_f38 = f20 * this->last_f38 + f18 * vC;
                    this->last_f40 = f18 * this->last_f38 + f20 * this->last_f40;

                    v10 = this->last_f38 * 1.5 - this->last_f40 * 0.5;

                    this->last_f48 = f20 * this->last_f48 + f18 * v10;
                    this->last_f50 = f18 * this->last_f48 +f18 * v10;
                    v14 = f48 * 1.5 - f50 * 0.5;

                    this->last_f58 = f20 * this->last_f58 + f18 * abs(v8);
                    this->last_f60 = f18 * this->last_f58 + f20 * this->last_f60;
                    v18 = this->last_f58 * 1.5 - f60 * 0.5;

                    this->last_f68 = f20 * this->last_f68 + f18 * v18;
                    this->last_f70 = f18 * this->last_f68 + f20 * this->last_f70;
                    v1C = this->last_f68 * 1.5 - this->last_f70 * 0.5;

                    this->last_f78 = f20 * this->last_f78 + f18 * v1C;
                    this->last_f80 = f18 * this->last_f78 + f20 * this->last_f80;

                    this->ret["value"] = NULL;
                    this->first = false;
                    return;
                }
                f90_ = this->last_f90_ == 0.0 ? 
                    1.0 : this->last_f88 <= this->last_f90_ ? 
                    this->last_f88 + 1 : this->last_f90_ + 1;

                f88 = (this->last_f90_ == 0.0) && 
                    (this->length - 1 >= 5) ?
                    this->length - 1.0 : 5.0;

                if (this->src == "Close")
                    f8 = 100 * candle.get_close_price();
                if (this->src == "Open")
                    f8 = 100 * candle.get_open_price();
                if (this->src == "High")
                    f8 = 100 * candle.get_high_price();
                if (this->src == "Low")
                    f8 = 100 * candle.get_low_price();

                f18 = 3.0 / (this->length + 2.0);
                f20 = 1.0 - f18;
                f10 = this->last_f8;
                v8 = f8 - f10;

                f28 = f20 * this->last_f28 + f18 * v8;
                f30 = f18 * f28 + f20 * this->last_f30;
                vC = f28 * 1.5 - f30 * 0.5;

                f38 = f20 * this->last_f38 + f18 * vC;
                f40 = f18 * f38 + f20 * this->last_f40;
                v10 = f38 * 1.5 - f40 * 0.5;

                f48 = f20 * this->last_f48 + f18 * v10;
                f50 = f18 * f48 + f20 * this->last_f50;
                v14 = f48 * 1.5 - f50 * 0.5;

                f58 = f20 * this->last_f58 + f18 * abs(v8);
                f60 = f18 * f58 + f20 * this->last_f60;
                v18 = f58 * 1.5 - f60 * 0.5;

                f68 = f20 * this->last_f68 + f18 * v18;
                f70 = f18 * f68 + f20 * this->last_f70;
                v1C = f68 * 1.5 - f70 * 0.5;

                f78 = f20 * this->last_f78 + f18 * v1C;
                f80 = f18 * f78 + f20 * this->last_f80;
                v20 = f78 * 1.5 - f80 * 0.5;

                f0 = ((f88 >= f90) && (f8 != f10)) ? 1.0 : 0.0;
                f90 = ((f88 == f90) && (f0 == 0.0)) ? 0.0 : f90_;

                v4_ = ((f88 < f90) && (v20 > 0.0000000001)) ? 
                    (v14 / v20 + 1.0) * 50.0 : 50.0;
                
                rsx = (v4_ > 100.0) ? 100.0 : (v4_ < 0.0) ? 0.0 : v4_;

                this->ret["value"] = rsx;

                this->last_f8 = f8;
                this->last_f90_ = f90_;
                this->last_f88 = f88;
                this->last_f28 = f28;
                this->last_f30 = f30;
                this->last_f38 = f38;
                this->last_f40 = f40;
                this->last_f48 = f48;
                this->last_f50 = f50;
                this->last_f58 = f58;
                this->last_f60 = f60;
                this->last_f68 = f68;
                this->last_f70 = f70;
                this->last_f78 = f78;
                this->last_f80 = f80;
            }
    };
}

















#endif