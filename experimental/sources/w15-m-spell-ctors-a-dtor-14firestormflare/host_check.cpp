// Host contract for _$_14FireStormFlare; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_FireStormFlare[8];
static std::string calls;
static unsigned char* current;
static int failures;

void destructor_GameObject(void* self, int flags) {
    unsigned table; std::memcpy(&table, current + 72, 4);
    char b[80];
    std::snprintf(b, sizeof b, "base(%ld,%d)@%s;", static_cast<long>(static_cast<unsigned char*>(self) - current), flags,
                  table == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_FireStormFlare)) ? "14FireStormFlare" : "?");
    calls += b;
}
void blockFree__FPv(void* block) { calls += block == current ? "free;" : "free-other;"; }

int main() {
    alignas(16) unsigned char object[128];
    current = object;
    const int cases[] = {0, 1, 2, 3, static_cast<int>(0x80000001u), 0x7ffffffe, -1};
    for (int flags : cases) {
        std::memset(object, 0xee, sizeof object);
        calls.clear();
        destructor_FireStormFlare(object, flags);
        unsigned table; std::memcpy(&table, object + 72, 4);
        const bool wantFree = (flags & 1) != 0;
        const std::string want = std::string("base(0,0)@14FireStormFlare;") + (wantFree ? "free;" : "");
        if (table != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_FireStormFlare))) { std::printf("fail: table flags %d\n", flags); ++failures; }
        if (calls != want) { std::printf("fail: flags %d calls %s\n", flags, calls.c_str()); ++failures; }
        if (object[0] != 0xee || object[71] != 0xee || object[76] != 0xee) { std::printf("fail: bytes touched flags %d\n", flags); ++failures; }
    }
    if (failures == 0) std::puts("ok");
    return failures;
}
