// Host check for Add__12VIDictionaryR10VIResourceUi14VIResourceTypei. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "add.cpp"

#include <cstdio>

static int nextResult;
static void* seenMap;
static unsigned seenKey;
static int seenType, seenHandle, calls;
extern "C" int Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry(void* map, const unsigned* key,
                                                                                                  const void* entry) {
    ++calls; seenMap = map; seenKey = *key;
    seenType = static_cast<const int*>(entry)[0]; seenHandle = static_cast<const int*>(entry)[1];
    return nextResult;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    unsigned resource[4] = {0xffffffffu, 7, 7, 7};
    int dictionary[4];
    nextResult = 3;
    CHECK(Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, resource, 0x1234, 12, 99) == 0);
    CHECK(calls == 1 && seenMap == dictionary && seenKey == 0x1234 && seenType == 12 && seenHandle == 99);
    CHECK(resource[0] == 0x1234 && resource[1] == 7);
    nextResult = -4;
    CHECK(Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, resource, 5, 1, 2) == -1);
    nextResult = 0;
    CHECK(Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, resource, 6, 1, 2) == 0);
    std::puts("ok");
    return 0;
}
