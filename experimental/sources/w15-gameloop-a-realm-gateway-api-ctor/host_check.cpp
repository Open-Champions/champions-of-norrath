// Host check for __Q212RealmService15RealmGatewayAPIUiPCcs. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <string>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "ctor.cpp"

unsigned char vt_realm_gateway_api[16];
static unsigned char* nextBlock;
static unsigned requestedSize;
static int coreCalls, seenB;
static void* seenCore; static void* seenApi; static const char* seenName;
extern "C" void* __builtin_new(unsigned size) { requestedSize = size; return nextBlock; }
extern "C" void __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI(void* core, const char* name, short b,
                                                                                            void* api) {
    ++coreCalls; seenCore = core; seenName = name; seenB = b; seenApi = api;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    using namespace norrath_aoc;
    unsigned char* self = arena_at(0x100);
    nextBlock = arena_at(0x2000);
    __Q212RealmService15RealmGatewayAPIUiPCcs(self, 2, "host", 0xa0e1);   // low half sign-extends to a negative short
    CHECK(requestedSize == 144 && coreCalls == 1 && seenCore == nextBlock && seenApi == self);
    CHECK(seenB == static_cast<short>(0xa0e1) && std::string(seenName) == "host");
    CHECK(load_word(self + 4) == 2 && load_pointer(self) == nextBlock && load_pointer(self + 8) == vt_realm_gateway_api);
    nextBlock = nullptr;
    __Q212RealmService15RealmGatewayAPIUiPCcs(self, 9, "x", 1);
    CHECK(coreCalls == 1 && load_word(self) == 0 && load_word(self + 4) == 9);
    std::puts("ok");
    return 0;
}
