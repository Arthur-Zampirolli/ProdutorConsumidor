rm -rf build
mkdir -p build
cd build 
cmake .. -GNinja && ninja
./main 10000 4