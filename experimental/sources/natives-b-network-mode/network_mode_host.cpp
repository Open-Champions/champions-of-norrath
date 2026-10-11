#include <cstdio>
extern "C" int amxNetworkMode__FP5__amxPi(void*, int*);
extern "C" signed char netNetworkMode = 0;

int main() {
    netNetworkMode = 0;
    if (amxNetworkMode__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    netNetworkMode = 1;
    if (amxNetworkMode__FP5__amxPi(nullptr, nullptr) != 1) return 2;
    netNetworkMode = static_cast<signed char>(0xFF);
    if (amxNetworkMode__FP5__amxPi(nullptr, nullptr) != -1) return 3;
    netNetworkMode = 127;
    if (amxNetworkMode__FP5__amxPi(nullptr, nullptr) != 127) return 4;
    std::puts("amxNetworkMode passed");
    return 0;
}
