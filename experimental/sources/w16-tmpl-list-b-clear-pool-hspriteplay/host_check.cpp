// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "clear.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned addr(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static unsigned* W(unsigned a) { return reinterpret_cast<unsigned*>(arena + a); }
static std::string trace;
static std::string n(unsigned v) { return std::to_string(v); }
static bool clear_word;
extern "C" int DestroyPool__t6VIPool1Z13VIHSpritePlay(void* p) { trace += "destroy@" + n(addr(p)) + ";"; if (clear_word) static_cast<unsigned*>(p)[2] = 0; return 5; }
extern "C" void __builtin_vec_delete(void* p) { trace += "vecdel@" + n(addr(p)) + ";"; }
int main() {
    bool ok = true;
    for (int has = 0; has < 2; ++has) for (int clr = 0; clr < 2; ++clr) {
        std::memset(arena, 0x5a, sizeof arena); trace.clear(); clear_word = clr;
        unsigned* p = W(0x100);
        p[2] = has ? 0x2000 : 0;
        if (Clear__t6VIPool1Z13VIHSpritePlay(p) != 0) ok = false;
        std::string want;
        if (has) { want += "destroy@256;"; if (!clr) want += "vecdel@8192;"; }
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
        if (p[0] != 0 || p[1] != 0 || p[2] != 0 || p[3] != 0xffffffffu || p[4] != 0xffffffffu || p[5] != 0xffffffffu) ok = false;
        if (p[6] != 0x5a5a5a5au) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
