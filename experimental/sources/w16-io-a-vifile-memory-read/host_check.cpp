// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "memory_read.cpp"
int main() {
    alignas(8) unsigned char f[96] = {};
    unsigned char out[16];
    for (int i = 0; i < 16; ++i) arena[0x100 + i] = static_cast<unsigned char>(i + 1);
    *reinterpret_cast<unsigned*>(f + 48) = 0x100;
    *reinterpret_cast<long long*>(f + 40) = 10;
    *reinterpret_cast<int*>(f + 8) = 9;
    for (auto& b : out) b = 0xee;
    if (MemoryRead__6VIFilePvi(f, out, 4) != 0 || out[0] != 1 || out[3] != 4 || out[4] != 0xee) return 1;
    if (*reinterpret_cast<long long*>(f + 16) != 4 || *reinterpret_cast<int*>(f + 8) != 0) return 2;
    if (MemoryRead__6VIFilePvi(f, out, 16) != -1 || out[5] != 10 || out[6] != 0xee) return 3;
    if (*reinterpret_cast<long long*>(f + 16) != 10 || *reinterpret_cast<int*>(f + 8) != 1) return 4;
    if (MemoryRead__6VIFilePvi(f, out, 2) != -1 || *reinterpret_cast<long long*>(f + 16) != 10) return 5;
    if (MemoryRead__6VIFilePvi(f, out, 0) != 0 || *reinterpret_cast<int*>(f + 8) != 0) return 6;
    return 0;
}
