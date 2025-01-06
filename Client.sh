mkdir -p Client/build
cd Client/build
cmake ..
make
./Client $(hostname -i) 3000 DEBUG