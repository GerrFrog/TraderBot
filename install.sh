#!/bin/bash
set -eu -o pipefail

sudo -n true

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

echo "Start installing dependencies"

bin_dir="bin/"
data_dir="data/"
backtest_dir="backtest_results/"

[ ! -d "$bin_dir" ] && mkdir -p "$bin_dir"
[ ! -d "$data_dir" ] && mkdir -p "$data_dir"
[ ! -d "$backtest_dir" ] && mkdir -p "$backtest_dir"

while read -r p ; do sudo apt-get install -y $p ; done < <(cat << "EOF"
    cmake
    extra-cmake-modules
    curl
    libcurl3-dev
    libjsoncpp-dev
    libcurl4-gnutls-dev
    libcurl4-openssl-dev
    libcurl4-nss-dev
    libwebsockets-dev
    libboost-all-dev
    libmysqlcppconn-dev
    libmysqlclient-dev
    libmysql++-dev
    mysql-server
    python3-dev
EOF
)

ln -s /usr/include/jsoncpp/json/ /usr/include/json

systemctl start mysql

pip3 install -r requirements.txt

# cpr (for C++ requests)
cd /tmp
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg 
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install cpr
pip3 install conan
ln -s ~/.local/bin/conan /usr/bin/conan

# Arg Parser for C++
cd /tmp
git clone https://github.com/jarro2783/cxxopts.git
cd cxxopts
cmake .
make -j8
make install

# Telegram bot library
cd /tmp/
git clone https://github.com/reo7sp/tgbot-cpp
cd tgbot-cpp
mkdir bin/ && cd bin/
cmake ..
make -j8
make install

# JSON C++
apt-get install nlohmann-json3-dev
git clone https://github.com/nlohmann/json.git
cd json
mkdir bin && cd bin
cmake ..
make -j4 
make install
