// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin -w host_check.cpp
#define NORRATH_HOST_ARENA 1
#include "notify.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return static_cast<unsigned>(p - arena); }
extern "C" void norrath_host_call_pointer_int(unsigned, unsigned char*, int) {}
extern "C" void norrath_host_call_pointer_pointer_int_float(unsigned, unsigned char*, unsigned char*, int, float) {}
extern "C" int norrath_host_call_pointer_int_pointer_ret_int(unsigned, unsigned char*, int, const float*) { return 0; }
char g_localIp[24];
int g_localPort;
unsigned _5VIWnd$mUI;
static std::string trace;
static unsigned short textbuf[4];
extern "C" {
int printf(const char*, ...) { trace += "printf;"; return 0; }
char* strcpy(char* d, const char* s) { trace += std::string("strcpy(") + (d == g_localIp ? "ip" : "?") + "," + s + ");"; std::memcpy(d, s, std::strlen(s) + 1); return d; }
unsigned long long GetTime__7VITimer() { trace += "time;"; return 0x1122334455667788ull; }
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += "str(" + std::to_string((const unsigned char*)t - arena) + "," + std::to_string(id) + "," + std::to_string(col) + ");";
    return textbuf;
}
void SetStringValue__9VIGUIPagePCcPCUsb(void* p, const char* k, const unsigned short* v, bool f) {
    trace += "set(" + std::to_string((unsigned char*)p - arena) + "," + k + "," + (v == textbuf ? "text" : "?") + "," + std::to_string(f) + ");";
}
int GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(void* s, void* a, const char* h, int p) {
    trace += "inet(" + std::to_string((unsigned char*)s - arena) + "," + std::to_string((unsigned char*)a - arena) + "," + h + "," + std::to_string(p) + ");";
    return 0;
}
}
int main() {
    std::memset(arena, 0x5a, sizeof arena);
    unsigned char* obj = arena + 0x100;     // address 0x100
    unsigned char* renderer = arena + 0xa000;
    unsigned char* table = arena + 0xb000;
    *reinterpret_cast<unsigned*>(obj + 12) = 0xa000;
    *reinterpret_cast<unsigned*>(renderer + 52) = 0xc000;
    *reinterpret_cast<unsigned*>(table + 48) = 9;
    _5VIWnd$mUI = 0xb000;
    notifyClientDiscovered__16VIRealmInterfacePCci(obj, "172.16.0.9", 4242);
    bool ok = trace == "printf;strcpy(ip,172.16.0.9);time;str(45056,1856,9);set(49152,ServerStatus,text,1);inet(3652,38268,172.16.0.9,1000);";
    ok = ok && g_localPort == 4242 && std::strcmp(g_localIp, "172.16.0.9") == 0;
    ok = ok && *reinterpret_cast<unsigned long long*>(obj + 9720) == 0x1122334455667788ull && *reinterpret_cast<unsigned*>(obj + 3468) == 11;
    ok = ok && obj[2348] == 0x5a && obj[3372] == 0x5a && obj[0] == 0x5a && obj[3467] == 0x5a;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
