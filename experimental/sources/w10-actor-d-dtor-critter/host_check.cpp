// Host contract for _$_7Critter; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>

unsigned char vt_7Critter[8];
static int baseCalls, frees, baseFlags = -1;
static void* baseObject;
static unsigned wordSeenByBase;
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
void creature_destructor(void* object, int flags) {
    ++baseCalls; baseObject = object; baseFlags = flags;
    std::memcpy(&wordSeenByBase, static_cast<unsigned char*>(object) + 72, 4);
}
void blockFree__FPv(void* object) { ++frees; expect(object == baseObject, "frees the same object, after the base call"); }
int main() {
    alignas(16) unsigned char object[80];
    std::memset(object, 0x3c, sizeof object);
    const unsigned wanted = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_7Critter));
    critter_destructor(object, 0);
    expect(baseCalls == 1 && baseFlags == 0 && frees == 0, "flag 0 calls the base with 0 and does not free");
    expect(wordSeenByBase == wanted, "table pointer stored before the base destructor");
    critter_destructor(object, 2);
    expect(baseCalls == 2 && frees == 0, "flag 2 does not free");
    critter_destructor(object, 1);
    expect(frees == 1, "flag 1 frees");
    critter_destructor(object, 5);
    expect(frees == 2 && baseCalls == 4 && baseFlags == 0, "any odd flag frees; base flag stays 0");
    return failures ? 1 : 0;
}
