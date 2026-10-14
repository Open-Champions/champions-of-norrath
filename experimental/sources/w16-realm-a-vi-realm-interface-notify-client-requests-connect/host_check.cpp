// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin -w host_check.cpp
#include "notify.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static std::string trace;
static unsigned char* object;
static int bump;   // added to the count by each strcmp call, to show the count is re-read
extern "C" {
int printf(const char*, ...) { trace += "printf;"; return 0; }
int strcmp(const char* a, const char* b) { trace += std::string("cmp(") + a + "," + std::to_string((b - (const char*)object - 5088) / 1144) + ");"; *(int*)(object + 3476) += bump; return 1; }
void* memset(void* d, int v, unsigned long n) { trace += "m" + std::to_string((unsigned char*)d - object) + "/" + std::to_string(n) + ";"; for (unsigned long i = 0; i < n; ++i) ((unsigned char*)d)[i] = (unsigned char)v; return d; }
int snprintf(char* d, unsigned long n, const char* f, ...) { trace += "sn(" + std::to_string((unsigned char*)d - object) + "," + std::to_string(n) + "," + f + ");"; std::strcpy(d, "entry"); return 0; }
unsigned long long GetTime__7VITimer() { trace += "time;"; return 0x1122334455667788ull; }
void UpdateNumPlayers__16VIRealmInterfacei(void*, int c) { trace += "upd(" + std::to_string(c) + ");"; }
}
static unsigned char buf[40000];
int main() {
    object = buf;
    bool ok = true;
    // count 3 of max 5: two strcmp calls, then entry index 2
    std::memset(buf, 0x5a, sizeof buf);
    *(int*)(buf + 3476) = 3; *(int*)(buf + 3472) = 5; bump = 0; trace.clear();
    notifyClientRequestsConnect__16VIRealmInterfacePCci(buf, "pc1", 9000);
    ok = ok && trace == "printf;cmp(pc1,0);cmp(pc1,1);m7376/1144;sn(7376,1023,pc1);time;m8468/52;upd(4);";
    ok = ok && *(int*)(buf + 7376 + 1024) == 9000 && *(int*)(buf + 7376 + 1048) == 7 && *(unsigned long long*)(buf + 7376 + 1056) == 0x1122334455667788ull;
    ok = ok && std::memcmp(buf + 7376, "entry", 6) == 0 && buf[7376 + 1143] == 0 && buf[7376 + 1143 + 1] == 0x5a && buf[7375] == 0x5a;
    // full table: only the loop runs
    std::memset(buf, 0x5a, sizeof buf);
    *(int*)(buf + 3476) = 5; *(int*)(buf + 3472) = 5; trace.clear();
    notifyClientRequestsConnect__16VIRealmInterfacePCci(buf, "pc2", 1);
    ok = ok && trace == "printf;cmp(pc2,0);cmp(pc2,1);cmp(pc2,2);cmp(pc2,3);";
    // count 1 or 0: no strcmp
    *(int*)(buf + 3476) = 1; *(int*)(buf + 3472) = 2; trace.clear();
    notifyClientRequestsConnect__16VIRealmInterfacePCci(buf, "pc3", 1);
    ok = ok && trace == "printf;m5088/1144;sn(5088,1023,pc3);time;m6180/52;upd(2);";
    *(int*)(buf + 3476) = 0; *(int*)(buf + 3472) = 2; trace.clear();
    notifyClientRequestsConnect__16VIRealmInterfacePCci(buf, "pc4", 1);
    ok = ok && trace == "printf;m3944/1144;sn(3944,1023,pc4);time;m5036/52;upd(1);";
    // the count is re-read after each strcmp
    *(int*)(buf + 3476) = 4; *(int*)(buf + 3472) = 9; bump = -1; trace.clear();
    notifyClientRequestsConnect__16VIRealmInterfacePCci(buf, "pc5", 1);
    ok = ok && trace.compare(0, 24, "printf;cmp(pc5,0);cmp(pc") == 0 && trace.find("cmp(pc5,2)") == std::string::npos;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
