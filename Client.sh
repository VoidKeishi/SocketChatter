mkdir -p Client/build
cd Client/build
cmake ..
make
./Client 127.0.0.1 3000 DEBUG