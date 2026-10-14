// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static unsigned newResult, newSize, newCalls, ctorAt, addResult, addedRef;
static unsigned char* poolSeen;
extern "C" unsigned __builtin_new(unsigned size) { ++newCalls; newSize = size; return newResult; }
extern "C" void __t5VIMap2ZiZi(unsigned char* map) { ctorAt = static_cast<unsigned>(map - arena) + arena_base; }
extern "C" int Add__t6VIPool1ZP8VIRefMapRCP8VIRefMap(unsigned char* pool, const unsigned* entry) { poolSeen = pool; addedRef = *entry; return addResult; }
#include "create_ref_map.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    newResult = arena_base + 0x2000; addResult = 3;
    W(0x2000) = 0x55; W(0x2004) = 0x66;
    unsigned char* self = arena + 0x100;
    bool ok = CreateRefMap__7VIScenei(self, 0) == 3 && newSize == 28 && W(0x2000) == 0 && W(0x2004) == 0 && ctorAt == arena_base + 0x2008 && poolSeen == self + 1044 && addedRef == newResult;
    W(0x2004) = 0x66;
    ok = ok && CreateRefMap__7VIScenei(self, 1) == 3 && W(0x2004) == 1;
    addResult = 0; ok = ok && CreateRefMap__7VIScenei(self, 5) == 0 && W(0x2004) == 1;
    addResult = static_cast<unsigned>(-2); W(0x2004) = 0x77;
    ok = ok && CreateRefMap__7VIScenei(self, 1) == -1 && W(0x2004) == 0;   // not incremented on failure
    newResult = 0; newCalls = 0; ctorAt = 0;
    ok = ok && CreateRefMap__7VIScenei(self, 1) == -1 && newCalls == 1 && ctorAt == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
