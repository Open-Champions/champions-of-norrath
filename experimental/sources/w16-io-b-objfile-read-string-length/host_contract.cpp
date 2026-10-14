#include "read_string_length.cpp"

#include <cstdio>
#include <cstring>

int VIEndian;
static int reads, swaps, last_count, last_swap;
extern "C" int Read__6VIFilePvi(unsigned char*, void* buffer, int count) {
    ++reads;
    last_count = count;
    const unsigned char bytes[2] = {0x12, 0x34};
    std::memcpy(buffer, bytes, 2);
    return count;
}
extern "C" void VIBufferSwap__FiPUc(int size, unsigned char* data) {
    ++swaps;
    last_swap = size;
    unsigned char t = data[0]; data[0] = data[1]; data[1] = t;
}

int main() {
    alignas(8) unsigned char f[640] = {};
    unsigned short v = 0xeeee;
    *reinterpret_cast<int*>(f + 232) = 3;
    int* remaining = reinterpret_cast<int*>(f + 3 * 32 + 264);
    *remaining = 50;
    if (ReadStringLength__9VIObjFileRUs(f, &v) != -1 || reads != 0 || v != 0xeeee) return 1;
    *reinterpret_cast<int*>(f + 584) = 1;
    *remaining = 1;
    if (ReadStringLength__9VIObjFileRUs(f, &v) != -1 || reads != 0 || *remaining != 1) return 2;
    VIEndian = 0;
    *remaining = 2;
    if (ReadStringLength__9VIObjFileRUs(f, &v) != 0 || reads != 1 || last_count != 2 || swaps != 1 || last_swap != 2) return 3;
    if (std::memcmp(&v, "\x34\x12", 2) != 0 || *remaining != 0) return 4;
    VIEndian = 1;
    *remaining = 9;
    if (ReadStringLength__9VIObjFileRUs(f, &v) != 0 || swaps != 1 || *remaining != 7) return 5;
    if (std::memcmp(&v, "\x12\x34", 2) != 0) return 6;
    std::puts("objfile read-string-length passed");
    return 0;
}
