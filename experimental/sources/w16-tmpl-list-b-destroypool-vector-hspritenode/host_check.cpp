// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "destroypool.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned addr(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static unsigned* W(unsigned a) { return reinterpret_cast<unsigned*>(arena + a); }
static std::string trace;
static std::string n(unsigned v) { return std::to_string(v); }
static int mutate_count_at_call = -1;
extern "C" void _$_10VIMatrix44(void* o, int flags) {
    trace += "dtor@" + n(addr(o)) + "," + n(flags) + ";";
    if (mutate_count_at_call >= 0 && (int)trace.size() > 0 && ++mutate_count_at_call == 1) W(0x100)[0] = 2;
}
int main() {
    bool ok = true;
    for (unsigned count : {0u, 1u, 2u, 3u}) {
        std::memset(arena, 0x5a, sizeof arena); trace.clear(); mutate_count_at_call = -1;
        unsigned* p = W(0x100);
        p[0] = count; p[2] = 0x2000;
        if (DestroyPool__t8VIVector1Z13VIHSpriteNode(p) != 0) ok = false;
        std::string want;
        for (unsigned i = 0; i < count; ++i) {
            unsigned char* e = arena + 0x2000 + 256 * i;
            want += "dtor@" + n(addr(e) + 160) + ",2;" + "dtor@" + n(addr(e) + 48) + ",2;";
        }
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
        if (p[0] != count || p[2] != 0x2000) ok = false;
    }
    // the count is re-read after every pass: shrinking it from 5 to 2 during the first pass leaves 2 passes
    {
        std::memset(arena, 0x5a, sizeof arena); trace.clear(); mutate_count_at_call = 0;
        unsigned* p = W(0x100);
        p[0] = 5; p[2] = 0x2000;
        DestroyPool__t8VIVector1Z13VIHSpriteNode(p);
        std::string want;
        for (unsigned i = 0; i < 2; ++i) {
            unsigned char* e = arena + 0x2000 + 256 * i;
            want += "dtor@" + n(addr(e) + 160) + ",2;" + "dtor@" + n(addr(e) + 48) + ",2;";
        }
        // pass 0 compares i == 5 - 1 with the count read before the calls, pass 1 compares with 2 - 1
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
