// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "objfile-close.cpp"
#include <cstring>

static unsigned char* file;
static int order[16], n, writes_left;
static int w(int off) { int v; std::memcpy(&v, file + off, 4); return v; }
static void put(int off, int v) { std::memcpy(file + off, &v, 4); }
extern "C" int WriteEnd__9VIObjFile(void* o) {
    order[n++] = 1;
    if (--writes_left == 0) put(232, -1);
    return 0;
}
extern "C" int Flush__9VIObjFile(void* o) { order[n++] = 2; return 0; }
extern "C" int Close__6VIFile(void* o) { order[n++] = 3; return 0; }

int main() {
    alignas(8) unsigned char buf[640];
    file = buf;
    std::memset(buf, 0x11, sizeof buf);
    put(560, 0);
    unsigned char before[640];
    std::memcpy(before, buf, sizeof buf);
    n = 0;
    if (Close__9VIObjFile(buf) != -1 || n != 0 || std::memcmp(before, buf, sizeof buf) != 0) return 1;
    put(560, 5); put(572, 0); put(232, 4);
    n = 0;
    if (Close__9VIObjFile(buf) != 0 || n != 2 || order[0] != 2 || order[1] != 3) return 2;
    for (int off = 560; off <= 588; off += 4) if (w(off) != 0) return 3;
    if (w(232) != -1) return 4;
    put(560, 5); put(572, 9); put(232, 4); writes_left = 3;
    n = 0;
    if (Close__9VIObjFile(buf) != 0 || n != 5 || order[0] != 1 || order[2] != 1 || order[3] != 2 || order[4] != 3) return 5;
    put(560, 5); put(572, 9); put(232, -1);
    n = 0;
    if (Close__9VIObjFile(buf) != 0 || n != 2) return 6;
    return 0;
}
