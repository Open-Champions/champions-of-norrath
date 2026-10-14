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
static int begin_result;
extern "C" int Begin__Ct6VIPool1Z17VIHSpritePlayNode(const void* p) { trace += "begin@" + n(addr(p)) + ";"; return begin_result; }
extern "C" int End__Ct6VIPool1Z17VIHSpritePlayNode(const void* p) { trace += "end@" + n(addr(p)) + ";"; return 0x7fffffff; }
int main() {
    bool ok = true;
    for (int variant = 0; variant < 3; ++variant) {
        std::memset(arena, 0x5a, sizeof arena); trace.clear();
        unsigned* p = W(0x100);
        p[2] = 0x1000;
        auto link = [&](int i, int next) { std::memcpy(arena + 0x1000 + 52 * i + 44, &next, 4); };
        begin_result = variant == 0 ? 0x7fffffff : variant == 1 ? 2 : 5;
        if (variant == 1) link(2, -1);
        if (variant == 2) { link(5, 1); link(1, 3); link(3, -1); }
        static unsigned char before[sizeof arena];
        std::memcpy(before, arena, sizeof arena);
        if (DestroyPool__t6VIPool1Z17VIHSpritePlayNode(p) != 0) ok = false;
        if (std::memcmp(before, arena, sizeof arena) != 0) ok = false;
        std::string want = "begin@256;";
        const int ends[] = {1, 3, 5};
        for (int i = 0; i < ends[variant]; ++i) want += "end@256;";
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
