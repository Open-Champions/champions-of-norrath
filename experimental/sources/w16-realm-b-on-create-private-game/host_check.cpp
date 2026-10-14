// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
// Stub callees record their arguments in call order; the expected trace is written out independently per status.
#include <cstdio>
#include <string>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "on_create_private_game.cpp"
unsigned vi_wnd_mui;
static std::string trace;
static unsigned short strings[4000];
extern "C" {
int stub_printf(const char* f, ...) __asm__("_printf");
int stub_printf(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    unsigned a = __builtin_va_arg(ap, unsigned); int b = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("printf:") + f + std::to_string(a) + "," + std::to_string(b) + ";";
    return 0;
}
void* stub_memset(void* d, int v, unsigned long n) __asm__("_memset");
void* stub_memset(void* d, int v, unsigned long n) {
    trace += "memset:" + std::to_string(static_cast<unsigned char*>(d) - arena_at(0x100)) + "," + std::to_string(v) + "," + std::to_string(n) + ";";
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned long i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v);
    return d;
}
int stub_strcmp(const char* a, const char* b) __asm__("_strcmp");
int stub_strcmp(const char* a, const char* b) { trace += std::string("strcmp:") + a + "," + b + ";"; return std::string(a) == b ? 0 : 1; }
int stub_snprintf(char* out, unsigned long size, const char* f, ...) __asm__("_snprintf");
int stub_snprintf(char* out, unsigned long size, const char* f, ...) {
    trace += "snprintf:" + std::to_string(out - reinterpret_cast<char*>(arena_at(0x100))) + "," + std::to_string(size) + "," + f + ";";
    return 0;
}
int snprintf16__8VIStringPUsiPCUse(unsigned short* out, int count, const unsigned short* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    const char* s = __builtin_va_arg(ap, const char*);
    __builtin_va_end(ap);
    trace += "sn16:" + std::to_string(count) + "," + std::to_string(f - strings) + "," + s + ";";
    out[0] = 'W'; out[1] = static_cast<unsigned short>(f - strings); out[2] = 0;
    return 0;
}
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third) {
    trace += "get:" + std::to_string(static_cast<unsigned char*>(table) - arena_at(0x900)) + "," + std::to_string(id) + "," + std::to_string(third) + ";";
    return strings + id;
}
void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* t, int f) {
    std::string shown = t >= strings && t < strings + 4000 ? "str" + std::to_string(t - strings) : "wide" + std::to_string(t[1]);
    trace += "settext:" + std::to_string(static_cast<unsigned char*>(d) - arena_at(0x600)) + "," + shown + "," + std::to_string(f) + ";";
}
void SetStringValue__9VIGUIPagePCcT1b(void* p, const char* n, const char* v, int f) {
    trace += "setvalue:" + std::to_string(static_cast<unsigned char*>(p) - arena_at(0x700)) + "," + n + "=" + v + "," + std::to_string(f) + ";";
}
void AddPage__20VIWndGenericRendererPCc(void* r, const char* p) { trace += "page:" + std::to_string(static_cast<unsigned char*>(r) - arena_at(0x400)) + "," + p + ";"; }
void UpdateNumPlayers__16VIRealmInterfacei(void* s, int v) { trace += "players:" + std::to_string(static_cast<unsigned char*>(s) - arena_at(0x100)) + "," + std::to_string(v) + ";"; }

}
int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(0x100);
    store_pointer(object + 12, arena_at(0x400));
    store_pointer(arena_at(0x400) + 56, arena_at(0x600));
    store_pointer(arena_at(0x400) + 52, arena_at(0x700));
    store_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui), arena_at(0x900));
    store_word(arena_at(0x900) + 48, 77);
    const char* hosts[] = {nullptr, "sdrealm-test", "other"};
    const char* names[] = {"MyGame", "Cool [Hard]", "[x"};
    for (int status : {0, 1, 36, 40, 2, -1}) {
        for (const char* host : hosts) {
            for (const char* name : names) {
                trace.clear();
                for (int i = 0; i < 4576; ++i) object[5088 + i] = 0x55;
                { char* d = reinterpret_cast<char*>(object + 276); const char* s = name; while ((*d++ = *s++)) {} }
                store_word(object + 2324, 0xabcd); store_word(object + 3468, 99);
                onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv(object, 4, status, host, 4321, nullptr);
                std::string want = "printf:VIRealmInterface::onCreatePublicGame(%i, %i)\n4," + std::to_string(status) + ";memset:5088,0,4576;";
                unsigned w2324 = 0xabcd, w3468 = 99;
                if (status == 0) {
                    want += "get:0,1848,77;sn16:4095,1848," + std::string(name) + ";settext:0,wide1848,0;";
                    w2324 = 4321;
                    if (host && std::string(host) == "sdrealm-test") want += "strcmp:sdrealm-test,sdrealm-test;snprintf:1300,1023,sdrealm-test.station.sony.com;";
                    else if (host) want += "strcmp:other,sdrealm-test;snprintf:1300,1023,other;";
                    w3468 = 1;
                    want += "page:0,data/ui/GameServ.xml;";
                    std::string shown = name;
                    want += "setvalue:0,GameName=" + shown + ",0;players:0,1;";
                } else if (status == 36) {
                    want += "get:0,1847,77;sn16:4095,1847," + std::string(name) + ";settext:0,wide1847,0;";
                } else {
                    int id = status == 40 ? 1849 : status == 1 ? 1861 : 1850;
                    want += "get:0,"+ std::to_string(id) + ",77;settext:0,str" + std::to_string(id) + ",0;";
                }
                bool cleared = true;
                for (int i = 0; i < 4576; ++i) cleared = cleared && object[5088 + i] == 0;
                ok = ok && trace == want && cleared && load_word(object + 2324) == w2324 && load_word(object + 3468) == w3468;
                if (trace != want) std::fputs(("status " + std::to_string(status) + " name " + name + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout);
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
