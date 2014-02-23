rm ./release/*
ls ./release
echo '------------------'
g++ -std=c++0x -static -O2 ./src/Source.cpp -o ./release/p2p_vod.exe
echo '------------------'
ls ./release
