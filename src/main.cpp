#include "main.hpp"

// using namespace std;
using std::cout, std::string, std::endl;

int main(int argc, char *argv[]) {
    dotenv::load("../.env");

    const std::string pk = dotenv::get("PRIVATE_KEY");
    const std::string sk = dotenv::get("SECRET_KEY");
    const std::string taapi_key = dotenv::get("TAAPI_KEY");

    boost::asio::io_context ioctx;
    binapi::rest::api api(
        ioctx
        ,"api.binance.com"
        ,"443"
        ,pk
        ,sk
        ,10000 // recvWindow
    );

    auto account = api.account_info();
    if ( !account ) {
        std::cerr << "account info error: " << account.errmsg << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "account info: " << account.v << std::endl << std::endl;

    return EXIT_SUCCESS;
}