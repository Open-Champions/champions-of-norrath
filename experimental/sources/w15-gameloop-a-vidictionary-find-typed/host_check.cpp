// Host check for FindTyped__12VIDictionaryUi14VIResourceTypeRi. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "find_typed.cpp"

static int findResult, endResult, endCalls;
static unsigned seenKey;
extern "C" int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(void*, const unsigned* key) { seenKey = *key; return findResult; }
extern "C" int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void*) { ++endCalls; return endResult; }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    using namespace norrath_aoc;
    unsigned char* dictionary = arena_at(0x100);
    unsigned char* nodes = arena_at(0x1000);
    store_pointer(dictionary + 8, nodes);
    store_word(nodes + 28 * 2 + 20, 12); store_word(nodes + 28 * 2 + 24, 0x55);
    store_word(nodes + 28 * 3 + 20, 11); store_word(nodes + 28 * 3 + 24, 0x66);
    int out = -1;
    findResult = 2; endResult = 5;
    CHECK(FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, 0xabc, 12, &out) == 1);
    CHECK(out == 0x55 && seenKey == 0xabc && endCalls == 1);
    out = -1; findResult = 3;
    CHECK(FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, 1, 12, &out) == 0 && out == -1);   // wrong type
    findResult = 5;
    CHECK(FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, 1, 12, &out) == 0 && out == -1);   // absent
    std::puts("ok");
    return 0;
}
