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

        public:
            /**
             * @brief Construct a new Trade_Manager object
             * 
             * @param key taapi.io key
             */
            Workers_Manager(const string &key) : taapi_key(key) { }

            void foo()
            {
                Workers::EMA_Cross_Worker ema_worker(this->taapi_key, "BTC/USDT", "1h", 5, 25);
                map<string, string> description;

                ema_worker.describe_worker(description);

                for (auto const& [key, val] : description)
                {
                    std::cout << key << " : " << val << endl;
                } cout << "\n\n";

                ema_worker.resolve();
            }
    };
}






















#endif