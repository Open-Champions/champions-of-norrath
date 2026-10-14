// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin -w host_check.cpp
#include "notify.cpp"
#include <cstdio>
#include <cstring>
#include <string>
char g_localIp[24];
int g_localPort;
static std::string trace;
static unsigned char* object;
extern "C" {
int printf(const char* f, ...) { trace += std::string("printf;"); (void)f; return 0; }
char* strcpy(char* d, const char* s) { trace += std::string("strcpy(") + (d == g_localIp ? "ip" : std::to_string(d - (char*)object)) + "," + s + ");"; std::memcpy(d, s, std::strlen(s) + 1); return d; }
int GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(void* s, void* a, const char* h, int p) {
    trace += "inet(" + std::to_string((unsigned char*)s - object) + "," + std::to_string((unsigned char*)a - object) + "," + h + "," + std::to_string(p) + ");";
    return 0;
}
}
int main() {
    static unsigned char buf[40000];
    object = buf;
    std::memset(buf, 0x5a, sizeof buf);
    bool ok = true;
    notifyGameDiscovered__16VIRealmInterfacePCci(buf, "10.1.2.3", -77);
    ok = ok && trace == "printf;strcpy(ip,10.1.2.3);strcpy(2348,10.1.2.3);inet(3396,38012,10.1.2.3,1000);";
    ok = ok && g_localPort == -77 && std::strcmp(g_localIp, "10.1.2.3") == 0 && std::strcmp((char*)buf + 2348, "10.1.2.3") == 0;
    ok = ok && *reinterpret_cast<int*>(buf + 3372) == -77 && *reinterpret_cast<int*>(buf + 3468) == 2;
    ok = ok && buf[0] == 0x5a && buf[3371] == 0x5a && buf[3376] == 0x5a && buf[3467] == 0x5a && buf[3472] == 0x5a;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
