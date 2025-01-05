mkdir -p Client/build
cd Client/build
cmake ..
make
# IP=$(hostname -I | awk '{print $1}')
# ./Client $IP 3000S DEBUG
./Client $(hostname -i) 3000S DEBUG