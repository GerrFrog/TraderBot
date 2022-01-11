cmake_minimum_required( VERSION 3.1.0 )

project( TraderBot VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
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
)

set(
    LIBS_HEADER_FILES
    ${LIBS_FILES}/binapi/include/binapi/api.hpp
    ${LIBS_FILES}/binapi/include/binapi/flatjson.hpp
    ${LIBS_FILES}/binapi/include/binapi/dtf.hpp
    ${LIBS_FILES}/binapi/include/binapi/double_type.hpp
    ${LIBS_FILES}/binapi/include/binapi/enums.hpp
    ${LIBS_FILES}/binapi/include/binapi/errors.hpp
    ${LIBS_FILES}/binapi/include/binapi/invoker.hpp
    ${LIBS_FILES}/binapi/include/binapi/message.hpp
    ${LIBS_FILES}/binapi/include/binapi/pairslist.hpp
    ${LIBS_FILES}/binapi/include/binapi/reports.hpp
    ${LIBS_FILES}/binapi/include/binapi/tools.hpp
    ${LIBS_FILES}/binapi/include/binapi/types.hpp
    ${LIBS_FILES}/binapi/include/binapi/websocket.hpp
    ${LIBS_FILES}/dotenv/include/dotenv.hpp
)

set(
    LIBS_IMPLEMENTED_FILES
    ${LIBS_FILES}/binapi/src/api.cpp 
    ${LIBS_FILES}/binapi/src/enums.cpp
    ${LIBS_FILES}/binapi/src/errors.cpp
    ${LIBS_FILES}/binapi/src/pairslist.cpp
    ${LIBS_FILES}/binapi/src/reports.cpp
    ${LIBS_FILES}/binapi/src/tools.cpp
    ${LIBS_FILES}/binapi/src/types.cpp
    ${LIBS_FILES}/binapi/src/websocket.cpp
    ${LIBS_FILES}/dotenv/src/dotenv.cpp
)
############## END SOURCE FILES ###########################

##################### PACKAGES#######################
find_package( Threads REQUIRED )
find_package( OpenSSL REQUIRED )
find_package( Boost COMPONENTS system REQUIRED )
find_package( CURL REQUIRED )
find_package( Threads REQUIRED )
find_package( OpenSSL REQUIRED )
find_package( nlohmann_json REQUIRED )
####################### END PACKAGES#######################

#################### LINKING ##########################
include_directories(
    src/libs/binapi/include
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
    # /usr/local/lib/libTgBot.a
    Threads::Threads
    OpenSSL::SSL
    nlohmann_json::nlohmann_json
    # cpr
    # CONAN_PKG::cpr # this
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
    ${PYTHON_LIBRARIES}
    -L${BOOST_LIB_DIR}
)
#################### END LINKING ####################################







