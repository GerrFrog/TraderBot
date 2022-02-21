#include "main.hpp"

using std::cout, std::string, std::endl;

int main(int argc, char *argv[]) {
    dotenv::load("../.env");

    std::ifstream ifs("../config.json");

    nlohmann::json jf = nlohmann::json::parse(ifs);
    nlohmann::json worker_configuration = jf["workers"]["worker_1"];
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

    const string dir = "../data/";
    const string mode = dotenv::get("mode");
    const string pk = jf["exchange"]["binance"]["private_key"];
    const string sk = jf["exchange"]["binance"]["secret_key"];

    double usdt_balance = 2000.0;
    double symbol_balance = 5.0;

    Workers::Worker<Strategies::RSXC_ADX, Candles::Candle> worker(
        worker_configuration
    );
    Managers::Trade_Manager trade_manager(jf);
    Exchanges::Binance::Binance_API binapi(pk, sk);

    Tester tester;

    // // INITIALIZING
    // worker.configurate(jf["exchange"]);
    trade_manager.initialize();

    // // ONLINE TRADING FOR REAL MONEY
    trade_manager.describe_workers();
    trade_manager.online_start(dir);

    // // BINANCE API
    // nlohmann::json account_info = binapi.account_info();
    // cout << account_info << endl;
    // cout << binapi.open_new_order(
    //     "ALGOUSDT",
    //     "BUY",
    //     "20"
    // ) << endl;
    // cout << binapi.exchange_info("ALGOUSDT") << endl;
    // cout << binapi.get_balance("ALGO") << endl;

    // // TELEGRAM BOT
    // map<string, string> message_params;
    // TG_Bot tg_bot(jf["telegram"]["token"]);
    // message_params["hello"] = "world";
    // tg_bot.initialize();
    // tg_bot.set_message_params(message_params);
    // tg_bot.run();

    // // ONLINE TEST TRADING FOR MANAGER
    // trade_manager.describe_workers();
    // trade_manager.online_backtest(
    //     dir,
    //     usdt_balance,
    //     symbol_balance
    // );

    // // ONLINE TRADING FOR WORKER
    // worker.online_backtest(
    //     usdt_balance,
    //     symbol_balance,
    //     dir
    // );

    // // STRATEGY BACKTEST FOR WORKER
    // worker.file_backtest(
    //     dir,
    //     usdt_balance,
    //     symbol_balance
    // );

    // // BACKTEST CANDLE
    // tester.backtest_candle<Candles::Heikin_Ashi>(
    //     "BTCUSDT",
    //     "1d",
    //     dir
    // );

    // // BACKTEST INDICATORS
    // tester.backtest_indicator<Indicators::TradingView::RSXC_LB<Candles::Candle>, Candles::Candle>(
    //     indicator_params,
    //     "BTCUSDT",
    //     "1d",
    //     dir
    // );

    return EXIT_SUCCESS;
}







