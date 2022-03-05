#pragma once

#ifndef STRATEGIES_HEADER
#define STRATEGIES_HEADER

#include <string>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

#include "../../indicators/inc/indicators.hpp"
#include "../../exceptions/inc/exceptions.hpp"

using std::cout, std::string, std::endl, std::map;

/**
 * @brief Custom Strategies for Bot
 */
namespace Strategies::Customs
{
    /**
     * @brief EMA Cross Strategy
     */
    class EMA_Cross
    {
        private:
            /**
             * @brief Is Short EMA above Long EMA?
             */
            bool is_short_above = false;

            /**
             * @brief Is Short EMA below Long EMA?
             */
            bool is_short_below = false;

            /**
             * @brief Does Short EMA crossed Long EMA above? (Buy signal)
             * 
             * @param short_ema Short EMA
             * @param long_ema Long EMA
             * @return true 
             * @return false 
             */
            bool cross_above(double short_ema, double long_ema)
            {
                if (!this->is_short_above && !this->is_short_below)
                {
                    if (short_ema > long_ema)
                    {
                        this->is_short_above = true;
                        this->is_short_below = false;
                    } else {
                        this->is_short_above = false;
                        this->is_short_below = true;
                    }
                    return false;
                }

                if (this->is_short_above && this->is_short_below)
                    throw Exceptions::Strategies::Logic_Exception(
                        "Logical error in Strategy. Something can't be",
                        1,
                        0
                    );

                if (this->is_short_below && short_ema > long_ema)
                {
                    this->is_short_above = true;
                    this->is_short_below = false;

                    return true;
                }

                return false;
            }

            /**
             * @brief Does Short EMA crossed Long EMA below? (Sell signal)
             * 
             * @param short_ema Short EMA
             * @param long_ema Long EMA
             * @return true 
             * @return false 
             */
            bool cross_below(double short_ema, double long_ema)
            {
                if (!this->is_short_above && !this->is_short_below)
                {
                    if (short_ema > long_ema)
                    {
                        this->is_short_above = true;
                        this->is_short_below = false;
                    } else {
                        this->is_short_above = false;
                        this->is_short_below = true;
                    }
                    return false;
                }

                if (this->is_short_above && this->is_short_below)
                    throw Exceptions::Strategies::Logic_Exception(
                        "Logical error in Strategy. Something can't be",
                        1,
                        0
                    );

                if (this->is_short_above && short_ema < long_ema)
                {
                    this->is_short_above = false;
                    this->is_short_below = true;

                    return true;
                }

                return false;
            }

        public:
            /**
             * @brief Construct a new ema cross object
             */
            EMA_Cross() = default;

            /**
             * @brief Destroy the ema cross object
             */
            ~EMA_Cross() = default;

            /**
             * @brief Resolve EMA Cross Strategy
             * 
             * @param params Short and Long EMAs values
             * @param signals Signals of resolving (sell or buy)
             */
            void resolve(nlohmann::json &params, std::map<std::string, bool> &signals)
            {
                signals["long_open"] = false;
                signals["long_close"] = false;
                signals["short_open"] = false;
                signals["short_close"] = false;

                if (this->cross_above(params["short_ema"]["value"], params["long_ema"]["value"])) 
                {
                    signals["long_open"] = true;
                    signals["short_close"] = true;
                }
                if (this->cross_below(params["short_ema"]["value"], params["long_ema"]["value"]))
                {
                    signals["long_close"] = true;
                    signals["short_open"] = true;
                }
            }
    };

    /**
     * @brief Normalized MACD Cross Strategy
     */
    class Normalized_MACD_Cross
    {
        private:
            /**
             * @brief Is Short EMA above Long EMA?
             */
            bool is_trigger_above = false;

            /**
             * @brief Is Short EMA below Long EMA?
             */
            bool is_trigger_below = false;

