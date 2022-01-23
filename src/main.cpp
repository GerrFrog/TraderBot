#include "main.hpp"

using std::cout, std::string, std::endl;

template<class UnaryFunction>
void recursive_iterate(const json& j, UnaryFunction f)
{
    for(auto it = j.begin(); it != j.end(); ++it)
    {
        if (it->is_structured())
        {
            recursive_iterate(*it, f);
        } else {
            f(it);
        }
    }
}

int main(int argc, char *argv[]) {
    dotenv::load("../.env");

    const std::string pk = dotenv::get("PRIVATE_KEY");
    const std::string sk = dotenv::get("SECRET_KEY");
    const std::string taapi_key = dotenv::get("TAAPI_KEY");

    // // CSV
    // io::CSVReader<3> in("../data/BTCUSDT_1d.csv");
    // string first, second, third;
    // while(in.read_row(first, second, third))
    //     cout << "First: " << first << ' '
    //          << "Second: " << second << ' '
    //          << "Third: " << third << ' '
    //          << endl
    //         ;

    // // Binance API
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

    // // Initialize 
    std::ifstream ifs("../config.json");
    json jf = json::parse(ifs);

    // cout << jf["timeframes"].size() << endl;

    // for (auto& elem : jf["timeframes"])
    //     timeframes.push_back(elem);

    // cout << Indicators::TAAPI::EMA(taapi_key, "BTC/USDT", "1h", 5) << endl;

    nlohmann::json strategies_config = jf["strategies"];
    Director director(jf["strategies"], taapi_key);

    director.run();

    return EXIT_SUCCESS;
}