// Host check only; not a build input.
#include "end.cpp"
#include <cstdio>
int main() {
    alignas(8) unsigned char pool[64] = {0};
    bool ok = End__Ct6VIPool1ZP12VIPrimBuffer(pool) == 0x7fffffff && End__Ct6VIPool1ZP12VIPrimBuffer(0) == 0x7fffffff;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
