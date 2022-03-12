echo "Start installing dependencies"

# Prepare directory
mkdir bin/
mkdir data/
mkdir backtest_results/

# CMake
apt-get install extra-cmake-modules 

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

# curl
apt-get install curl libcurl3 libcurl3-dev

# json 
apt-get install libjsoncpp-dev -y
ln -s /usr/include/jsoncpp/json/ /usr/include/json

# libcurl-dev
apt install libcurl4-gnutls-dev -y
apt install libcurl4-openssl-dev -y
apt install libcurl4-nss-dev -y

# web socket
apt install libwebsockets-dev -y

# boost
apt install libboost-all-dev -y

# MySQL connector
apt-get install libmysqlcppconn-dev -y
apt install libmysqlclient-dev -y
apt install libmysql++-dev -y

# MySQL system
apt install mysql-server -y
systemctl start mysql

# Telegram bot library
cd /tmp/
git clone https://github.com/reo7sp/tgbot-cpp
cd tgbot-cpp
mkdir bin/ && cd bin/
cmake ..
make -j4
make install
cd /tmp/

# Python for C++
apt install python3-dev -y

echo "You should install mysql, if you haven't. Type 'mysql_secure_installation'"

# Python packages 
pip3 install -r requirements.txt

# JSON C++
apt-get install nlohmann-json3-dev
git clone https://github.com/nlohmann/json.git
cd json
mkdir bin && cd bin
cmake ..
make -j4 
make install