            /**
             * @brief Does Trigger crossed MacNorm above? (Buy signal)
             * 
             * @param trigger Trigger
             * @param macnorm MacNorm
             * @return true 
             * @return false 
             */
            bool cross_above(double trigger, double macnorm)
            {
                if (!this->is_trigger_above && !this->is_trigger_below)
                {
                    if (trigger > macnorm)
                    {
                        this->is_trigger_above = true;
                        this->is_trigger_below = false;
                    } else {
                        this->is_trigger_above = false;
                        this->is_trigger_below = true;
                    }
                    return false;
                }

                if (this->is_trigger_above && this->is_trigger_below)
                    throw Exceptions::Strategies::Logic_Exception(
                        "Logical error in Strategy. Something can't be",
                        1,
                        0
                    );

                if (this->is_trigger_below && trigger > macnorm)
                {
                    this->is_trigger_above = true;
                    this->is_trigger_below = false;

                    return true;
                }

                return false;
            }

            /**
             * @brief Does Trigger crossed MacNorm below? (Sell signal)
             * 
             * @param trigger Trigger
             * @param macnorm MacNorm
             * @return true 
             * @return false 
             */
            bool cross_below(double trigger, double macnorm)
            {
                if (!this->is_trigger_above && !this->is_trigger_below)
                {
                    if (trigger > macnorm)
                    {
                        this->is_trigger_above = true;
                        this->is_trigger_below = false;
                    } else {
                        this->is_trigger_above = false;
                        this->is_trigger_below = true;
                    }
                    return false;
                }

                if (this->is_trigger_above && this->is_trigger_below)
                    throw Exceptions::Strategies::Logic_Exception(
                        "Logical error in Strategy. Something can't be",
                        1,
                        0
                    );

                if (this->is_trigger_above && trigger < macnorm)
                {
                    this->is_trigger_above = false;
                    this->is_trigger_below = true;

                    return true;
                }

                return false;
            }

        public:
            /**
             * @brief Construct a new Normalized_MACD_Cross object
             */
            Normalized_MACD_Cross() = default;

            /**
             * @brief Destroy the Normalized_MACD_Cross object
             */
            ~Normalized_MACD_Cross() = default;

            /**
             * @brief Resolve Normalized MACD Cross Strategy
             * 
             * @param params Trigger and MacNorm values
             * @param signals Signals of resolving (sell or buy)
             */
            void resolve(nlohmann::json &params, std::map<std::string, bool> &signals)
            {
                signals["long_open"] = false;
                signals["long_close"] = false;
                signals["short_open"] = false;
                signals["short_close"] = false;

                if (this->cross_above((double)params["normalized_macd"]["Trigger"], (double)params["normalized_macd"]["MacNorm"])) 
                {
                    signals["long_open"] = true;
                    signals["short_close"] = true;
                }
                if (this->cross_below(params["normalized_macd"]["Trigger"], params["normalized_macd"]["MacNorm"]))
                {
                    signals["long_close"] = true;
                    signals["short_open"] = true;
                }
            }
    };

    /**
     * @brief RSXC + ADX strategy
     */
    class RSXC_ADX_Strategy
    {
        private:
            /**
             * @brief ADX line
             */
            double adx_line = 22.0;

            /**
             * @brief RSXC_LB low line
             */
            double rsxc_lb_low_line = 30.0;

            /**
             * @brief RSXC_LB middle line
             */
            double rsxc_lb_middle_line = 50.0;

            /**
             * @brief RSXC_LB high line
             */
            double rsxc_lb_high_line = 70.0;

        public:
            /**
             * @brief Construct a new rsx adx object
             */
            RSXC_ADX_Strategy() = default;

            /**
             * @brief Destroy the rsx adx object
             * 
             */
            ~RSXC_ADX_Strategy() = default;

             /**
             * @brief Resolve Strategy
             * 
             * @param params Parameters for strategy (indicators value)
             * @param signals Signals of resolving (sell or buy)
             */
            void resolve(nlohmann::json &params, std::map<std::string, bool> &signals)
            {
                signals["long_open"] = false;
                signals["long_close"] = false;
                signals["short_open"] = false;
                signals["short_close"] = false;

                if (
                    params["rsxc_lb"]["value"] <= this->rsxc_lb_low_line &&
                    params["adx"]["value"] >= this->adx_line 
                )
                    signals["long_open"] = true;
                if (
                    params["rsxc_lb"]["value"] >= this->rsxc_lb_high_line
                )
                    signals["long_close"] = true;
                if (
                    params["rsxc_lb"]["value"] >= this->rsxc_lb_high_line &&
                    params["adx"]["value"] >= this->adx_line 
                )
                    signals["short_open"] = true;
                if (
                    params["rsxc_lb"]["value"] <= this->rsxc_lb_low_line
                )
                    signals["short_close"] = true;
            }
    };

