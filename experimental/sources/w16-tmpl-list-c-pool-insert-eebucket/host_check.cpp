// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x2000];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "insert.cpp"
#include <cstdio>
#include <cstring>
static int addCalls, allocCalls, allocMode;   // allocMode: 0 fail, 1 succeed and set a free chain
extern "C" int Add__t6VIPool1Z10VIEEBucketRC10VIEEBucket(void* pool, const void* bucket) { ++addCalls; return bucket == arena + 0x400 ? 77 : -9; }
static unsigned* P = reinterpret_cast<unsigned*>(arena + 0x10);
static unsigned* rec(unsigned i) { return reinterpret_cast<unsigned*>(arena + 0x100) + 6 * i; }
extern "C" int Allocate__t6VIPool1Z10VIEEBucket(void* pool) {
    ++allocCalls;
    if (!allocMode) return -1;
    P[3] = 6;            // newly created free record
    rec(6)[4] = 0xffffffffu;
    return 0;
}
static void setup() {
    std::memset(arena, 0x5a, sizeof arena);
    P[0] = 2; P[1] = 4; P[2] = arena_base + 0x100; P[3] = 3; P[4] = 1;
    rec(1)[4] = 2; rec(1)[5] = 0xffffffffu;
    rec(2)[4] = 0xffffffffu; rec(2)[5] = 1;
    rec(3)[4] = 4;                                  // free chain 3 -> 4 -> end
    rec(4)[4] = 0xffffffffu;
    unsigned* b = reinterpret_cast<unsigned*>(arena + 0x400);
    for (int i = 0; i < 6; ++i) b[i] = 0xb0000000u + i;
}
int main() {
    bool ok = true;
    // insert before the head record (position 1)
    setup(); addCalls = allocCalls = 0;
    int r = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(P, arena + 0x400, 1);
    if (r != 3 || P[0] != 3 || P[3] != 4 || P[4] != 3 || addCalls || allocCalls) ok = false;
    if (rec(3)[0] != 0xb0000000u || rec(3)[1] != 0xb0000001u || rec(3)[2] != 0xb0000002u || rec(3)[3] != 0xb0000003u) ok = false;
    if (rec(3)[4] != 1 || rec(3)[5] != 0xffffffffu || rec(1)[5] != 3 || rec(2)[5] != 1 || rec(1)[4] != 2) ok = false;
    // insert before the last record (position 2)
    setup();
    r = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(P, arena + 0x400, 2);
    if (r != 3 || P[4] != 1 || rec(1)[4] != 3 || rec(3)[4] != 2 || rec(3)[5] != 1 || rec(2)[5] != 3) ok = false;
    // end marker: forwarded to Add
    setup(); addCalls = 0;
    r = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(P, arena + 0x400, 0x7fffffff);
    if (r != 77 || addCalls != 1 || P[0] != 2 || P[3] != 3) ok = false;
    // empty free chain: capacity doubles, Allocate called
    setup(); P[3] = 0xffffffffu; allocMode = 1; allocCalls = 0;
    r = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(P, arena + 0x400, 1);
    if (r != 6 || allocCalls != 1 || P[1] != 8 || P[0] != 3 || P[3] != 0xffffffffu || P[4] != 6) ok = false;
    setup(); P[3] = 0xffffffffu; P[1] = 0; allocMode = 1;
    Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(P, arena + 0x400, 1);
    if (P[1] != 1) ok = false;
    // allocation failure
    setup(); P[3] = 0xffffffffu; allocMode = 0;
    r = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(P, arena + 0x400, 1);
    if (r != -1 || P[0] != 2 || P[1] != 8) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
