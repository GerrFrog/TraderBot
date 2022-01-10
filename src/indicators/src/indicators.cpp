#include "../inc/indicators.hpp"

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

    // Request::Simple::JSON_Curl json_curl(Indicators::TAAPI::taapi_url + "ema");
    Request::Simple::JSON_Curl json_curl("https://api.taapi.io/ema");

    json_curl.construct_request(params);

    nlohmann::json res = json_curl.request();

    // TODO: Throw Request limit error
    if (res.contains("value")) 
    {
        return res["value"];
    }

    return NULL;
};