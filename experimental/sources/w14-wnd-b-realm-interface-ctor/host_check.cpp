// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin host_check.cpp. Pointer words are truncated to 32 bits on the host, so the check compares
// call order, sizes, offsets, counters and the empty-string reference count rather than address values.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

StringRep nilRep = {0, 0, 5, 0};
unsigned char vt_realm_interface[16];
static std::string trace;
static unsigned char* object;
static int clones;
extern "C" {
void __Q212RealmService15RealmGatewayAPIUiPCcs(void*, unsigned a, const char* n, short b) { trace += "base(" + std::to_string(a) + "," + n + "," + std::to_string(b) + ");"; }
void __11VIUDPSocketi(void* s, int v) { trace += "udp(" + std::to_string(static_cast<unsigned char*>(s) - object) + "," + std::to_string(v) + ");"; }
void* memset(void* d, int v, unsigned long n) {
    trace += "m" + std::to_string(static_cast<unsigned char*>(d) - object) + "/" + std::to_string(n) + ";";
    for (unsigned long i = 0; i < n; ++i) static_cast<unsigned char*>(d)[i] = static_cast<unsigned char>(v);
    return d;
}
int printf(const char*, ...);
unsigned long long GetTime__7VITimer() { trace += "time;"; return 0x1122334455667788ull; }
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep*) { ++clones; return nullptr; }
}

int main() {
    static unsigned char buf[40000];
    object = buf;
    bool ok = true;
    std::memset(buf, 0xcc, sizeof buf);
    trace.clear();
    ctor_16VIRealmInterface(buf, buf + 100, nullptr);
    const std::string expect_start = "base(2,con.soegames.net,-24735);m16/256;m276/1024;m1300/1024;m2328/20;m2348/1024;m3376/20;udp(3396,-1);m3480/400;m3884/400;";
    ok = ok && trace.compare(0, expect_start.size(), expect_start) == 0;
    const std::string expect_end = "m276/1024;m1300/1024;m2328/20;m2348/1024;m3376/20;m16/256;m3480/400;m3884/400;m5088/4576;m9672/20;m9692/24;time;m9736/256;m9992/25600;m35592/20;m35612/2400;";
    ok = ok && trace.size() > expect_end.size() && trace.compare(trace.size() - expect_end.size(), expect_end.size(), expect_end) == 0;
    ok = ok && nilRep.references == 5 + 200 && clones == 0;
    ok = ok && trace.find("m5088/4576;m9672/20;m9692/24;m276") != std::string::npos;
    // the 100-entry tables hold 32-bit words (not zero, since they hold the empty-string data pointer)
    ok = ok && *reinterpret_cast<unsigned*>(buf + 4284) != 0 && *reinterpret_cast<unsigned*>(buf + 5080) != 0;
    ok = ok && *reinterpret_cast<unsigned*>(buf + 4284) == *reinterpret_cast<unsigned*>(buf + 4684);
    nilRep.shared = 1; clones = 0; trace.clear();
    ctor_16VIRealmInterface(buf, buf, "host");
    ok = ok && clones == 200 && trace.compare(0, 7, "base(2,") == 0 && trace.find("base(2,host,") == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