    /**
     * @brief HMA+CCI strategy v0.1
     */
    class HMA_CCI_Strategy
    {
        private:
            /**
             * @brief Is above status
             */
            bool is_above = false;

            /**
             * @brief Is below status
             */
            bool is_below = false;

            /**
             * @brief Last value of HMA indicator
             */
            double last_hma = 0.0;

            /**
             * @brief CCI Lower line
             */
            double cci_lower_line = -50.0;

            /**
             * @brief CCI Upper line
             */
            double cci_upper_line = 50.0;

            /**
             * @brief CCI Lower exit
             */
            double cci_lower_exit = -100;

            /**
             * @brief CCI Upper exit
             */
            double cci_upper_exit = 100;

            /**
             * @brief Does crossing value crossed what value above?
             * 
             * @param crossing_value Crossing value
             * @param what_value What crossing_value is crossing
             * @return true 
             * @return false 
             */
            bool cross_above(double crossing_value, double what_value)
            {
                if (!this->is_above && !this->is_below)
                {
                    if (crossing_value > what_value)
                    {
                        this->is_above = true;
                        this->is_below = false;
                    } else {
                        this->is_above = false;
                        this->is_below = true;
                    }
                    return false;
                }

                if (this->is_above && this->is_below)
                    throw Exceptions::Strategies::Logic_Exception(
                        "Logical error in Strategy. Something can't be",
                        1,
                        0
                    );

                if (this->is_below && crossing_value > what_value)
                {
                    this->is_above = true;
                    this->is_below = false;

                    return true;
                }

                return false;
            }

            /**
             * @brief Does crossing value crossed what value below?
             * 
             * @param crossing_value Crossing value
             * @param what_value What crossing_value is crossing
             * @return true 
             * @return false 
             */
            bool cross_below(double crossing_value, double what_value)
            {
                if (!this->is_above && !this->is_below)
                {
                    if (crossing_value > what_value)
                    {
                        this->is_above = true;
                        this->is_below = false;
                    } else {
                        this->is_above = false;
                        this->is_below = true;
                    }
                    return false;
                }

                if (this->is_above && this->is_below)
                    throw Exceptions::Strategies::Logic_Exception(
                        "Logical error in Strategy. Something can't be",
                        1,
                        0
                    );

                if (this->is_above && crossing_value < what_value)
                {
                    this->is_above = false;
                    this->is_below = true;

                    return true;
                }

                return false;
            }

        public:
            /**
             * @brief Construct a new hma cci strategy object
             */
            HMA_CCI_Strategy() = default;

            /**
             * @brief Destroy the hma cci strategy object
             */
            ~HMA_CCI_Strategy() = default;

            /**
             * @brief Resolve Strategy
             * 
             * @param params Parameters for strategy (indicators value)
             * @param signals Signals of resolving (sell or buy)
             */
            void resolve(nlohmann::json &params, std::map<std::string, bool> &signals)
            {
                signals["long_open"] = false;
                signals["long_close"] = false;
                signals["short_open"] = false;
                signals["short_close"] = false;
                
                if (this->last_hma == 0.0)
                {
                    this->last_hma = params["hma"]["value"];
                    return;
                }

                double hma = params["hma"]["value"];
                double cci = params["cci"]["value"];

                if (
                    this->last_hma < hma &&
                    this->cross_above(cci, this->cci_lower_line)
                )
                    signals["long_open"] = true;
                if (
                    cci > this->cci_upper_exit
                )
                    signals["long_close"] = true;
                if (
                    this->last_hma > hma &&
                    this->cross_below(cci, this->cci_upper_line)
                )
                    signals["short_open"] = true;
                if (
                    cci < this->cci_lower_exit
                )
                    signals["short_close"] = true;

                this->last_hma = params["hma"]["value"];
            }
    };
}

/**
 * @brief Strategies from TradingView for Bot
 */
namespace Strategies::TradingView
{

}






















#endif