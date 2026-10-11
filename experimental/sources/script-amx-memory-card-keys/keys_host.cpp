#include <cstdio>
#include <cstring>
extern "C" unsigned char norrath_mc_pre_start_key_bytes[];
extern "C" unsigned char norrath_mc_start_key_bytes[];
int main() {
    if (std::strcmp(reinterpret_cast<char*>(norrath_mc_pre_start_key_bytes), "preStart") != 0) return 1;
    if (std::strcmp(reinterpret_cast<char*>(norrath_mc_start_key_bytes), "start") != 0) return 2;
    if (norrath_mc_pre_start_key_bytes[8] != 0 || norrath_mc_start_key_bytes[5] != 0) return 3;
    std::puts("memory-card script keys passed");
}
