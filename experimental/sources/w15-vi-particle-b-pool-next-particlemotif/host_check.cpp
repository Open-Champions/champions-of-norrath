// Host check only; not a build input.
#include "next.cpp"
#include <cstdio>
#include <cstring>
int main() {
    alignas(8) unsigned char pool[32] = {0};
    int entries[5 * 3];
    for (int i = 0; i < 5; ++i) { entries[3 * i] = 100 + i; entries[3 * i + 1] = i + 1; entries[3 * i + 2] = i - 1; }
    entries[3 * 4 + 1] = -1;
    entries[3 * 2 + 1] = 0;
    const unsigned char* base = reinterpret_cast<const unsigned char*>(entries);
    std::memcpy(pool + 8, &base, sizeof base);
    const int want[] = {1, 2, 0, 4, 0x7fffffff};
    for (int i = 0; i < 5; ++i)
        if (Next__Ct6VIPool1ZP15VIParticleMotifi(pool, i) != want[i]) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
