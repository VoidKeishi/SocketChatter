cd Client/build
cmake ..
make
IP=$(hostname -I | awk '{print $1}')
./Client $IP 3000