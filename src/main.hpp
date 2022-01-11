#include <iostream>
#include <boost/asio/io_context.hpp>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

#include "libs/binapi/include/binapi/api.hpp"
#include "libs/dotenv/include/dotenv.hpp"
#include "managers/inc/managers.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[]);

// TODO: Bot heartbeat
// TODO: GUI managment
// TODO: Telebot managment
// TODO: API managment
// TODO: Site