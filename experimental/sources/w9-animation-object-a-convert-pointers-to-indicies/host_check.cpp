// Host contract for convertPointersToIndicies__Fi. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <string>
#include <vector>
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
extern "C" unsigned char player[23984 * 2] = {};
extern "C" unsigned char tempPlayerStruct[23984 * 2] = {};
static std::vector<std::string> events;
static std::vector<unsigned char*> packed;
extern "C" void playerPrintPaperDoll__Fi(int n) { events.push_back("paper " + std::to_string(n)); }
extern "C" void pack__4itemPQ24item11description(void* d) { events.push_back("pack"); packed.push_back((unsigned char*)d); }
#include "convert_pointers_to_indicies.cpp"
static int failures;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
static unsigned address_of(const unsigned char* p) { return norrath_host_address(p); }
int main() {
    const unsigned R = 23984;
    for (int p = 0; p < 2; ++p) {
        unsigned char* live = player + R * p;
        unsigned char* copy = tempPlayerStruct + R * p;
        for (unsigned i = 0; i < R; ++i) { live[i] = (unsigned char)(i * 7 + p); copy[i] = 0xee; }
        // Lists: counts 3, 0, 2.
        sw_(live + 4936, 3); sw_(live + 4936 + 4616, 0); sw_(live + 4936 + 2 * 4616, 2);
        const unsigned off[19] = {21896, 21900, 21904, 21908, 21912, 21916, 21920, 21924, 21928, 21932, 21936, 21940, 21944, 21948, 21952, 21956, 21960, 21964, 21968};
        for (int k = 0; k < 19; ++k) sw_(live + off[k], 0);
        sw_(live + 21896, address_of(live + 328));                           // list 0, element 0
        sw_(live + 21900, address_of(live + 328 + 72 * 2));                  // list 0, element 2
        sw_(live + 21904, address_of(live + 328 + 4616 + 72));               // list 1 is empty: not found
        sw_(live + 21908, address_of(live + 328 + 2 * 4616 + 72));           // list 2, element 1 -> 2001
        sw_(live + 21912, address_of(live + 328 + 72 * 3));                  // one past the list-0 count
        sw_(live + 21916, address_of(live + 329));                           // inside an element but not at its start
        sw_(live + 21956, address_of(live + 328 + 2 * 4616));                // list 2, element 0 -> 2000
        sw_(live + 21960, address_of(live + 328 + 72));                      // list 0, element 1 -> 1
        sw_(live + 21964, 0x1234);                                           // stray
        sw_(live + 21968, address_of(live + 328 + 2 * 4616 + 72));           // the single word after the loops
        // Words outside the nineteen stay as copied.
        unsigned char before[R];
        std::memcpy(before, live, R);
        events.clear(); packed.clear();
        convertPointersToIndicies__Fi(p);
        check(events[0] == "paper " + std::to_string(p), "paper doll first");
        check(same("live record untouched", live, before, R) == 0, "live untouched");
        unsigned char want[R];
        std::memcpy(want, before, R);
        const int expect[19] = {0, 2, -1, 2001, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2000, 1, -1, 2001};
        for (int k = 0; k < 19; ++k) sw_(want + off[k], (unsigned)expect[k]);
        check(same("copy", copy, want, R) == 0, "copy equals record with converted words");
        // pack: list 0 (3 elements), list 1 none, list 2 (2 elements), in order, on the copy.
        check(packed.size() == 5, "pack count");
        if (packed.size() == 5) {
            check(packed[0] == copy + 328 && packed[1] == copy + 328 + 72 && packed[2] == copy + 328 + 144, "list 0 packed in order");
            check(packed[3] == copy + 328 + 2 * 4616 && packed[4] == copy + 328 + 2 * 4616 + 72, "list 2 packed in order");
        }
        check(events.size() == 6, "calls");
    }
    // The other record is not touched by player 0.
    for (unsigned i = 0; i < R; ++i) tempPlayerStruct[R + i] = 0xcc;
    convertPointersToIndicies__Fi(0);
    for (unsigned i = 0; i < R; ++i) if (tempPlayerStruct[R + i] != 0xcc) { std::printf("fail: record 1 touched\n"); ++failures; break; }
    if (failures) return 1;
    std::printf("convertPointersToIndicies check passed\n");
    return 0;
}
