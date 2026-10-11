#include <cstdio>
extern "C" unsigned char dispEnv[48];
int main() {
    if (sizeof dispEnv != 48u) return 1;
    for (unsigned index = 0; index < sizeof dispEnv; ++index)
        if (dispEnv[index] != 0) return 2;
    std::puts("dispEnv contract passed");
    return 0;
}
