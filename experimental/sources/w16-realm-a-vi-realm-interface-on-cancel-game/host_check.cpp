// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin -w host_check.cpp
#define NORRATH_HOST_ARENA 1
#include "on_cancel_game.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return static_cast<unsigned>(p - arena); }
extern "C" void norrath_host_call_pointer_int(unsigned, unsigned char*, int) {}
extern "C" void norrath_host_call_pointer_pointer_int_float(unsigned, unsigned char*, unsigned char*, int, float) {}
extern "C" int norrath_host_call_pointer_int_pointer_ret_int(unsigned, unsigned char*, int, const float*) { return 0; }
unsigned _5VIWnd$mUI;
static std::string trace;
static unsigned char* object;
static unsigned short texts[4000];
extern "C" {
int printf(const char*, ...) { trace += "printf;"; return 0; }
void Reset__16VIRealmInterface(void* s) { trace += "reset(" + std::to_string(*(unsigned*)((unsigned char*)s + 3468)) + ");"; }
void UnLock__11VIGUIDialog(void* d) { trace += "unlock(" + std::to_string((unsigned char*)d - arena) + ");"; }
void Show__11VIGUIDialogPCUsPCc(void* d, const unsigned short* t, const char* e) { trace += "show(" + std::to_string((unsigned char*)d - arena) + "," + std::to_string(t - texts) + "," + std::to_string(e != 0) + ");"; }
void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* t, bool f) { trace += "text(" + std::to_string((unsigned char*)d - arena) + "," + std::to_string(t - texts) + "," + std::to_string(f) + ");"; }
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) { trace += "col" + std::to_string(col) + ";"; return texts + id; }
}
int main() {
    std::memset(arena, 0x5a, sizeof arena);
    object = arena + 0x100;
    *reinterpret_cast<unsigned*>(object + 12) = 0xa000;
    *reinterpret_cast<unsigned*>(arena + 0xa000 + 56) = 0xc000;
    *reinterpret_cast<unsigned*>(arena + 0xb000 + 48) = 3;
    _5VIWnd$mUI = 0xb000;
    bool ok = true;
    struct { int status; const char* want; } cases[] = {
        {0, "reset(0);printf;unlock(49152);col3;show(49152,1840,0);col3;text(49152,1854,0);"},
        {1, "reset(0);printf;col3;text(49152,1861,0);"},
        {2, "reset(0);printf;col3;show(49152,1835,0);col3;text(49152,1855,0);"},
        {-1, "reset(0);printf;col3;show(49152,1835,0);col3;text(49152,1855,0);"},
    };
    for (auto& c : cases) {
        trace.clear();
        *reinterpret_cast<unsigned*>(object + 3468) = 99;
        onCancelGame__16VIRealmInterfaceUiiPv(object, 5, c.status, nullptr);
        ok = ok && trace == c.want && *reinterpret_cast<unsigned*>(object + 3468) == 0;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
