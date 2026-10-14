// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
// Stub callees record their arguments in call order; the expected trace is written out independently per status.
#include <cstdio>
#include <cstring>
#include <string>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "on_do_client_discovery.cpp"
unsigned vi_wnd_mui;
static std::string trace;
static unsigned short strings[4000];
extern "C" {
int printf(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    unsigned a = __builtin_va_arg(ap, unsigned); int b = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("printf:") + f + std::to_string(a) + "," + std::to_string(b) + ";";
    return 0;
}
int strcmp(const char* a, const char* b) { trace += std::string("strcmp:") + a + "," + b + ";"; return std::string(a) == b ? 0 : 1; }
int stub_snprintf(char* out, unsigned size, const char* f, ...) __asm__("snprintf");
int stub_snprintf(char* out, unsigned size, const char* f, ...) {
    trace += "snprintf:" + std::to_string(out - reinterpret_cast<char*>(arena_at(0x100))) + "," + std::to_string(size) + "," + f + ";";
    return 0;
}
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third) {
    trace += "get:" + std::to_string(static_cast<unsigned char*>(table) - arena_at(0x900)) + "," + std::to_string(id) + "," + std::to_string(third) + ";";
    return strings + id;
}
void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* t, int f) {
    trace += "settext:" + std::to_string(static_cast<unsigned char*>(d) - arena_at(0x600)) + "," + std::to_string(t - strings) + "," + std::to_string(f) + ";";
}
void Show__11VIGUIDialogPCUsPCc(void* d, const unsigned short* t, const char* x) {
    trace += "show:" + std::to_string(static_cast<unsigned char*>(d) - arena_at(0x600)) + "," + std::to_string(t - strings) + "," + std::to_string(x != nullptr) + ";";
}
void GoBack__20VIWndGenericRenderer(void* r) { trace += "goback:" + std::to_string(static_cast<unsigned char*>(r) - arena_at(0x400)) + ";"; }
void Reset__16VIRealmInterface(void* s) { trace += "reset:" + std::to_string(static_cast<unsigned char*>(s) - arena_at(0x100)) + ";"; }
}
int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(0x100);
    store_pointer(object + 12, arena_at(0x400));
    store_pointer(arena_at(0x400) + 56, arena_at(0x600));
    store_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui), arena_at(0x900));
    store_word(arena_at(0x900) + 48, 77);
    const char* hosts[] = {nullptr, "sdrealm-test", "other"};
    for (int status : {0, 1, 2, -1, 36}) {
        for (const char* host : hosts) {
            trace.clear();
            std::memset(object + 1300, 'x', 8);
            store_word(object + 2324, 0xabcd); store_word(object + 3468, 99);
            onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv(object, 4, status, host, 4321, nullptr);
            std::string want = "printf:VIRealmInterface::onDoClientDiscovery(%i, %i)\n4," + std::to_string(status) + ";";
            unsigned w2324 = 0xabcd, w3468 = 99;
            if (status == 0) {
                w2324 = 4321; w3468 = 5;
                if (host && std::string(host) == "sdrealm-test") want += "strcmp:sdrealm-test,sdrealm-test;snprintf:1300,1023,sdrealm-test.station.sony.com;";
                else if (host) want += std::string("strcmp:other,sdrealm-test;snprintf:1300,1023,other;");
            } else if (status == 1) {
                want += "get:0,1861,77;settext:0,1861,0;";
            } else {
                want += "goback:0;get:0,1840,77;show:0,1840,0;get:0,1841,77;settext:0,1841,0;reset:0;";
                w3468 = 0;
            }
            ok = ok && trace == want && load_word(object + 2324) == w2324 && load_word(object + 3468) == w3468;
            if (trace != want) std::printf("status %d host %s\n got  %s\n want %s\n", status, host ? host : "null", trace.c_str(), want.c_str());
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
