#include "async_state.cpp"

#include <cstdio>

int main() {
    if (sizeof sAsyncMode != 4 || sizeof sAsyncFd != 4 || sizeof sProductId != 32) return 1;
    if (sAsyncMode != 0 || sAsyncFd != 0) return 2;
    const char expected[] = "BASLUS-20470EQONLINE";
    for (unsigned i = 0; i < sizeof expected; ++i) if (sProductId[i] != expected[i]) return 3;
    for (unsigned i = sizeof expected; i < sizeof sProductId; ++i) if (sProductId[i] != 0) return 4;
    if (static_cast<void*>(&sAsyncMode) == static_cast<void*>(&sAsyncFd)) return 5;
    std::puts("memcard async state contract passed");
    return 0;
}
