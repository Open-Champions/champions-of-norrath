#include "core_loading.cpp"

#include <cstdio>
#include <cstring>

int main() {
    unsigned char byte = 0xff;
    std::memcpy(&byte, &coreLoadingScreenRunning, 1);
    if (byte != 0) return 1;
    byte = 1;
    std::memcpy(&byte, &coreLoadingScreenRunning, 1);
    if (byte == 1) return 2;
    std::puts("core loading screen running contract passed");
    return 0;
}
