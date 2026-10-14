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
extern "C" void __builtin_delete(void* p) { trace += "delete@" + n(addr(p)) + ",head@" + n(W(0x100)[1]) + ";"; }
int main() {
    bool ok = true;
    for (unsigned count = 0; count <= 3; ++count) {
        std::memset(arena, 0x5a, sizeof arena); trace.clear();
        unsigned* l = W(0x100);
        const unsigned nodes[3] = {0x1000, 0x2000, 0x3000};
        l[0] = count; l[1] = count ? nodes[0] : 0; l[2] = count ? nodes[count - 1] : 0;
        for (unsigned i = 0; i < count; ++i) { W(nodes[i])[1] = i + 1 < count ? nodes[i + 1] : 0; W(nodes[i])[2] = 0x77; }
        if (Clear__t6VIList1ZP12VIPrimBuffer(l) != 0) ok = false;
        std::string want;
        for (unsigned i = 0; i < count; ++i) want += "delete@" + n(nodes[i]) + ",head@" + n(nodes[i]) + ";";
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
        if (l[0] != 0 || l[1] != 0 || l[2] != 0) ok = false;
        if (l[3] != 0x5a5a5a5au) ok = false;
        for (unsigned i = 0; i < count; ++i) if (W(nodes[i])[2] != 0x77) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
