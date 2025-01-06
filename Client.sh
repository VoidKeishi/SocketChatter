mkdir -p Client/build
cd Client/build
cmake ..
make
./Client 192.168.103.242 3000 DEBUG