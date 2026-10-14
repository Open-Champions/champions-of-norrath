#include <cstdio>
extern "C" unsigned long long zwritesOff;
int main() {
    if (zwritesOff != 0ull) return 1;
    std::puts("zwritesOff contract passed");
    return 0;
}
