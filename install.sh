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
json_link="usr/include/json"

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

# Arg Parser for C++
cd ./depends/cxxopts
cmake .
make -j8
make install

# Telegram bot library
cd ./depends/tgbot-cpp
cmake .
make -j8
make install

# JSON C++
cd ./depends/json
cmake .
make -j8 
make install

if [ ! -L $json_link]; then
  ln -s /usr/include/jsoncpp/json/ /usr/include/json
fi

systemctl start mysql

pip3 install -r requirements.txt