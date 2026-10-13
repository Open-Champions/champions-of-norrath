// Host contract for objectIterateRunLists__FPFP10GameObject_v. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <vector>
#include <string>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "iterate_run_lists.cpp"
int objectIgnoreFlags;
unsigned char runList[8];
unsigned char mediumRunList[64];
unsigned char slowRunList[240];
// Stand-in iterator: each head owns a list of arena nodes (set by the test).
static std::vector<std::string> events;
static std::vector<unsigned> chains[1 + 15 + 60];
static int active_chain, position;
static unsigned char* heads_base[3] = {runList, mediumRunList, slowRunList};
static int chain_of(unsigned char* head) {
    if (head == runList) return 0;
    if (head >= mediumRunList && head < mediumRunList + 64) return 1 + int(head - mediumRunList) / 4;
    return 16 + int(head - slowRunList) / 4;
}
static void show(unsigned* it) {
    sw_(reinterpret_cast<unsigned char*>(it) + 4, position < int(chains[active_chain].size()) ? chains[active_chain][position] : 0);
}
extern "C" void __16ListSafeIteratorP8ListHeadi(unsigned* it, unsigned char* head, int mask) {
    active_chain = chain_of(head); position = 0;
    events.push_back("C" + std::to_string(active_chain) + "m" + std::to_string(mask));
    show(it);
}
extern "C" void next__16ListSafeIterator(unsigned* it) { ++position; events.push_back("N"); show(it); }
extern "C" void _$_16ListSafeIterator(unsigned*, int flags) { events.push_back("D" + std::to_string(flags)); }
static void callback(unsigned char* object) {
    events.push_back("F" + std::to_string(norrath_host_address(object)));
    objectIgnoreFlags += 1;   // proves the mask word is re-read for each head
}
int main() {
    unsigned char* node = arena_at(0x400);
    // Nodes: two on runList, one on medium head 3, one on medium head 14, one on slow head 0 and two on slow head 59.
    auto make = [&](int chain, int slot, unsigned object) {
        unsigned char* n = node + slot * 32;
        sw_(n + 12, object);
        chains[chain].push_back(norrath_host_address(n));
    };
    make(0, 0, 0x1000); make(0, 1, 0x1001);
    make(1 + 3, 2, 0x2000); make(1 + 14, 3, 0x3000);
    make(16 + 0, 4, 0x4000); make(16 + 59, 5, 0x5000); make(16 + 59, 6, 0x5001);
    objectIgnoreFlags = 7;
    // Object words are read through load_pointer, so give each node a real arena object.
    for (int c = 0; c != 76; ++c)
        for (unsigned a : chains[c]) {
            unsigned char* n = norrath_host_pointer(a);
            sw_(n + 12, norrath_host_address(arena_at(0x2000 + (n - node) * 4)));
        }
    objectIterateRunLists__FPFP10GameObject_v(reinterpret_cast<void (*)(unsigned char*)>(+callback));
    // Build the expected event string independently.
    std::vector<std::string> want;
    int mask = 7;
    auto expect_chain = [&](int c) {
        want.push_back("C" + std::to_string(c) + "m" + std::to_string(mask));
        for (size_t i = 0; i != chains[c].size(); ++i) {
            unsigned char* n = norrath_host_pointer(chains[c][i]);
            want.push_back("F" + std::to_string(hw(n + 12)));
            ++mask;
            want.push_back("N");
        }
        want.push_back("D2");
    };
    for (int c = 0; c != 76; ++c) expect_chain(c);
    if (events != want) {
        std::printf("event sequence differs: got %zu events want %zu\n", events.size(), want.size());
        for (size_t i = 0; i < events.size() && i < want.size(); ++i)
            if (events[i] != want[i]) { std::printf("first difference at %zu: %s vs %s\n", i, events[i].c_str(), want[i].c_str()); break; }
        return 1;
    }
    if (events.size() < 76 * 2) return 2;
    std::printf("objectIterateRunLists check passed\n");
    return 0;
}
