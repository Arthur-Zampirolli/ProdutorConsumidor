rm -rf build
rm main
mkdir -p build
cd build 
cmake .. -GNinja && ninja
chmod +x main
ls
mv main ..; cd ..
./main