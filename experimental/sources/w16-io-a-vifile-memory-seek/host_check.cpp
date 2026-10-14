// Host check only; not a build input.
#include "memory_seek.cpp"
int main() {
    alignas(8) unsigned char f[96] = {};
    long long* pos = reinterpret_cast<long long*>(f + 16);
    *reinterpret_cast<long long*>(f + 40) = 10;
    *reinterpret_cast<int*>(f + 8) = 1;
    if (MemorySeek__6VIFilelQ26VIFile10SeekOrigin(f, 4, 1) != 0 || *pos != 4 || *reinterpret_cast<int*>(f + 8) != 0) return 1;
    *reinterpret_cast<int*>(f + 8) = 1;
    if (MemorySeek__6VIFilelQ26VIFile10SeekOrigin(f, 3, 0) != 0 || *pos != 7 || *reinterpret_cast<int*>(f + 8) != 0) return 2;
    if (MemorySeek__6VIFilelQ26VIFile10SeekOrigin(f, -2, 2) != 0 || *pos != 8) return 3;
    *reinterpret_cast<int*>(f + 8) = 1;
    if (MemorySeek__6VIFilelQ26VIFile10SeekOrigin(f, 0, 2) != -1 || *pos != 10 || *reinterpret_cast<int*>(f + 8) != 1) return 4;
    if (MemorySeek__6VIFilelQ26VIFile10SeekOrigin(f, -1, 1) != -1 || *pos != -1) return 5;
    *pos = 5;
    if (MemorySeek__6VIFilelQ26VIFile10SeekOrigin(f, 1, 3) != -1 || *pos != 5) return 6;
    return 0;
}
