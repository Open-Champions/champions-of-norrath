// Host contract for _$_5Spell; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_5Spell[8];
static std::string calls;
static unsigned char* current;
static int failures;
void listDestroy__FP8ListHead(void* head) {
    unsigned table; std::memcpy(&table, current + 20, 4);
    char b[64]; std::snprintf(b, sizeof b, "list(%ld)@%s;", static_cast<long>(static_cast<unsigned char*>(head) - current),
                              table == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_5Spell)) ? "5Spell" : "?");
    calls += b;
}
void blockFree__FPv(void* block) { calls += block == current ? "free;" : "free-other;"; }
int main() {
    alignas(16) unsigned char object[64];
    current = object;
    const int cases[] = {0, 1, 2, 3, static_cast<int>(0x80000001u), 0x7ffffffe, -1};
    for (int flags : cases) {
        std::memset(object, 0xee, sizeof object);
        calls.clear();
        destructor_5Spell(object, flags);
        unsigned table; std::memcpy(&table, object + 20, 4);
        const bool wantFree = (flags & 1) != 0;
        const std::string want = std::string("list(4)@5Spell;") + (wantFree ? "free;" : "");
        if (table != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_5Spell))) { std::printf("fail: table flags %d\n", flags); ++failures; }
        if (calls != want) { std::printf("fail: flags %d calls %s\n", flags, calls.c_str()); ++failures; }
        if (object[0] != 0xee || object[4] != 0xee) { std::printf("fail: bytes touched flags %d\n", flags); ++failures; }
    }
    if (failures == 0) std::puts("ok");
    return failures;
}
