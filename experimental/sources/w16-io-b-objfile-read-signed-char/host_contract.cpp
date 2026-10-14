#include "read_signed_char.cpp"

#include <cstdio>
#include <cstring>

static int reads, last_count;
extern "C" int Read__6VIFilePvi(unsigned char*, void* buffer, int count) {
    ++reads;
    last_count = count;
    *static_cast<unsigned char*>(buffer) = 0xf3;
    return count;
}

int main() {
    alignas(8) unsigned char f[640] = {};
    signed char v = 0x11;
    *reinterpret_cast<int*>(f + 232) = 2;
    int* remaining = reinterpret_cast<int*>(f + 2 * 32 + 264);
    *remaining = 5;
    if (Read__9VIObjFileRSc(f, &v) != -1 || reads != 0 || v != 0x11) return 1;
    *reinterpret_cast<int*>(f + 584) = 1;
    *remaining = 0;
    if (Read__9VIObjFileRSc(f, &v) != -1 || reads != 0) return 2;
    *remaining = -3;
    if (Read__9VIObjFileRSc(f, &v) != -1 || reads != 0 || *remaining != -3) return 3;
    *remaining = 1;
    if (Read__9VIObjFileRSc(f, &v) != 0 || reads != 1 || last_count != 1 || v != -13 || *remaining != 0) return 4;
    *remaining = 9;
    if (Read__9VIObjFileRSc(f, &v) != 0 || *remaining != 8) return 5;
    *reinterpret_cast<int*>(f + 232) = 0;
    if (Read__9VIObjFileRSc(f, &v) != -1) return 6;
    std::puts("objfile read-signed-char passed");
    return 0;
}
