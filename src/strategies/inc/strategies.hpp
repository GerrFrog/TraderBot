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
 * @brief All Strategies in Bot
 */
namespace Strategies
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
            EMA_Cross() 
            { }

            /**
             * @brief Destroy the ema cross object
             */
            ~EMA_Cross() 
            { }

            /**
             * @brief Resolve EMA Cross Strategy
             * 
             * @param params Short and Long EMAs values
             * @param signals Signals of resolving (sell or buy)
             */
            void resolve(nlohmann::json &params, std::map<std::string, bool> &signals)
            {
                signals["buy"] = false;
                signals["sell"] = false;

                if (this->cross_above(params["short_ema"]["value"], params["long_ema"]["value"])) 
                    signals["buy"] = true;
                if (this->cross_below(params["short_ema"]["value"], params["long_ema"]["value"]))
                    signals["sell"] = true;
            }
    };
}
























#endif