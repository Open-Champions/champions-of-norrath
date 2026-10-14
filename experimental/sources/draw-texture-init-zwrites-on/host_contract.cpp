#include <cstdio>
extern "C" unsigned long long zwritesOn;
int main() {
    if (zwritesOn != 0ull) return 1;
    std::puts("zwritesOn contract passed");
    return 0;
}
