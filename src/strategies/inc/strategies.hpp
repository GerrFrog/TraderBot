#pragma once

#ifndef STRATEGIES_HEADER
#define STRATEGIES_HEADER

#include <string>
#include <iostream>
#include <map>

using std::cout, std::string, std::endl;

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
                {
                    // TODO: Throw Exception (can't be)
                    this->is_short_above = false;
                    this->is_short_below = false;

                    return false;
                }

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
                {
                    // TODO: Throw Exception (can't be)
                    this->is_short_above = false;
                    this->is_short_below = false;

                    return false;
                }

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
            EMA_Cross() { }

            /**
             * @brief Destroy the ema cross object
             */
            ~EMA_Cross() { }

            /**
             * @brief Resolve EMA Cross Strategy
             * 
             * @param short_ema Short EMA (e.g. 5 period)
             * @param long_ema Long EMA (e.g. 30 period)
             * @param signals Signals of resolving (sell or buy)
             */
            void resolve(double short_ema, double long_ema, std::map<std::string, bool> &signals)
            {
                signals["buy"] = false;
                signals["sell"] = false;

                if (this->cross_above(short_ema, long_ema)) 
                    signals["buy"] = true;
                if (this->cross_below(short_ema, long_ema))
                    signals["sell"] = true;
            }
    };
}
























#endif