#include <cstdio>
extern "C" unsigned engineEnvTexAddress;
int main() {
    if (engineEnvTexAddress != 0u) return 1;
    std::puts("engineEnvTex contract passed");
    return 0;
}
