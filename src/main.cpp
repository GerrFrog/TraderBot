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
    nlohmann::json indicator_params = {
        {"interval", "1d"},
        {"period", 14},
        {"smoothed", "RMA"},
        {"dmi_period", 14},
        {"dmi_smoothed", "RMA"},
        {"dmi_atr_smoothed", "RMA"},
        {"length", 14},
        {"src", "Close"}
    };
    nlohmann::json indicator_params2 = {
        {"interval", "1d"},
        {"type", "WMA"},
        {"sma", 12},
        {"lma", 21},
        {"tsp", 9},
        {"np", 50}
    };

    nlohmann::json strategy_params = jf["strategies"]["RSXC_ADX"]["1"];

    string sym = strategy_params["trader_params"]["symbol"];
    string symbol;
    string interval = strategy_params["trader_params"]["interval"];
    string dir = "../data/";

    std::remove_copy(
        sym.begin(),
        sym.end(),
        std::back_inserter(symbol),
        '/'
    );

    double usdt_balance = 10000.0;
    double symbol_balance = 20.0;

    io::CSVReader<11> data_file(dir + symbol + '_' + interval +".csv");

    Tester tester(
        dir,
        taapi_key
    );
    // tester.backtest_candle(
    //     "BTCUSDT",
    //     "1d",
    //     data_file
    // );
    // tester.backtest_candle_indicator<Indicators::TradingView::RSXC_LB<Candle>>(
    //     indicator_params,
    //     "BTCUSDT",
    //     "1d",
    //     data_file
    // );
    // tester.backtest_heikin_ashi_indicator<Indicators::TradingView::Normalized_MACD<Heikin_Ashi>>(
    //     indicator_params2,
    //     "BTCUSDT",
    //     "1d",
    //     data_file
    // );
    tester.backtest_strategy<Strategies::RSXC_ADX>(
        strategy_params,
        usdt_balance,
        symbol_balance,
        data_file
    );
    // tester.backtest_strategy<Strategies::Normalized_MACD_Cross>(
    //     jf["strategies"]["normalized_macd_cross"]["1"],
    //     10000.0,
    //     1,
    //     data_file
    // );

    return EXIT_SUCCESS;
}







