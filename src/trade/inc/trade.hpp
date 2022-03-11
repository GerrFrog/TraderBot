#pragma once

#ifndef TRADE_HEADER
#define TRADE_HEADER

#include <chrono>
#include <ctime> 
#include <map>
#include <string>
#include <iostream>

using std::map, std::cout, std::endl, std::string;

/**
 * @brief Abstract classes for Trades
 */
namespace Trades::Abstract
{
    /**
     * @brief Abstract class for Trade objects
     */
    class Trade_Abstract
    {
        private:

        protected:
            /**
             * @brief Unique Trade id
             */
            unsigned int id;

            /**
             * @brief Activity state of trade
             */
            bool active = false;

            /**
             * @brief Completed status of trade
             */
            bool completed = false;

            /**
             * @brief The traded symbol
             */
            string symbol;

            /**
             * @brief Trade position (long/short)
             */
            string position;

            /**
             * @brief Open Time of Trade
             */
            uint64_t open_time;

            /**
             * @brief Close Time of Trade
             */
            uint64_t close_time;

            /**
             * @brief The Live time of trade
             */
            uint64_t live_time;

            /**
             * @brief Stake for open "long" trade (buy cryptocurrency with this stake)
             */
            double long_amount;

            /**
             * @brief Stake for open "short" trade
             */
            double short_amount;

            /**
             * @brief The price of cryptocurrency when trade was created
             */
            double open_price;

            /**
             * @brief The price of cryptocurrency when trade was closed
             */
            double close_price;

            /**
             * @brief Profit in USD
             */
            double profit;

            /**
             * @brief Profit in percentage
             */
            double per_profit;

        public:
            /**
             * @brief Construct a new Trade object
             * 
             * @param id ID of trade
             * @param long_amount Stake amount to open a "long" trade (USDT)
             * @param short_amount Stake amount of symbol to open a "short" trade
             * @param open_price Open Price when Trade was created
             * @param symbol The traded pair
             * @param interval Candles Interval
             */
            Trade_Abstract(
                unsigned int id,
                double long_amount, 
                double short_amount,
                double open_price, 
                const string &symbol,
                const string &position
            ) : id(id),
                symbol(symbol), 
                position(position),
                long_amount(long_amount),
                short_amount(short_amount),
                open_price(open_price)
            { 
                this->active = true;
                this->completed = false;
                this->open_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();
            }

            /**
             * @brief Destroy the Trade object
             */
            ~Trade_Abstract() = default;

            /**
             * @brief Overload of "==" operator
             * 
             * @param trade Trade object
             * @return true 
             * @return false 
             */
            bool operator ==(const Trade_Abstract& trade)
            {
                if (
                    trade.open_price == this->open_price &&
                    trade.position == this->position &&
                    trade.open_time == this->open_time &&
                    trade.long_amount == this->long_amount &&
                    trade.short_amount == this->short_amount &&
                    trade.symbol == this->symbol 
                ) return true;
                return false;
            }

            /**
             * @brief Get the position 
             * 
             * @return string 
             */
            string get_position() { return this->position; }

            /**
             * @brief Get the stake amount 
             * 
             * @return double 
             */
            double get_long_amount() { return this->long_amount; }

            /**
             * @brief Get the symbol amount
             * 
             * @return double 
             */
            double get_short_amount() { return this->short_amount; }

            /**
             * @brief Get the open price 
             * 
             * @return double 
             */
            double get_open_price() { return this->open_price; }

            /**
             * @brief Get the close price
             * 
             * @return double 
             */
            double get_close_price() { return this->close_price; }

            /**
             * @brief Get the abs profit 
             * 
             * @return double 
             */
            double get_abs_profit() { return this->profit; }

            /**
             * @brief Get the per profit 
             * 
             * @return double 
             */
            double get_per_profit() { return this->per_profit; }

            /**
             * @brief Is trade active?
             * 
             * @return true 
             * @return false 
             */
            bool is_active() { return this->active; }

            /**
             * @brief Is trade completed?
             * 
             * @return true 
             * @return false 
             */
            bool is_completed() { return this->completed; }

            /**
             * @brief Describe the trade
             */
            void describe_trade()
            {
                cout << "ID: " << this->id << endl
                    << "Symbol: " << this->symbol << endl
                    << "Position: " << this->position << endl
                    << "Open price: " << this->open_price << endl
                    << "Long amount: " << this->long_amount << endl
                    << "Short amount: " << this->short_amount << endl;
            }
    };
}

/**
 * @brief Implemented Trades objects
 */
namespace Trades
{
    /**
     * @brief Trade for backtesting
     */
    class Backtest_Trade : virtual public Trades::Abstract::Trade_Abstract
    {
        private:

