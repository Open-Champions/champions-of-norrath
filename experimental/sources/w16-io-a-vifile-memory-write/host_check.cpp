// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "memory_write.cpp"
int main() {
    alignas(8) unsigned char f[96] = {};
    unsigned char in[16];
    for (int i = 0; i < 16; ++i) in[i] = static_cast<unsigned char>(i + 1);
    *reinterpret_cast<unsigned*>(f + 48) = 0x100;
    *reinterpret_cast<long long*>(f + 32) = 10;
    *reinterpret_cast<long long*>(f + 40) = 6;
    *reinterpret_cast<int*>(f + 8) = 9;
    if (MemoryWrite__6VIFilePCvi(f, in, 4) != 0 || arena[0x100] != 1 || arena[0x103] != 4 || arena[0x104] != 0) return 1;
    if (*reinterpret_cast<long long*>(f + 16) != 4 || *reinterpret_cast<long long*>(f + 40) != 6 || *reinterpret_cast<int*>(f + 8) != 0) return 2;
    if (MemoryWrite__6VIFilePCvi(f, in, 4) != 0 || *reinterpret_cast<long long*>(f + 40) != 8) return 3;
    if (MemoryWrite__6VIFilePCvi(f, in, 4) != -1 || arena[0x109] != 2 || arena[0x10a] != 0) return 4;
    if (*reinterpret_cast<long long*>(f + 16) != 10 || *reinterpret_cast<long long*>(f + 40) != 10) return 5;
    if (MemoryWrite__6VIFilePCvi(f, in, 1) != -1 || *reinterpret_cast<long long*>(f + 16) != 10) return 6;
    return 0;
}
