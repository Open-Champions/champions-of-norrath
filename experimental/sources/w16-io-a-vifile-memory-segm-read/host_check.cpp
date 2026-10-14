// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "memory_segm_read.cpp"
static void segment(int i, long long length, unsigned base) {
    *reinterpret_cast<long long*>(arena + 0x100 + 24 * i + 8) = length;
    *reinterpret_cast<unsigned*>(arena + 0x100 + 24 * i + 16) = base;
    for (int k = 0; k < length; ++k) arena[base + k] = static_cast<unsigned char>(i * 16 + k + 1);
}
int main() {
    alignas(8) unsigned char f[96] = {};
    unsigned char out[16];
    segment(0, 3, 0x1000);
    segment(1, 2, 0x2000);
    segment(2, 4, 0x3000);
    *reinterpret_cast<int*>(f + 52) = 2;
    *reinterpret_cast<unsigned*>(f + 56) = 0x100;
    *reinterpret_cast<long long*>(f + 40) = 9;
    *reinterpret_cast<int*>(f + 8) = 7;
    for (auto& b : out) b = 0xee;
    if (MemorySegmRead__6VIFilePvi(f, out, 4) != 0 || out[0] != 1 || out[2] != 3 || out[3] != 17 || out[4] != 0xee) return 1;
    if (*reinterpret_cast<long long*>(f + 16) != 4 || *reinterpret_cast<int*>(f + 60) != 1 || *reinterpret_cast<long long*>(f + 64) != 1) return 2;
    if (*reinterpret_cast<int*>(f + 8) != 0) return 3;
    if (MemorySegmRead__6VIFilePvi(f, out, 10) != -1 || out[0] != 18 || out[1] != 33 || out[4] != 36 || out[5] != 0xee) return 4;
    if (*reinterpret_cast<long long*>(f + 16) != 9 || *reinterpret_cast<int*>(f + 8) != 1 || *reinterpret_cast<int*>(f + 60) != 2) return 5;
    if (MemorySegmRead__6VIFilePvi(f, out, 1) != -1 || *reinterpret_cast<long long*>(f + 16) != 9) return 6;
    return 0;
}
