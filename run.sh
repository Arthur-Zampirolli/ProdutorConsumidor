rm -rf build
rm -rf ./src/output/
rm ./src/main
mkdir -p build
mkdir -p ./src/output/
cd build 
cmake .. -GNinja && ninja
chmod +x main
ls
mv main ../src; cd ../src
./main