        public:
            /**
             * @brief Construct a new Backtest Trade object
             * 
             * @param id ID of trade
             * @param long_amount Stake amount to open a "long" trade (USDT)
             * @param short_amount Stake amount of symbol to open a "short" trade
             * @param open_price Open Price when Trade was created
             * @param symbol The traded pair
             * @param interval Candles Interval
             */
            Backtest_Trade(
                unsigned int id,
                double long_amount, 
                double short_amount,
                double open_price, 
                const string &symbol,
                const string &position
            ) : Trade_Abstract(
                    id,
                    long_amount,
                    short_amount,
                    open_price,
                    symbol,
                    position
                )
            { }

            /**
             * @brief Destroy the Backtest_Trade object
             */
            virtual ~Backtest_Trade() = default;

            /**
             * @brief Close trade
             * 
             * @param close_p Close price
             * @param cummulative_qty Cummulative quote quantity
             * @param executed_qty Executed quantity
             * @param commission Commission
             */
            void close_trade(
                double close_p
            ) {
                this->active = false;
                this->completed = true;

                this->close_price = close_p;
                this->close_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();
                this->live_time = close_time - open_time;

                double percentage = close_p / this->open_price * 100;
                double total_monets;

                if (this->position == "long")
                {
                    this->per_profit = percentage - 100;
                    this->profit = this->long_amount * percentage / 100 - this->long_amount;
                } else if (this->position == "short") {
                    total_monets = (this->short_amount * this->open_price) /
                        close_p;
                    this->per_profit = (total_monets / this->short_amount - 1) * 100;
                    this->profit = total_monets - this->short_amount;
                }
            }
    };

    /**
     * @brief Trade for Online trading
     */
    class Online_Trade : virtual public Trades::Abstract::Trade_Abstract
    {
        private:
            /**
             * @brief When you buy and open long trade it holds first symbol amount to sell and exit trade
             */
            double long_holding;

            /**
             * @brief When you sell and open short trade it holds second symbol amount to buy and exit trade
             */
            double short_holding;

            /**
             * @brief Cummulative quote quantity when open trade(in second symbol, e.g. USDT).
             * Your order in second symbol.
             */
            double open_cummulativeQuoteQty;

            /**
             * @brief Executed quantity when open trade (in first symbol, e.g. BTC).
             * How much first symbol you bought/sell
             */
            double open_executedQty;

            /**
             * @brief Commission when open trade
             */
            double open_commission;

            /**
             * @brief Cummulative quote quantity when close trade(in second symbol, e.g. USDT).
             * Your order in second symbol.
             */
            double close_cummulativeQuoteQty;

            /**
             * @brief Executed quantity when open trade (in first symbol, e.g. BTC).
             * How much first symbol you bought/sell
             */
            double close_executedQty;

            /**
             * @brief Commission when close trade
             */
            double close_commission;

        public:
            /**
             * @brief Construct a new Backtest Trade object
             * 
             * @param id ID of trade
             * @param long_amount Stake amount to open a "long" trade (USDT)
             * @param short_amount Stake amount of symbol to open a "short" trade
             * @param open_price Open Price when Trade was created
             * @param symbol The traded pair
             * @param interval Candles Interval
             * @param cummmulative_qty Cummulative quote quantity
             * @param executed_qty Executed quantity
             * @param commission Commission
             * @param long_hold Holding for long trade
             * @param short_hold Holding for short trade
             */
            Online_Trade(
                unsigned int id,
                double long_amount, 
                double short_amount,
                double open_price, 
                const string &symbol,
                const string &position,
                double cummulative_qty,
                double executed_qty,
                double commission,
                double long_hold,
                double short_hold
            ) : Trade_Abstract(
                    id,
                    long_amount,
                    short_amount,
                    open_price,
                    symbol,
                    position
                ),
                long_holding(long_hold),
                short_holding(short_hold),
                open_cummulativeQuoteQty(cummulative_qty),
                open_executedQty(executed_qty),
                open_commission(commission)
            { }

            /**
             * @brief Get the long holding object
             * 
             * @return double 
             */
            double get_long_holding() { return this->long_holding; }

            /**
             * @brief Get the short holding object
             * 
             * @return double 
             */
            double get_short_holding() { return this->short_holding; }

            /**
             * @brief Close trade
             * 
             * @param close_p Close price
             * @param cummulative_qty Cummulative quote quantity
             * @param executed_qty Executed quantity
             * @param commission Commission
             */
            void close_trade(
                double close_p,
                double cummulative_qty = 0,
                double executed_qty = 0,
                double commission = 0
            ) {
                this->active = false;
                this->completed = true;

                this->close_price = close_p;
                this->close_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count();
                this->live_time = close_time - open_time;

                this->close_cummulativeQuoteQty = cummulative_qty;
                this->close_executedQty = executed_qty;
                this->close_commission = commission;
            
                // TODO: Calculate profit
            }
    };
}























#endif