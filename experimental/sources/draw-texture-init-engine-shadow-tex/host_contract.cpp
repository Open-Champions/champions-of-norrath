#include <cstdio>
extern "C" int engineShadowTexAddress;
int main() {
    if (engineShadowTexAddress != 0) return 1;
    std::puts("engineShadowTex contract passed");
    return 0;
}
