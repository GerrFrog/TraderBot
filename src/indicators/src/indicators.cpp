#include "../inc/indicators.hpp"

std::string Indicators::TAAPI::get_taapi_url()
{
    return "https://api.taapi.io/";
}

nlohmann::json Indicators::TAAPI::EMA::get (
    const std::string &url,
    const std::string &key,
    const std::string &symbol, 
    const std::string &interval, 
    nlohmann::json &indicator_params
) {
    std::map<std::string, std::string> params;

    int period = indicator_params["period"];

    params["secret"] = key;
    params["exchange"] = "binance";
    params["symbol"] = symbol;
    params["interval"] = interval;
    params["optInTimePeriod"] = std::to_string(period);

    Request::Simple::JSON_Curl json_curl(url + "ema");

    json_curl.construct_request(params);

    nlohmann::json res = json_curl.request();

    if (res.contains("error"))
        throw Exceptions::TAAPI::Rate_Limit(res["error"], 429, 0);
    
    return res;
};