#pragma once

#ifndef CANDLE_HEADER
#define CANDLE_HEADER

#include <iostream>
#include <string>

using std::string, std::cout, std::endl;

/**
 * @brief Everything about candle
 */
class Candle
{
    private:
        /**
         * @brief Open time 
         */
        double open_time;

        /**
         * @brief Close time
         */
        double close_time;

        /**
         * @brief Open price
         */
        double open_price;

        /**
         * @brief High price
         */
        double high_price;

        /**
         * @brief Low price
         */
        double low_price;

        /**
         * @brief Close price
         */
        double close_price;

        /**
         * @brief Volume
         */
        double volume;

        /**
         * @brief Quote asset volume
         */
        double quote_asset_volume;

        /**
         * @brief Number of trades
         */
        double trades_count;

        /**
         * @brief Taker buy base asset volume
         */
        double taker_buy_base_asset_volume;

        /**
         * @brief Taker buy quote asset volume
         */
        double taker_buy_quote_asset_volume;

    public:
        /**
         * @brief Construct a new Candle object
         * 
         * @param open_time Open time
         * @param close_time Close time
         * @param open_price Open price
         * @param high_price High price
         * @param low_price Low price
         * @param close_price Close price
         * @param volume Volume 
         * @param quote Quote asset volume
         * @param trades_count Number of trades
         * @param tbbav Taker buy base asset volume
         * @param tbqav Taker buy quote asset volume
         */
        Candle(
            double open_time,
            double close_time,
            double open_price,
            double high_price,
            double low_price,
            double close_price,
            double volume,
            double quote,
            double trades_count,
            double tbbav,
            double tbqav
        ) : taker_buy_quote_asset_volume(tbqav),
            taker_buy_base_asset_volume(tbbav),
            trades_count(trades_count),
            quote_asset_volume(quote),
            volume(volume),
            close_price(close_price),
            low_price(low_price),
            high_price(high_price),
            open_price(open_price),
            close_time(close_time),
            open_time(open_time)
        { }

        /**
         * @brief Destroy the Candle object
         */
        ~Candle() { }

        /**
         * @brief Get the close price 
         * 
         * @return double 
         */
        double get_close_price() { return this->close_price; }
};
























#endif