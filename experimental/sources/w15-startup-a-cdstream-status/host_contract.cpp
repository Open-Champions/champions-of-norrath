#include "status.cpp"

#include <cstdio>

int norrath_cdstream_status;

int main() {
    const int values[] = {0, 1, 2, -1, 0x7fffffff, static_cast<int>(0x80000000u)};
    for (int v : values) {
        norrath_cdstream_status = v;
        if (VICdStreamStatus__Fv() != v) return 1;
    }
    std::puts("VICdStreamStatus contract passed");
    return 0;
}
