# JSON C++
sudo apt-get install nlohmann-json3-dev
git clone https://github.com/nlohmann/json.git
cd json
mkdir bin && cd bin
cmake ..
make -j4 
make install