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

using std::string, std::map, std::cout, std::endl,
      std::vector;

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
     * @brief Get EMA value
     * 
     * @param key Key of taapi.io
     * @param symbol Symbol (pair)
     * @param interval Interval
     * @param indicator_params Params for Indicator
     * @return double 
     */
    double EMA(
        const std::string &key,
        const std::string &symbol, 
        const std::string &interval, 
        nlohmann::json &indicator_params
    );
}

/**
 * @brief Integral indicators. Pass candle to calculate current value
 */
namespace Indicators::Integral
{
    // TODO: EMA with different sources (close, low, high...)
    /**
     * @brief Exponential Moving Average
     */
    class EMA
    {
        private:
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
             * @brief Last value of Candles from 1 to period
             */
            vector<double> last_candles;

        public:
            /**
             * @brief Construct a new EMA object
             * 
             * @param period Period for EMA
             */
            EMA(
                int period
            ) : period(period)
            { }

            /**
             * @brief Get the EMA for last passed Candle
             * 
             * @return double 
             */
            double get_ema() { return this->ema; }

            /**
             * @brief Get the period of EMA
             * 
             * @return double 
             */
            double get_period() { return this->period; }

            /**
             * @brief Resolve the EMA Indicator for new Candle
             * 
             * @param candle Candle object
             */
            void resolve(Candle &candle)
            {
                if (this->last_candles.size() < this->period)
                {
                    double curr = 0;

                    last_candles.push_back(
                        candle.get_close_price()
                    );

                    for (double &last_candle : this->last_candles)
                        curr += last_candle / this->last_candles.size();

                    this->ema = curr;
                    this->last_ema = curr;
                } else {
                    this->ema = 
                        (candle.get_close_price() - this->last_ema) * 2.0
                        / (this->period + 1.0) + this->last_ema
                    ;
                    this->last_ema = this->ema;
                }
            }
    };
}



















#endif