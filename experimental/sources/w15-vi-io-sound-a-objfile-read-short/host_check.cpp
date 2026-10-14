// Host check only; not a build input.
#include <cstring>
static int reads, lastCount, swaps, lastSwap;
static unsigned char fill[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
extern "C" { int VIEndian; }
extern "C" int Read__6VIFilePvi(unsigned char*, void* buffer, int count) { ++reads; lastCount = count; std::memcpy(buffer, fill, count); return count; }
extern "C" void VIBufferSwap__FiPUc(int size, unsigned char* data) {
    ++swaps; lastSwap = size;
    for (int i = 0; i < size / 2; ++i) { unsigned char t = data[i]; data[i] = data[size - 1 - i]; data[size - 1 - i] = t; }
}
#include "read_short.cpp"
int main() {
    alignas(8) unsigned char f[600] = {};
    short v;
    std::memset(&v, 0xee, sizeof v);
    int* index = reinterpret_cast<int*>(f + 232);
    *index = 2;
    int* remaining = reinterpret_cast<int*>(f + 2 * 32 + 264);
    // not open for reading
    *remaining = 100;
    if (Read__9VIObjFileRs(f, &v) != -1 || reads != 0) return 1;
    *reinterpret_cast<int*>(f + 584) = 1;
    // too little left
    *remaining = 1;
    if (Read__9VIObjFileRs(f, &v) != -1 || reads != 0 || *remaining != 1) return 2;
    *remaining = -5;
    if (Read__9VIObjFileRs(f, &v) != -1 || reads != 0) return 3;
    // exact fit, host order (VIEndian == 1)
    *remaining = 2;
    VIEndian = 1;
    if (Read__9VIObjFileRs(f, &v) != 0 || reads != 1 || lastCount != 2 || swaps != 0 || *remaining != 0) return 4;
    if (std::memcmp(&v, fill, 2) != 0) return 5;
    // other order: swapped after the read
    *remaining = 8;
    VIEndian = 0;
    if (Read__9VIObjFileRs(f, &v) != 0 || reads != 2 || *remaining != 6) return 6;
    if (swaps != 1 || lastSwap != 2) return 7;
    unsigned char want[8]; for (int i = 0; i < 2; ++i) want[i] = fill[1 - i]; if (std::memcmp(&v, want, 2) != 0) return 8;
    // a different entry index changes which count is used
    *index = 0;
    *reinterpret_cast<int*>(f + 264) = 0;
    if (Read__9VIObjFileRs(f, &v) != -1 || reads != 2) return 9;
    return 0;
}
