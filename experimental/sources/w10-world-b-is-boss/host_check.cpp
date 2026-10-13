// Host contract for isBoss__FP10GameObject. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
// A host pointer is 8 bytes: the table pointer at +72 and the function pointer at table + 4
// are host pointers (the unit reads nothing else near them).
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int ids[12];
static int *current_type;
static int virtual_calls, eq_calls, throws;
static int order[40], order_n;
extern "C" void __throw_bad_typeid() { ++throws; }
extern "C" int __eq__C9type_infoRC9type_info(void* l, void* r) { ++eq_calls; order[order_n++] = int(static_cast<int*>(r) - ids); return l == r; }
extern "C" void* __tf8AntQueen() { return &ids[1]; }
extern "C" void* __tf10Arenabeast() { return &ids[2]; }
extern "C" void* __tf10CloudGiant() { return &ids[3]; }
extern "C" void* __tf8Innoruuk() { return &ids[4]; }
extern "C" void* __tf6Minion() { return &ids[5]; }
extern "C" void* __tf9MummyKing() { return &ids[6]; }
extern "C" void* __tf11SpiderQueen() { return &ids[7]; }
extern "C" void* __tf12UndeadKnight() { return &ids[8]; }
extern "C" void* __tf11VampireLord() { return &ids[9]; }
static void* type_fn() { ++virtual_calls; return current_type; }
#include "is_boss.cpp"
static void make(unsigned char* obj, unsigned char* table, unsigned flags, unsigned long long bits, int w976) {
    std::memset(obj, 0, 1100); std::memset(table, 0, 64);
    void* (*fn)() = type_fn;
    std::memcpy(table + 4, &fn, sizeof fn);
    std::memcpy(obj + 72, &table, sizeof table);
    std::memcpy(obj + 12, &flags, 4);
    std::memcpy(obj + 160, &bits, 8);
    std::memcpy(obj + 976, &w976, 4);
}
static int run(unsigned char* obj) { virtual_calls = eq_calls = throws = order_n = 0; return isBoss__FP10GameObject(obj); }
int main() {
    unsigned char obj[1100], table[64];
    int other = 0;
    // Flag 0x20 clear: 0 with nothing called, even for a boss type or a set low bit.
    make(obj, table, 0x1f, 1, 1); current_type = &ids[1];
    if (run(obj) != 0 || virtual_calls != 0 || eq_calls != 0) return 1;
    // Low bit of the 64-bit word at +160: 1 with no type tests. Higher bits alone do nothing.
    make(obj, table, 0x20, 1, 0); current_type = &other;
    if (run(obj) != 1 || virtual_calls != 0) return 2;
    make(obj, table, 0x20, 0xfffffffffffffffeull, 0); current_type = &other;
    if (run(obj) != 0 || eq_calls != 9) return 3;
    make(obj, table, 0x20, 0x100000000ull, 0);                    // bit 32 only
    if (run(obj) != 0) return 4;
    // Each boss type answers 1, stopping at its own comparison; CloudGiant needs +976 == 1.
    const int bosses[] = {1, 2, 4, 5, 6, 7, 8, 9};
    for (int id : bosses) {
        make(obj, table, 0xffffffffu, 0, 0); current_type = &ids[id];
        if (run(obj) != 1) return 10 + id;
        if (virtual_calls != eq_calls) return 20 + id;           // type read again before each test
    }
    // The order of comparisons: AntQueen, Arenabeast, CloudGiant, Innoruuk, Minion,
    // MummyKing, SpiderQueen, UndeadKnight, VampireLord.
    make(obj, table, 0x20, 0, 0); current_type = &other;
    run(obj);
    const int want[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    if (order_n != 9) return 30;
    for (int i = 0; i != 9; ++i) if (order[i] != want[i]) return 31;
    make(obj, table, 0x20, 0, 1); current_type = &ids[3];
    if (run(obj) != 1) return 32;
    for (int w : {0, 2, -1, 0x100, 0x10001}) {
        make(obj, table, 0x20, 0, w); current_type = &ids[3];
        if (run(obj) != 0) return 33;                             // no later type matches CloudGiant
        if (eq_calls != 9) return 34;                             // the checks went on to the end
    }
    // An unlisted type answers 0.
    make(obj, table, 0x20, 0, 1); current_type = &other;
    if (run(obj) != 0) return 35;
    std::puts("isBoss check passed");
    return 0;
}
