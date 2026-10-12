#include "cache_cd_done.cpp"

#include <cstdio>

int main() {
    if (sizeof cacheCdDone != 1 || cacheCdDone != 0) return 1;
    std::puts("cache cd done passed");
    return 0;
}
