// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <string>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "on_create_account.cpp"
unsigned vi_wnd_mui;
static std::string trace;
static unsigned short strings[4000];
static int appMode;
extern "C" {
int printf(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    unsigned a = __builtin_va_arg(ap, unsigned); int b = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("printf:") + f + std::to_string(a) + "," + std::to_string(b) + ";";
    return 0;
}
void UnLock__11VIGUIDialog(void* d) { trace += "unlock:" + std::to_string(static_cast<unsigned char*>(d) - arena_at(0x600)) + ";"; }
void SetText__11VIGUIDialogib(void* d, int id, int f) { trace += "textid:" + std::to_string(static_cast<unsigned char*>(d) - arena_at(0x600)) + "," + std::to_string(id) + "," + std::to_string(f) + ";"; }
void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* t, int f) { trace += "text:" + std::to_string(t - strings) + "," + std::to_string(f) + ";"; }
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third) {
    trace += "get:" + std::to_string(static_cast<unsigned char*>(table) - arena_at(0x900)) + "," + std::to_string(id) + "," + std::to_string(third) + ";";
    return strings + id;
}
void strncpyWide__FPUsPCUsUi(unsigned short* out, const unsigned short* in, unsigned n) {
    trace += "copy:" + std::to_string(reinterpret_cast<unsigned char*>(out) - arena_at(0x100)) + "," + std::to_string(n) + ";";
    for (unsigned i = 0; i < n; ++i) { out[i] = in[i]; if (!in[i]) break; }
}
int strlenWide__FPCUs(const unsigned short* t) { int n = 0; while (t[n]) ++n; trace += "len:" + std::to_string(reinterpret_cast<const unsigned char*>(t) - arena_at(0x100)) + ";"; return n; }
void* GetAppPtr__5VIWnd(void* r) { trace += "app:" + std::to_string(static_cast<unsigned char*>(r) - arena_at(0x400)) + ";"; return arena_at(0xa00); }
void AddPage__20VIWndGenericRendererPCc(void* r, const char* p) { trace += "page:" + std::to_string(static_cast<unsigned char*>(r) - arena_at(0x400)) + "," + p + ";"; }
}
int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(0x100);
    store_pointer(object + 12, arena_at(0x400));
    store_pointer(arena_at(0x400) + 56, arena_at(0x600));
    store_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui), arena_at(0x900));
    store_word(arena_at(0x900) + 48, 5);
    static const unsigned short name[] = {'A', 'b', 'c', 0};
    const struct { int status; const char* tail; } cases[] = {
        {1, "get:0,1861,5;text:1861,0;"}, {36, "textid:0,1371,0;"}, {37, "get:0,1844,5;text:1844,0;"},
        {38, "get:0,1845,5;text:1845,0;"}, {39, "get:0,1846,5;text:1846,0;"}, {2, ""}, {-1, ""}, {35, ""}, {40, ""}};
    for (const auto& c : cases) {
        trace.clear(); store_word(object + 272, 999);
        onCreateAccount__16VIRealmInterfaceUiiPCUsPv(object, 8, c.status, name, nullptr);
        std::string want = "printf:VIRealmInterface::onCreateAccount(%i, %i)\n8," + std::to_string(c.status) + ";unlock:0;" + c.tail;
        ok = ok && trace == want && load_word(object + 272) == 999;
        if (trace != want) std::printf("status %d\n got  %s\n want %s\n", c.status, trace.c_str(), want.c_str());
    }
    for (unsigned mode : {1u, 0u, 2u}) {
        trace.clear(); store_word(arena_at(0xa00), mode);
        onCreateAccount__16VIRealmInterfaceUiiPCUsPv(object, 8, 0, name, nullptr);
        std::string want = "printf:VIRealmInterface::onCreateAccount(%i, %i)\n8,0;unlock:0;textid:0,1384,0;copy:16,256;len:16;app:0;page:0,";
        want += mode == 1 ? "data/ui/SRSMains.xml;" : "data/ui/SRSMainc.xml;";
        ok = ok && trace == want && load_word(object + 272) == 3 && object[16] == 'A' && object[18] == 'b';
        if (trace != want) std::printf("mode %u\n got  %s\n want %s\n", mode, trace.c_str(), want.c_str());
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
