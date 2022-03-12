#include "main.hpp"

using std::cout, std::string, std::endl;

int main(int argc, char *argv[]) {
    dotenv::load("../.env");

    cxxopts::Options options(
        "./TraderBot",
        "Trading bot for cryptocurrency"
    );

    std::ifstream ifs("../config.json");

    string mode, worker_name, dir;

    double long_balance, short_balance;

    nlohmann::json jf = nlohmann::json::parse(ifs);
    nlohmann::json worker_configuration,
                   exchange = jf["exchange"]
    ;

    // const string http_proxy = dotenv::get("HTTP_PROXY"),
    //              https_proxy = dotenv::get("HTTPS_PROXY")
    // ;

    options.add_options()
        ("m,mode", "Working mode: backtest, real, online_backtest", cxxopts::value<std::string>()->default_value("online_backtest"))
        ("w,worker-name", "Worker name", cxxopts::value<string>()->default_value("worker_1"))
        ("d,dir", "Realtive path to directory with data", cxxopts::value<string>()->default_value("../data/"))
        ("l, long-balance", "Your start balance for long positions (e.g. BTC/USDT -> USDT balance)", cxxopts::value<double>()->default_value("10000.0"))
        ("s, short-balance", "Your start balance for short positions (e.g. BTC/USDT -> BTC balance)", cxxopts::value<double>()->default_value("20.0"))
        ("h,help", "Help for arguments list")
    ;

    auto result = options.parse(argc, argv);
    
    if (result.count("help"))
    {
        cout << options.help() << endl;
        exit(0);
    }

    mode = result["mode"].as<string>();
    worker_name = result["worker-name"].as<string>();
    dir = result["dir"].as<string>();
    long_balance = result["long-balance"].as<double>();
    short_balance = result["short-balance"].as<double>();

    if (result.count("mode"))
        mode = result["mode"].as<string>();
    if (result.count("worker-name"))
        worker_name = result["worker-name"].as<string>();
    if (result.count("dir"))
        dir = result["dir"].as<string>();
    if (result.count("long-balance"))
        long_balance = result["long-balance"].as<double>();
    if (result.count("short-balance"))
        short_balance = result["short-balance"].as<double>();

    worker_configuration = jf["workers"][worker_name];

    cout
        << "Input: " << endl
        << "Mode: " << mode << endl
        << "Worker name: " << worker_name << endl
        << "Dir: " << dir << endl
        << "Long: " << long_balance << endl
        << "Short: " << short_balance << endl
    << endl;

    // TODO: Take strategy and candle type from config
    if (mode == "online_backtest")
    {
        if (worker_name == "worker_1")
        {
            Workers::Customs::Worker<Strategies::Customs::RSXC_ADX_Scalping_Strategy, Candles::Candle> worker(
                worker_configuration,
                exchange,
                dir
            );
            worker.online_backtest(
                long_balance,
                short_balance
            );
        }
        if (worker_name == "worker_2")
        {
            Workers::Customs::Worker<Strategies::Customs::HMA_CCI_Strategy, Candles::Candle> worker(
                worker_configuration,
                exchange,
                dir
            );
            worker.online_backtest(
                long_balance,
                short_balance
            );
        }
    }
    if (mode == "backtest")
    {
        if (worker_name == "worker_1")
        {
            Workers::Customs::Worker<Strategies::Customs::RSXC_ADX_Scalping_Strategy, Candles::Candle> worker(
                worker_configuration,
                exchange,
                dir
            );
            worker.file_backtest(
                long_balance,
                short_balance
            );
        }
        if (worker_name == "worker_2")
        {
            Workers::Customs::Worker<Strategies::Customs::HMA_CCI_Strategy, Candles::Candle> worker(
                worker_configuration,
                exchange,
                dir
            );
            worker.file_backtest(
                long_balance,
                short_balance
            );
        }
    }
    if (mode == "real")
    {
        if (worker_name == "worker_1")
        {
            Workers::Customs::Worker<Strategies::Customs::RSXC_ADX_Scalping_Strategy, Candles::Candle> worker(
                worker_configuration,
                exchange,
                dir
            );
            worker.start();
        }
        if (worker_name == "worker_2")
        {
            Workers::Customs::Worker<Strategies::Customs::HMA_CCI_Strategy, Candles::Candle> worker(
                worker_configuration,
                exchange,
                dir
            );
            worker.start();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Tester tester;

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

    return EXIT_SUCCESS;
}







