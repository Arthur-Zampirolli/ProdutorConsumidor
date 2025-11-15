rm -rf build
rm ./src/main
mkdir -p build
cd build 
cmake .. -GNinja && ninja
chmod +x main
ls
mv main ../src; cd ../src
./main