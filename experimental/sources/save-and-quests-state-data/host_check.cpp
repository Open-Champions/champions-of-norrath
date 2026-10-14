#include <cstdio>
#include "state_data.cpp"
int main() {
    if (sizeof stateData != 32) return 1;
    for (unsigned char b : stateData) if (b != 0) return 2;
    stateData[31] = 1;  // the whole extent is writable
    std::puts("ok");
    return 0;
}
