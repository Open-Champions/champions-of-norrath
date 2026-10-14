// Host check for objectProcessDeleteList__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "object_process_delete_list.cpp"

int objectIgnoreFlags;
unsigned char deleteList[8];
static std::vector<std::string> events;
static std::vector<unsigned> chain;
static unsigned position;
static void show(unsigned* it) {
    norrath_aoc::store_word(reinterpret_cast<unsigned char*>(it) + 4, position < chain.size() ? chain[position] : 0);
}
extern "C" void __16ListSafeIteratorP8ListHeadi(unsigned* it, unsigned char* head, int mask) {
    events.push_back(std::string("C") + (head == deleteList ? "L" : "?") + std::to_string(mask));
    position = 0; show(it);
}
extern "C" void next__16ListSafeIterator(unsigned* it) { events.push_back("N"); ++position; ++objectIgnoreFlags; show(it); }
extern "C" void _$_16ListSafeIterator(unsigned*, int flags) { events.push_back("D" + std::to_string(flags)); }
extern "C" void norrath_host_call_pointer_int(unsigned function, unsigned char* first, int second) {
    events.push_back("F" + std::to_string(function) + "@" + std::to_string(norrath_host_address(first)) + "," + std::to_string(second));
}

int main() {
    using namespace norrath_aoc;
    objectIgnoreFlags = 9;
    // three nodes: object with adjustment 16, a node with no object, object with adjustment -8
    unsigned char* vt = arena_at(0x100);
    store_half(vt + 8, 16); store_word(vt + 12, 0x7001);
    unsigned char* vt2 = arena_at(0x200);
    store_half(vt2 + 8, static_cast<unsigned>(-8) & 0xffff); store_word(vt2 + 12, 0x7002);
    unsigned char* o1 = arena_at(0x1000); store_pointer(o1 + 72, vt);
    unsigned char* o2 = arena_at(0x2000); store_pointer(o2 + 72, vt2);
    unsigned char* n[3] = {arena_at(0x3000), arena_at(0x3020), arena_at(0x3040)};
    store_pointer(n[0] + 12, o1); store_word(n[1] + 12, 0); store_pointer(n[2] + 12, o2);
    for (unsigned char* x : n) chain.push_back(norrath_host_address(x));
    objectProcessDeleteList__Fv();
    std::vector<std::string> want = {
        "CL9",
        "F28673@" + std::to_string(norrath_host_address(o1 + 16)) + ",3", "N",
        "N",
        "F28674@" + std::to_string(norrath_host_address(o2 - 8)) + ",3", "N",
        "D2"};
    if (events != want) {
        std::printf("event sequence differs\n");
        for (const std::string& e : events) std::printf("  %s\n", e.c_str());
        return 1;
    }
    if (objectIgnoreFlags != 12) { std::printf("mask word was re-read or advance count wrong\n"); return 2; }
    std::puts("ok");
    return 0;
}
