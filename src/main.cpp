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

    // std::ifstream ifs("../config.json");
    // json jf = json::parse(ifs);

    // for (auto& [key, value] : jf.items()) {
    //     std::cout << key << " : " << value << "\n";
    // }

    // for (auto& el : jf.items()) {
    //     std::cout << el.key() << " : " << el.value() << "\n";
    // }

    // cout <<"\n\n";

    // recursive_iterate(jf, [](json::const_iterator it){
    //     std::cout << *it << std::endl;
    // });

    // cout << "\n\n";

    // for (const auto& item : jf.items())
    // {
    //     std::cout << item.key() << "\n";
    //     // if (item.value().is_object())
    //     //     cout << " Object";
    //     for (const auto& val : item.value().items())
    //     {
    //         std::cout << "  " << val.key() << ": " << val.value() << "\n";
    //     }
    // }

    Managers::Workers_Manager worker(taapi_key);

    worker.foo();

    return EXIT_SUCCESS;
}