// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x1000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
unsigned norrath_host_address(const void* pointer) { return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - arena); }
unsigned char _vt$19VIFlatFileDirectory[8] __asm__("_vt$19VIFlatFileDirectory");
static std::string trace;
static unsigned snapshot[8];
static bool constructor_sets_path;
extern "C" void __t6VIList1ZP10VIFlatFile(void* list) {
    trace += (static_cast<unsigned char*>(list) == arena + 0x100 + 24) ? "list;" : "list?;";
    std::memcpy(snapshot, arena + 0x100, sizeof snapshot);
    if (constructor_sets_path) *reinterpret_cast<unsigned*>(arena + 0x100) = 0x400;
}
extern "C" void __wrap_free(void* block) { trace += (static_cast<unsigned char*>(block) == arena + 0x400) ? "free;" : "free?;"; }
extern "C" char* __wrap_strdup(const char* text) {
    trace += std::strcmp(text, "/") == 0 ? "strdup;" : "strdup?;";
    return reinterpret_cast<char*>(arena + 0x500);
}
int main() {
    bool ok = true;
    for (int stale = 0; stale < 2; ++stale) {
        std::memset(arena, 0x5a, sizeof arena);
        trace.clear();
        unsigned* o = reinterpret_cast<unsigned*>(arena + 0x100);
        o[0] = 0x777;   // the constructor clears the path word before anything can free it
        constructor_sets_path = stale != 0;
        __19VIFlatFileStructure(o);
        unsigned vt = norrath_host_address(_vt$19VIFlatFileDirectory);
        if (trace != (stale ? "list;free;strdup;" : "list;strdup;")) ok = false;
        // seen by the list constructor: everything but +0 already stored, +20 not yet
        if (snapshot[0] != 0 || snapshot[1] != 0 || snapshot[2] != 0xffffffffu || snapshot[3] != 2 || snapshot[4] != vt || snapshot[5] != 0x5a5a5a5au) ok = false;
        if (o[0] != 0x500 || o[1] != 0 || o[2] != 0xffffffffu || o[3] != 2 || o[4] != vt || o[5] != 0) ok = false;
        if (o[6] != 0x5a5a5a5au) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
