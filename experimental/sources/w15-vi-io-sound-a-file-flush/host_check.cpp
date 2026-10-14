// Host check only; not a build input.
static int flushes;
extern "C" int BufferFlush__6VIFile(unsigned char*) { ++flushes; return 9; }
#include "flush.cpp"
int main() {
    alignas(8) unsigned char f[64] = {};
    *reinterpret_cast<int*>(f) = 3;
    if (Flush__6VIFile(f) != 0 || flushes != 1) return 1;
    *reinterpret_cast<int*>(f) = 0;
    if (Flush__6VIFile(f) != 0 || flushes != 2) return 2;
    *reinterpret_cast<int*>(f) = -1;
    if (Flush__6VIFile(f) != 0 || flushes != 2) return 3;
    return 0;
}
