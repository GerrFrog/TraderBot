#include <iostream>
#include <boost/asio/io_context.hpp>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <vector>

#include "libs/binapi/include/binapi/api.hpp"
#include "libs/dotenv/include/dotenv.hpp"
#include "director/inc/director.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[]);
