// Host check only; not a build input.
static int which, lastCount, ret;
extern "C" int BufferedWrite__6VIFilePCvi(unsigned char*, const void*, int n) { which = 1; lastCount = n; return ret; }
extern "C" int MemoryWrite__6VIFilePCvi(unsigned char*, const void*, int n) { which = 2; lastCount = n; return ret; }
extern "C" int MemorySegmWrite__6VIFilePCvi(unsigned char*, const void*, int n) { which = 3; lastCount = n; return ret; }
#include "write.cpp"
int main() {
    alignas(8) unsigned char f[64] = {};
    char data[8];
    *reinterpret_cast<int*>(f) = 2;
    ret = 8;
    if (Write__6VIFilePCvi(f, data, 8) != 0 || which != 1 || lastCount != 8) return 1;
    ret = 5;
    if (Write__6VIFilePCvi(f, data, 8) != -1) return 2;
    *reinterpret_cast<int*>(f) = -1;
    *reinterpret_cast<int*>(f + 48) = 1;
    *reinterpret_cast<int*>(f + 56) = 1;
    ret = 42;
    which = 0;
    if (Write__6VIFilePCvi(f, data, 8) != 42 || which != 2) return 3;
    *reinterpret_cast<int*>(f + 48) = 0;
    ret = -7;
    if (Write__6VIFilePCvi(f, data, 8) != -7 || which != 3) return 4;
    *reinterpret_cast<int*>(f + 56) = 0;
    which = 0;
    if (Write__6VIFilePCvi(f, data, 8) != -1 || which != 0) return 5;
    return 0;
}
