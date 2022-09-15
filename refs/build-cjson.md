## install cjson from source

```sh
cd ~
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build

# make
cmake ..
make
sudo make install

# customize PREFIX for debian/ubuntu
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make PREFIX=/usr install
```
