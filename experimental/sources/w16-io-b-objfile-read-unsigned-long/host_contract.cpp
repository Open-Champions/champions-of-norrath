#include "read_unsigned_long.cpp"

#include <cstdio>
#include <cstring>

int VIEndian;
static int reads, swaps, last_count, last_swap;
static const unsigned char fill[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
extern "C" int Read__6VIFilePvi(unsigned char*, void* buffer, int count) {
    ++reads;
    last_count = count;
    std::memcpy(buffer, fill, 8);
    return count;
}
extern "C" void VIBufferSwap__FiPUc(int size, unsigned char* data) {
    ++swaps;
    last_swap = size;
    for (int i = 0; i < size / 2; ++i) { unsigned char t = data[i]; data[i] = data[size - 1 - i]; data[size - 1 - i] = t; }
}

int main() {
    alignas(8) unsigned char f[640] = {};
    unsigned long long v = 0xeeeeeeeeeeeeeeeeull;
    *reinterpret_cast<int*>(f + 232) = 1;
    int* remaining = reinterpret_cast<int*>(f + 32 + 264);
    *remaining = 100;
    VIEndian = 0;
    if (Read__9VIObjFileRUl(f, &v) != -1 || reads != 0) return 1;
    *reinterpret_cast<int*>(f + 584) = 1;
    *remaining = 7;
    if (Read__9VIObjFileRUl(f, &v) != -1 || reads != 0 || *remaining != 7) return 2;
    *remaining = 8;
    if (Read__9VIObjFileRUl(f, &v) != 0 || reads != 1 || last_count != 8 || swaps != 1 || last_swap != 8) return 3;
    if (v != 0x1122334455667788ull || *remaining != 0) return 4;
    VIEndian = 1;
    v = 0;
    *remaining = 20;
    if (Read__9VIObjFileRUl(f, &v) != 0 || swaps != 1 || *remaining != 12) return 5;
    if (std::memcmp(&v, fill, 8) != 0) return 6;
    std::puts("objfile read-unsigned-long passed");
    return 0;
}
