// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "calc_segm.cpp"
static void segment(int i, long long start, long long length) {
    *reinterpret_cast<long long*>(arena + 0x1000 + 24 * i) = start;
    *reinterpret_cast<long long*>(arena + 0x1000 + 24 * i + 8) = length;
}
int main() {
    unsigned char* f = arena;
    *reinterpret_cast<int*>(f + 52) = 3;
    *reinterpret_cast<unsigned*>(f + 56) = 0x1000;
    segment(0, 0, 10);
    segment(1, 10, 20);
    segment(2, 40, 8);
    long long cases[][3] = {{0, 0, 0}, {9, 0, 9}, {10, 1, 0}, {29, 1, 19}, {40, 2, 0}, {47, 2, 7}, {35, 2, -5}, {60, 2, 20}};
    for (auto& c : cases) {
        *reinterpret_cast<long long*>(f + 16) = c[0];
        if (MemorySegmCalcSegm__6VIFile(f, 0, 99) != 0) return 1;
        if (*reinterpret_cast<int*>(f + 60) != c[1] || *reinterpret_cast<long long*>(f + 64) != c[2]) return 2 + static_cast<int>(c[0]);
    }
    // empty table: the index is the caller's third argument
    *reinterpret_cast<int*>(f + 52) = 0;
    *reinterpret_cast<long long*>(f + 16) = 15;
    MemorySegmCalcSegm__6VIFile(f, 0, 1);
    if (*reinterpret_cast<int*>(f + 60) != 1 || *reinterpret_cast<long long*>(f + 64) != 5) return 100;
    return 0;
}
