// Host check only; not a build input. Callees are recorders; the object is pre-filled with a
// pattern so every byte the constructor changes beyond the listed words and cleared blocks
// would show. The same call order and changed memory were compared with the original in a
// private interpreter run.
#define memset memset_hook
#include "ctor.cpp"
#undef memset
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$20VIWndGenericRenderer[8];
static unsigned char storage[0x800];
static std::string trace;
static std::string at(void* p) { return std::to_string(static_cast<unsigned char*>(p) - storage); }
static const char server_name[] = "server";

extern "C" {
void __5VIWnd(void* s) { trace += "base@" + at(s) + ";"; }
void __t6VIList1ZP9VIGUIPage(void* s) { trace += "list@" + at(s) + ";"; }
void __19VIFlatFileStructure(void* s) { trace += "ffs@" + at(s) + ";"; }
void __9VIWndEdit(void* s) { trace += "edit@" + at(s) + ";"; }
const char* LoadSRSServer__Fv() { trace += "srs;"; return server_name; }
void __16VIRealmInterfaceP20VIWndGenericRendererPCc(void* s, void* renderer, const char* server) {
    trace += "realm@" + at(s) + "," + at(renderer) + (server == server_name ? ",server;" : ",?;");
}
void* memset_hook(void* destination, int value, unsigned count) {
    trace += "memset@" + at(destination) + "," + std::to_string(value) + "," + std::to_string(count) + ";";
    std::memset(destination, value, count);
    return destination;
}
}

int main() {
    std::memset(storage, 0x5a, sizeof storage);
    __20VIWndGenericRenderer(storage);
    bool ok = trace ==
        "base@0;list@40;ffs@1364;edit@1424;srs;realm@1880,0,server;"
        "memset@1220,0,64;memset@1284,0,64;memset@1092,0,64;memset@1156,0,64;memset@64,0,1024;";
    if (!ok) std::puts(trace.c_str());
    const int zero_words[] = {36, 52, 56, 60, 1088, 1348, 1352, 1360, 1400, 1404, 1408, 1412, 1416};
    for (int i = 0; i < 0x800; ++i) {
        bool zero = (i >= 64 && i < 1088) || (i >= 1092 && i < 1284 + 64);
        for (int w : zero_words) if (i >= w && i < w + 4) zero = true;
        const bool minus_one = i >= 1356 && i < 1360;
        const bool table = i >= 32 && i < 36;
        unsigned char want = zero ? 0 : minus_one ? 0xff : 0x5a;
        if (!table && storage[i] != want) { ok = false; std::printf("byte %d is %02x\n", i, storage[i]); break; }
    }
    unsigned w;
    std::memcpy(&w, storage + 32, 4);
    if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$20VIWndGenericRenderer))) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
