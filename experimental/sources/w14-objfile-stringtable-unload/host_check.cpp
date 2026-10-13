// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "unload.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x1000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace;
extern "C" void __builtin_vec_delete(void* p) { trace += std::to_string(static_cast<unsigned char*>(p) - arena) + ";"; }
int main() {
    bool ok = true;
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            std::memset(arena, 0x5a, sizeof arena);
            unsigned* o = reinterpret_cast<unsigned*>(arena + 0x100);
            o[0] = 5; o[1] = 6; o[2] = a ? 0x200 : 0; o[3] = b ? 0x300 : 0;
            trace.clear();
            Unload__13VIStringTable(o);
            std::string want = std::string(a ? "512;" : "") + (b ? "768;" : "");
            if (trace != want) ok = false;
            if (o[0] || o[1] || o[2] || o[3] || o[4] != 0x5a5a5a5au) ok = false;
        }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
