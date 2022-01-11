#pragma once

#ifndef MANAGER_HEADER
#define MANAGER_HEADER

#include <string>
#include <iostream>
#include <map>

#include "../../workers/inc/workers.hpp"

using std::map, std::string, std::cout, std::endl;

/**
 * @brief All Managers for working
 */
namespace Managers
{
    /**
     * @brief Workers Manager
     */
    class Workers_Manager
    {
        private:
            /**
             * @brief taapi.io key
             */
            string taapi_key;

            /**
             * @brief EMA Cross Strategy worker object
             */
            Workers::EMA_Cross_Worker ema_worker;

        public:
            /**
             * @brief Construct a new Trade_Manager object
             * 
             * @param key taapi.io key
             */
            Workers_Manager(const string &key) : taapi_key(key) { }

            void foo()
            {
                // Workers::EMA_Cross_Worker ema_worker(this->taapi_key, "BTC/USDT", "1m", 5, 25);
                this->ema_worker.set_taapi_key(this->taapi_key);
                this->ema_worker.set_symbol("BTC/USDT");
                this->ema_worker.set_interval("1m");
                this->ema_worker.set_short_period(5);
                this->ema_worker.set_long_period(25);

                map<string, string> description;

                ema_worker.describe_worker(description);

                for (auto const& [key, val] : description)
                {
                    std::cout << key << " : " << val << endl;
                }

                ema_worker.resolve();

                cout << "Buy signal: " << ema_worker.get_buy_signal() << endl;
                cout << "Sell signal: " << ema_worker.get_sell_signal() << endl;
                cout << endl;
            }
    };
}






















#endif