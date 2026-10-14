// Host check only; not a build input.
static int calls, lastOffset, lastOrigin;
static long long positionSeen;
extern "C" int MemorySegmCalcSegm__6VIFile(unsigned char* file, int offset, int origin) {
    ++calls; lastOffset = offset; lastOrigin = origin;
    positionSeen = *reinterpret_cast<long long*>(file + 16);
    return 0;
}
#include "memory_segm_seek.cpp"
int main() {
    alignas(8) unsigned char f[96] = {};
    long long* pos = reinterpret_cast<long long*>(f + 16);
    *reinterpret_cast<long long*>(f + 40) = 10;
    *reinterpret_cast<int*>(f + 8) = 1;
    if (MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(f, 4, 1) != 0 || *pos != 4 || calls != 1 || positionSeen != 4) return 1;
    if (lastOffset != 4 || lastOrigin != 1 || *reinterpret_cast<int*>(f + 8) != 0) return 2;
    if (MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(f, 3, 0) != 0 || *pos != 7 || calls != 2) return 3;
    if (MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(f, -2, 2) != 0 || *pos != 8 || calls != 3) return 4;
    *reinterpret_cast<int*>(f + 8) = 1;
    if (MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(f, 0, 2) != -1 || *pos != 10 || calls != 3 || *reinterpret_cast<int*>(f + 8) != 1) return 5;
    if (MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(f, -1, 1) != -1 || *pos != -1 || calls != 3) return 6;
    *pos = 5;
    if (MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(f, 1, 3) != -1 || *pos != 5 || calls != 3) return 7;
    return 0;
}
