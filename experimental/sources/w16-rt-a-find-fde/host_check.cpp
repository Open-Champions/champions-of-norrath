// Host check (not a plan input). Build:
//   c++ -O1 -std=c++17 -c -DW16_HOST_CHECK -Dfind_fde=w16_find_fde -o /tmp/w16_ff_src.o find_fde.cpp
//   c++ -O1 -std=c++17 -o /tmp/w16_ff host_check.cpp /tmp/w16_ff_src.o && /tmp/w16_ff
// Address words are offsets into a host arena. The stand-ins count semaphore calls and record
// what search_fdes was asked.
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

unsigned char* w16_host_arena;
int __sce_eh_sema_id = 77;
unsigned objects;
extern "C" unsigned w16_find_fde(unsigned pc);

static int waits, signals, frame_inits;
static std::vector<std::pair<unsigned, unsigned>> searches;
static std::map<unsigned, unsigned> search_result;
static unsigned init_array;  // what frame_init stores into word 12 when called
static unsigned init_count;
static unsigned init_first, init_end;

static unsigned& word(unsigned offset) { return *reinterpret_cast<unsigned*>(w16_host_arena + offset); }

extern "C" int WaitSema(int id) { if (id != 77) std::printf("bad id\n"); ++waits; return 0; }
extern "C" int SignalSema(int id) { if (id != 77) std::printf("bad id\n"); ++signals; return 0; }
extern "C" void frame_init(void* record) {
    ++frame_inits;
    unsigned* r = static_cast<unsigned*>(record);
    if (r[0] == 0) { r[0] = init_first; r[1] = init_end; }
    r[3] = init_array;
    r[4] = init_count;
}
extern "C" unsigned search_fdes(unsigned entries, unsigned pc) {
    searches.push_back({entries, pc});
    auto it = search_result.find(entries);
    return it == search_result.end() ? 0 : it->second;
}

static void reset() {
    waits = signals = frame_inits = 0;
    searches.clear();
    search_result.clear();
    init_array = init_count = init_first = init_end = 0;
    objects = 0;
    std::memset(w16_host_arena, 0, 1 << 20);
}
static unsigned make_record(unsigned at, unsigned first, unsigned end, unsigned entries, unsigned array, unsigned count, unsigned next) {
    word(at) = first; word(at + 4) = end; word(at + 8) = entries; word(at + 12) = array; word(at + 16) = count; word(at + 20) = next;
    return at;
}

int main() {
    w16_host_arena = new unsigned char[1 << 20];
    unsigned cases = 0;
    // 1. empty list.
    reset();
    if (w16_find_fde(0x1000) != 0 || waits != 1 || signals != 1) return 1;
    ++cases;
    // 2. second record applies; both end addresses are exclusive; the list is the unsorted-array case.
    reset();
    objects = make_record(0x100, 0x1000, 0x2000, 0x900, 0, 0, 0x200);
    make_record(0x200, 0x3000, 0x4000, 0xa00, 0, 0, 0);
    search_result[0xa00] = 0x555;
    if (w16_find_fde(0x3000) != 0x555 || searches.size() != 1 || searches[0].first != 0xa00 || frame_inits != 1) return 2;
    if (w16_find_fde(0x4000) != 0 || waits != signals || waits != 2) return 3;
    ++cases;
    // 3. a record with first address 0 is set up before the range test.
    reset();
    init_first = 0x5000; init_end = 0x6000;
    objects = make_record(0x100, 0, 0, 0x900, 0, 0, 0);
    search_result[0x900] = 0x777;
    if (w16_find_fde(0x5500) != 0x777 || frame_inits < 1) return 4;
    ++cases;
    // 4. array equal to the entry list: zero-terminated list of lists, first nonzero answer wins.
    reset();
    objects = make_record(0x100, 0x1000, 0x2000, 0x800, 0x800, 0, 0);
    word(0x800) = 0x900; word(0x804) = 0xa00; word(0x808) = 0xb00; word(0x80c) = 0;
    init_array = 0x800;
    search_result[0xa00] = 0x888;
    search_result[0xb00] = 0x999;
    if (w16_find_fde(0x1500) != 0x888 || searches.size() != 2 || searches[1].first != 0xa00) return 5;
    search_result.clear(); searches.clear();
    if (w16_find_fde(0x1500) != 0 || searches.size() != 3 || waits != signals) return 6;
    ++cases;
    // 5. sorted array: binary search without the semaphore held; every position found, gaps give 0.
    for (unsigned n : {0u, 1u, 2u, 5u, 8u}) {
        reset();
        objects = make_record(0x100, 0x10000, 0x20000, 0x900, 0x2000, n, 0);
        for (unsigned i = 0; i < n; ++i) {
            const unsigned fde = 0x3000 + 0x20 * i;
            word(0x2000 + 4 * i) = fde;
            word(fde + 8) = 0x10000 + 0x1000 * i;
            word(fde + 12) = 0x800;
        }
        for (unsigned pc = 0x10000 - 1; pc < 0x10000 + 0x1000 * n + 0x1000; pc += 0x7f) {
            const unsigned got = w16_find_fde(pc);
            unsigned expected = 0;
            for (unsigned i = 0; i < n; ++i) {
                const unsigned start = 0x10000 + 0x1000 * i;
                if (pc >= start && pc < start + 0x800) expected = 0x3000 + 0x20 * i;
            }
            if (pc < 0x10000 || pc >= 0x20000) expected = 0;  // outside the record range
            if (got != expected) { std::printf("sorted n=%u pc=%x got %x expected %x\n", n, pc, got, expected); return 7; }
            ++cases;
        }
        if (waits != signals) return 8;
        if (!searches.empty()) return 9;
    }
    // 6. frame_init supplies the array.
    reset();
    objects = make_record(0x100, 0x10000, 0x20000, 0x900, 0, 0, 0);
    init_array = 0x2000; init_count = 1;
    word(0x2000) = 0x3000; word(0x3008) = 0x10000; word(0x300c) = 0x100;
    if (w16_find_fde(0x10010) != 0x3000 || frame_inits != 1) return 10;
    ++cases;
    std::printf("%u find_fde cases passed\n", cases);
    return 0;
}
