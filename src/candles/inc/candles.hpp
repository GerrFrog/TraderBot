#pragma once

#ifndef CANDLE_HEADER
#define CANDLE_HEADER

#include <iostream>
#include <string>

using std::string, std::cout, std::endl;

namespace Candles
{
    class Kline
    {
        protected:
            /**
             * @brief Candle set status
             */
            bool set = false;

            /**
             * @brief True - green, False - red
             */
            bool green;

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
             * @brief Absolute change of candle
             */
            double change_absolute;

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
             * @brief Construct a new Kline object
             */
            Kline() = default;

            /**
             * @brief Destroy the Kline object
             */
            virtual ~Kline() = default;

            /**
             * @brief Is Candle set?
             * 
             * @return true 
             * @return false 
             */
            bool is_set() { return this->set; }

            /**
             * @brief Is candle is green?
             * 
             * @return true 
             * @return false 
             */
            bool is_green() { return this->green; }

            /**
             * @brief Get the open time object
             * 
             * @return double 
             */
            double get_open_time() { return this->open_time; }

            /**
             * @brief Get the close time object
             * 
             * @return double 
             */
            double get_close_time() { return this->close_time; }

            /**
             * @brief Get the close price 
             * 
             * @return double 
             */
            double get_close_price() { return this->close_price; }

            /**
             * @brief Get the open price object
             * 
             * @return double 
             */
            double get_open_price() { return this->open_price; }

            /**
             * @brief Get the low price object
             * 
             * @return double 
             */
            double get_low_price() { return this->low_price; }

            /**
             * @brief Get the high price object
             * 
             * @return double 
             */
            double get_high_price() { return this->high_price; }

            /**
             * @brief Get the change absolute
             * 
             * @return double 
             */
            double get_change_abs() { return this->change_absolute; }
    };

    /**
     * @brief Everything about candle
     */
    class Candle : public Candles::Kline
    {
        public:
            /**
             * @brief Construct a new Candle object
             */
            Candle() = default;

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
            )
            { 
                this->open_time = open_time;
                this->close_time = close_time;
                this->open_price = open_price;
                this->high_price = high_price;
                this->low_price = low_price;
                this->close_price = close_price;
                this->volume = volume;
                this->quote_asset_volume = quote;
                this->trades_count = trades_count;
                this->taker_buy_base_asset_volume = tbbav;
                this->taker_buy_quote_asset_volume = tbqav;

                this->set = true;
                if (this->close_price >= this->open_price)
                {
                    this->green = true;
                } else {
                    this->green = false;
                }
                if (this->green) {
                    this->change_absolute = close_price - open_price;
                } else {
                    this->change_absolute = open_price - close_price;
                }
            }

            /**
             * @brief Destroy the Candle object
             */
            virtual ~Candle() = default;

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
            void construct(
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
            )
            {
                this->open_time = open_time;
                this->close_time = close_time;
                this->open_price = open_price;
                this->high_price = high_price;
                this->low_price = low_price;
                this->close_price = close_price;
                this->volume = volume;
                this->quote_asset_volume = quote;
                this->trades_count = trades_count;
                this->taker_buy_base_asset_volume = tbbav;
                this->taker_buy_quote_asset_volume = tbqav;

                this->set = true;
                if (this->close_price >= this->open_price)
                {
                    this->green = true;
                } else {
                    this->green = false;
                }
                if (this->green) {
                    this->change_absolute = close_price - open_price;
                } else {
                    this->change_absolute = open_price - close_price;
                }
            }
    };

    /**
     * @brief Heikin Ashi candle
     */
    class Heikin_Ashi : public Candles::Kline
    {
        public:
            /**
             * @brief Construct a new Heikin Ashi object
             */
            Heikin_Ashi() = default;

            /**
             * @brief Construct a new Heikin Ashi object
             * 
             */
            Heikin_Ashi(
                Candles::Candle &candle,
                double close_prev,
                double open_prev
            )
            { 
                this->set = true;

                this->open_price = (open_prev + close_prev) / 2;
                this->close_price = (
                    candle.get_open_price() + candle.get_high_price() +
                    candle.get_low_price() + candle.get_close_price()
                ) / 4;

                this->high_price = ((candle.get_high_price() > open_prev && candle.get_high_price() > close_prev) 
                                ? candle.get_high_price() : (open_prev < close_prev) ? open_prev : close_prev);
                this->low_price = ((candle.get_low_price() < open_prev && candle.get_low_price() < close_prev) 
                                ? candle.get_low_price() : (open_prev < close_prev) ? open_prev : close_prev);

                if (this->close_price >= this->open_price)
                {
                    this->green = true;
                } else {
                    this->green = false;
                }
                if (this->green) {
                    this->change_absolute = close_price - open_price;
                } else {
                    this->change_absolute = open_price - close_price;
                }
            }

            /**
             * @brief Destroy the Heikin Ashi object
             */
            virtual ~Heikin_Ashi() = default;

            /**
             * @brief Construct new Heikin Ashi Candle
             * 
             * @param candle Candle
             * @param close_prev Previous Heikin Ashi close price
             * @param open_prev Previous Heikin Ashi open price
             */
            void construct(
                Candle &candle,
                double close_prev,
                double open_prev
            ) 
            {
                this->set = true;

                this->open_price = (open_prev + close_prev) / 2;
                this->close_price = (
                    candle.get_open_price() + candle.get_high_price() +
                    candle.get_low_price() + candle.get_close_price()
                ) / 4;

                this->high_price = ((candle.get_high_price() > open_prev && candle.get_high_price() > close_prev) 
                                ? candle.get_high_price() : (open_prev < close_prev) ? open_prev : close_prev);
                this->low_price = ((candle.get_low_price() < open_prev && candle.get_low_price() < close_prev) 
                                ? candle.get_low_price() : (open_prev < close_prev) ? open_prev : close_prev);

                if (this->close_price >= this->open_price)
                {
                    this->green = true;
                } else {
                    this->green = false;
                }
                if (this->green) {
                    this->change_absolute = close_price - open_price;
                } else {
                    this->change_absolute = open_price - close_price;
                }
            }
    };
}

























#endif