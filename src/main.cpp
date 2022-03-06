#include "main.hpp"

using std::cout, std::string, std::endl;

int main(int argc, char *argv[]) {
    dotenv::load("../.env");

    std::ifstream ifs("../config.json");

    nlohmann::json jf = nlohmann::json::parse(ifs);
    nlohmann::json worker_configuration_1 = jf["workers"]["worker_1"];
    nlohmann::json worker_configuration_2 = jf["workers"]["worker_2"];
    nlohmann::json exchange = jf["exchange"];

    const string mode = dotenv::get("mode");
    const string dir = "../data/";
    const string pk = jf["exchange"]["binance"]["private_key"];
    const string sk = jf["exchange"]["binance"]["secret_key"];

    double usdt_balance = 10000.0;
    double symbol_balance = 20.0;

    Workers::Customs::Worker<Strategies::Customs::RSXC_ADX_Strategy, Candles::Candle> worker_1(
        worker_configuration_1,
        exchange,
        dir
    );
    // Workers::Customs::Worker<Strategies::Customs::HMA_CCI_Strategy, Candles::Candle> worker_2(
    //     worker_configuration_2,
    //     exchange,
    //     dir
    // );

    Tester tester;

    // // ONLINE TRADING FOR REAL MONEY WITH WORKER
    worker_1.start();
    // worker_2.start();

    // // ONLINE TRADING FOR WORKER
    // worker_1.online_backtest(
    //     usdt_balance,
    //     symbol_balance
    // );
    // worker_2.online_backtest(
    //     usdt_balance,
    //     symbol_balance
    // );

    // // STRATEGY BACKTEST FOR WORKER
    // worker_1.file_backtest(
    //     usdt_balance,
    //     symbol_balance
    // );
    // worker_2.file_backtest(
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
    // nlohmann::json indicator_params({
    //     {"period", 20}
    // });
    // tester.backtest_indicator<Indicators::Integral::CCI<Candles::Candle>, Candles::Candle>(
    //     indicator_params,
    //     "ALGOUSDT",
    //     "1d",
    //     dir
    // );

    // // TELEGRAM BOT
    // map<string, string> message_params;
    // TG_Bot tg_bot(jf["telegram"]["token"]);
    // message_params["hello"] = "world";
    // tg_bot.initialize();
    // tg_bot.set_message_params(message_params);
    // tg_bot.run();

    // Exchanges::Binance::Binance_API binapi(
    //     pk,
    //     sk
    // );

    // nlohmann::json response = binapi.open_new_order(
    //     "ALGOUSDT",
    //     "SELL",
    //     "19"
    // );

    // cout << response << endl;

    return EXIT_SUCCESS;
}







