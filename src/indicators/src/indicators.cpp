#include "../inc/indicators.hpp"

std::string Indicators::TAAPI::get_taapi_url()
{
    return "https://api.taapi.io/";
}

double Indicators::TAAPI::EMA (
    const std::string &key,
    const std::string &symbol, 
    const std::string &interval, 
    const int &period
) {
    std::map<std::string, std::string> params;

    params["secret"] = key;
    params["exchange"] = "binance";
    params["symbol"] = symbol;
    params["interval"] = interval;
    params["optInTimePeriod"] = std::to_string(period);

    Request::Simple::JSON_Curl json_curl(Indicators::TAAPI::get_taapi_url() + "ema");

    json_curl.construct_request(params);

    nlohmann::json res = json_curl.request();

    if (res.contains("value")) 
        return res["value"];

    if (res.contains("error"))
        throw Exceptions::TAAPI::Rate_Limit(res["error"], 429, 0);

    throw Exceptions::Panic::Panic_Exception("Something went wrong in indicators!", 1, 0);
};