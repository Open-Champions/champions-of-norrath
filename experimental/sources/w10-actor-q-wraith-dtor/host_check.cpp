// Host check only; not an EE build input.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char vt_Wraith[4];
static std::string calls; static int failures;
static void* object_ptr;
extern "C" void listDestroy__FP8ListHead(void* h) { calls += "destroy(" + std::to_string(static_cast<unsigned char*>(h) - static_cast<unsigned char*>(object_ptr)) + ");"; }
extern "C" void creature_destructor(void* s, int f) __asm__("_$_8Creature");
extern "C" void creature_destructor(void* s, int f) { calls += std::string("creature(") + (s == object_ptr ? "self" : "?") + "," + std::to_string(f) + ");"; }
extern "C" void blockFree__FPv(void* b) { calls += std::string("free(") + (b == object_ptr ? "self" : "?") + ");"; }
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s (%s)\n", m, calls.c_str()); ++failures; } }
int main() {
    alignas(8) static unsigned char object[1024];
    object_ptr = object;
    for (int flags : {0, 1, 2, 3, -1, -2}) {
        std::memset(object, 0, sizeof object); calls.clear();
        wraith_destructor(object, flags);
        const bool frees = (flags & 1) != 0;
        expect(calls == std::string("destroy(968);creature(self,0);") + (frees ? "free(self);" : ""), "call order and free on bit 0 only");
        expect(*reinterpret_cast<unsigned*>(object + 72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Wraith)), "table pointer stored at +72");
    }
    if (!failures) std::puts("wraith dtor ok");
    return failures ? 1 : 0;
}
