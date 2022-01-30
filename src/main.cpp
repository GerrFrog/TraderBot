#include "main.hpp"

using std::cout, std::string, std::endl;

int main(int argc, char *argv[]) {
    // // Load Configuration for Bot
    dotenv::load("../.env");

    std::ifstream ifs("../config.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);

    const std::string mode = dotenv::get("mode");
    const std::string taapi_key = jf["taapi"]["key"];
    const std::string plan = jf["taapi"]["plan"];
    const std::string pk = jf["exchange"]["binance"]["private_key"];
    const std::string sk = jf["exchange"]["binance"]["secret_key"];
    std::vector<string> binance_pairs = jf["exchange"]["binance"]["tradable_pairs"];

    // // BINANCE API
    // boost::asio::io_context ioctx;
    // binapi::rest::api api(
    //     ioctx
    //     ,"api.binance.com"
    //     ,"443"
    //     ,pk
    //     ,sk
    //     ,10000 
    // );

    // auto account = api.account_info();
    // if ( !account ) {
    //     std::cerr << "account info error: " << account.errmsg << std::endl;
    //     return EXIT_FAILURE;
    // }

    // std::cout << "account info: " << account.v << std::endl << std::endl;

    // // START JOB
    // Managers::Manager manager(
    //     jf["strategies"],
    //     taapi_key
    // );
    // manager.run();

    // // STRATEGY BACKTEST
    Tester tester(
        "../data/",
        taapi_key
    );
    // tester.backtest_strategy<Strategies::EMA_Cross>(
    //     jf["strategies"]["ema_cross"]["1"],
    //     10000.0
    // );
    // tester.backtest_strategy<Strategies::Normalized_MACD_Cross>(
    //     jf["strategies"]["normalized_macd_cross"]["1"],
    //     10000.0,
    //     1
    // );
    nlohmann::json indicator_params = {
        {"interval", "1d"},
        {"period", 30}
    };
    tester.backtest_indicator<Indicators::Integral::SMA>(
        indicator_params,
        "BTCUSDT",
        "1d"
    );

    return EXIT_SUCCESS;
}







