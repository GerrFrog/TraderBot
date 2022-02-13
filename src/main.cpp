#include "main.hpp"

using std::cout, std::string, std::endl;

int main(int argc, char *argv[]) {
    dotenv::load("../.env");

    std::ifstream ifs("../config.json");
    nlohmann::json jf = nlohmann::json::parse(ifs);

    const std::string mode = dotenv::get("mode");

    nlohmann::json worker_configuration = jf["workers"]["worker_1"];
    string dir = "../data/";

    double usdt_balance = 10000.0;
    double symbol_balance = 20.0;

    Workers::Worker<Strategies::RSXC_ADX, Candles::Candle> worker_cd(
        worker_configuration
    );
    Workers::Worker<Strategies::RSXC_ADX, Candles::Heikin_Ashi> worker_ha(
        worker_configuration
    );

    worker_cd.initialize();
    worker_ha.initialize();

    Tester tester;

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


    // // STRATEGY BACKTEST
    // worker_cd.backtest_candle(
    //     dir,
    //     usdt_balance,
    //     symbol_balance
    // );
    // worker_ha.backtest_heikin_ashi(
    //     dir,
    //     usdt_balance,
    //     symbol_balance
    // );

    // // BACKTEST CANDLE
    // tester.backtest_candle(
    //     "BTCUSDT",
    //     "1d",
    //     dir
    // );

    // // BACKTEST INDICATORS
    // tester.backtest_candle_indicator<Indicators::TradingView::RSXC_LB<Candles::Candle>>(
    //     indicator_params,
    //     "BTCUSDT",
    //     "1d",
    //     dir
    // );
    // tester.backtest_heikin_ashi_indicator<Indicators::TradingView::Normalized_MACD<Candles::Heikin_Ashi>>(
    //     indicator_params2,
    //     "BTCUSDT",
    //     "1d",
    //     dir
    // );

    return EXIT_SUCCESS;
}







