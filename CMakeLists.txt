cmake_minimum_required( VERSION 3.1.0 )

project( TraderBot VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address" )
set( Boost_USE_MULTITHREADED ON )
set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
set( THREADS_PREFER_PTHREAD_FLAG TRUE )

################ VARIABLES ###########################
set( REQUEST_FILES src/request )
set( STRATEGIES_FILES src/strategies )
set( INDICATORS_FILES src/indicators )
set( MANAGERS_FILES src/managers )
set( TRADE_FILES src/trade )
set( WORKERS_FILES src/workers )
set( EXCEPTIONS_FILES src/exceptions )
set( TESTER_FILES src/tester )
set( CANDLES_FILES src/candles )
set( TGBOT_FILES src/tgbot )
set( EXCHANGES_FILES src/exchanges )
set( LIBS_FILES src/libs )
############# END VARIABLES ############################

############### SOURCE FILES ##############################
set(
    HEADER_FILES 
    src/main.hpp
    ${REQUEST_FILES}/inc/request.hpp
    ${STRATEGIES_FILES}/inc/strategies.hpp
    ${INDICATORS_FILES}/inc/indicators.hpp
    ${MANAGERS_FILES}/inc/managers.hpp
    ${TRADE_FILES}/inc/trade.hpp
    ${WORKERS_FILES}/inc/workers.hpp
    ${EXCEPTIONS_FILES}/inc/exceptions.hpp
    ${TESTER_FILES}/inc/tester.hpp
    ${CANDLES_FILES}/inc/candles.hpp
    ${TGBOT_FILES}/inc/tgbot.hpp
    ${EXCHANGES_FILES}/inc/exchanges.hpp
)
set(
    IMPLEMENTED_FILES
    src/main.cpp
    ${REQUEST_FILES}/src/request.cpp
    ${STRATEGIES_FILES}/src/strategies.cpp
    ${INDICATORS_FILES}/src/indicators.cpp
    ${MANAGERS_FILES}/src/managers.cpp
    ${TRADE_FILES}/src/trade.cpp
    ${WORKERS_FILES}/src/workers.cpp
    ${EXCEPTIONS_FILES}/src/exceptions.cpp
    ${TESTER_FILES}/src/tester.cpp
    ${CANDLES_FILES}/src/candles.cpp
    ${TGBOT_FILES}/src/tgbot.cpp
    ${EXCHANGES_FILES}/src/exchanges.cpp
)

set(
    LIBS_HEADER_FILES
    ${LIBS_FILES}/dotenv/include/dotenv.hpp
    ${LIBS_FILES}/csv/csv.hpp
)

set(
    LIBS_IMPLEMENTED_FILES
    ${LIBS_FILES}/dotenv/src/dotenv.cpp
)
############## END SOURCE FILES ###########################

##################### PACKAGES#######################
find_package( Threads REQUIRED )
find_package( Boost COMPONENTS system REQUIRED )
find_package( CURL REQUIRED )
find_package( OpenSSL REQUIRED )
find_package( nlohmann_json REQUIRED )
####################### END PACKAGES#######################

#################### LINKING ##########################
include_directories(
    src/libs/binapi/include
    /usr/local/include
    ${MYSQL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
)

add_definitions(
    -UNDEBUG
    -DDTF_HEADER_ONLY
    -D__FLATJSON__CHILDS_TYPE=std::uint32_t
    -D__FLATJSON__VLEN_TYPE=std::uint32_t
    -DHAVE_CURL
)

add_executable(
    ${PROJECT_NAME}
    ${HEADER_FILES}
    ${IMPLEMENTED_FILES}
    ${LIBS_HEADER_FILES}
    ${LIBS_IMPLEMENTED_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
    z
    crypto
    ssl
    pthread
    mysqlclient
    /usr/local/lib/libTgBot.a
    Threads::Threads
    OpenSSL::SSL
    nlohmann_json::nlohmann_json
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
    ${PYTHON_LIBRARIES}
    -L${BOOST_LIB_DIR}
)
#################### END LINKING ####################################







