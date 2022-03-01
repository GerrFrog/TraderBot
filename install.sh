echo "Start installing dependencies"

# Prepare directory
mkdir bin/
mkdir data/
mkdir backtest_results/

# CMake
sudo apt-get install extra-cmake-modules 

# cpr (for C++ requests)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg 
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install cpr
pip3 install conan
ln -s ~/.local/bin/conan /usr/bin/conan

# curl
sudo apt-get install curl libcurl3 libcurl3-dev

# json 
sudo apt-get install libjsoncpp-dev -y
sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json

# libcurl-dev
sudo apt install libcurl4-gnutls-dev -y
sudo apt install libcurl4-openssl-dev -y
sudo apt install libcurl4-nss-dev -y

# web socket
sudo apt install libwebsockets-dev -y

# boost
sudo apt install libboost-all-dev -y

# MySQL connector
sudo apt-get install libmysqlcppconn-dev -y
sudo apt install libmysqlclient-dev -y
sudo apt install libmysql++-dev -y

# MySQL system
sudo apt install mysql-server -y
sudo systemctl start mysql

# Telegram bot library
cd /tmp/
git clone https://github.com/reo7sp/tgbot-cpp
cd tgbot-cpp
mkdir bin/ && cd bin/
cmake ..
make -j4
sudo make install
cd /tmp/

# Python for C++
sudo apt install python3-dev -y

echo "You should install mysql, if you haven't. Type 'sudo mysql_secure_installation'"

# Python packages 
pip3 install -r requirements.txt

# JSON C++
sudo apt-get install nlohmann-json3-dev
git clone https://github.com/nlohmann/json.git
cd json
mkdir bin && cd bin
cmake ..
make -j4 
make install
