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
static bool clear_word; static int result;
extern "C" int DestroyPool__t7VIArray1Z7VIVect3(void* p) { trace += "destroy@" + n(addr(p)) + ";"; if (clear_word) static_cast<unsigned*>(p)[1] = 0; return result; }
extern "C" void __builtin_vec_delete(void* p) { trace += "vecdel@" + n(addr(p)) + ";"; }
int main() {
    bool ok = true;
    const int results[] = {0, 7, -1, -100};
    for (int has = 0; has < 2; ++has) for (int clr = 0; clr < 2; ++clr) for (int r : results) {
        std::memset(arena, 0x5a, sizeof arena); trace.clear(); clear_word = clr; result = r;
        unsigned* p = W(0x100);
        p[0] = 9; p[2] = 0x2222;
        p[1] = has ? 0x2000 : 0;
        int got = Clear__t7VIArray1Z7VIVect3(p);
        std::string want;
        bool neg = false;
        if (has) { want += "destroy@256;"; if (r < 0) neg = true; else if (!clr) want += "vecdel@8192;"; }
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
        if (neg) {
            if (got != -1 || p[0] != 9 || p[1] != (clr ? 0u : 0x2000u)) ok = false;
        } else {
            if (got != 0 || p[0] != 0 || p[1] != 0) ok = false;
            
        }
        if (p[2] != 0x2222 || p[3] != 0x5a5a5a5au) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
