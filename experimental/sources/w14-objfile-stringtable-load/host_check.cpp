// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "load.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x1000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace;
static int r_load, r_proc, r_fix;
static void* raw_seen;
extern "C" void __t7VIArray1Zc(void* a) { trace += "ctor;"; raw_seen = a; }
extern "C" void _$_t7VIArray1Zc(void* a, int f) { trace += (a == raw_seen && f == 2) ? "dtor;" : "dtor?;"; }
extern "C" int LoadRawFile__13VIStringTablePCcRt7VIArray1Zc(const char* n, void* raw) { trace += (raw == raw_seen && n && std::strcmp(n, "x") == 0) ? "load;" : "load?;"; return r_load; }
extern "C" int ProcessRawFile__13VIStringTableRCt7VIArray1Zc(void*, const void* raw) { trace += raw == raw_seen ? "proc;" : "proc?;"; return r_proc; }
extern "C" int FixupTextBuffer__13VIStringTable(void*) { trace += "fix;"; return r_fix; }
extern "C" void __builtin_vec_delete(void* p) { trace += "del" + std::to_string(static_cast<unsigned char*>(p) - arena) + ";"; }
int main() {
    bool ok = true;
    const int rs[] = {-3, -1, 0, 1, 4};
    for (int a : rs) for (int b : rs) for (int c : rs) {
        std::memset(arena, 0x5a, sizeof arena);
        unsigned* o = reinterpret_cast<unsigned*>(arena + 0x100);
        o[0] = 5; o[1] = 6; o[2] = 0x200; o[3] = 0x300;
        trace.clear(); r_load = a; r_proc = b; r_fix = c;
        int r = Load__13VIStringTablePCc(o, "x");
        std::string want = "ctor;load;";
        if (a >= 0) { want += "proc;"; if (b >= 0) want += "fix;"; }
        bool good = a >= 0 && b >= 0 && c >= 0;
        if (!good) want += "del512;del768;";
        want += "dtor;";
        if (trace != want || r != (good ? 0 : -1)) ok = false;
        if (good ? (o[0] != 5 || o[1] != 6 || o[2] != 0x200 || o[3] != 0x300) : (o[0] || o[1] || o[2] || o[3])) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
