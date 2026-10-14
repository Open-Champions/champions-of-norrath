// Host check only; not a build input. The buffer word is a tag translated to a host address.
#define NORRATH_HOST_CONTRACT 1
#include "dtor.cpp"
#include <cstdio>
#include <string>
static unsigned object[40];
static unsigned char buffer[16];
static std::string trace;
extern "C" void* norrath_host_buffer(unsigned address) { return address == 0x1000 ? buffer : nullptr; }
extern "C" void Close__11VIUDPSocket(void* s) { trace += s == object ? "close;" : "close?;"; }
extern "C" void __wrap_free(void* p) { trace += p == buffer ? "free;" : "free?;"; }
extern "C" int sceLibnetTerminate(void* c) { trace += c == object + 2 ? "term;" : "term?;"; return 0; }
extern "C" void __builtin_delete(void* p) { trace += p == object ? "delete;" : "delete?;"; }
int main() {
    bool ok = true;
    for (unsigned buf : {0u, 0x1000u})
        for (int flags : {0, 1, 2, 3, -1, -2}) {
            trace.clear();
            for (auto& w : object) w = 0x5a5a5a5au;
            object[1] = buf;
            _$_11VIUDPSocket(object, flags);
            std::string want = "close;";
            if (buf) want += "free;term;";
            if (flags & 1) want += "delete;";
            if (trace != want) ok = false;
            if (object[0] != 0xffffffffu || object[17] != 0xffffffffu || object[1] != 0) ok = false;
            for (int i = 2; i < 40; ++i) if (i != 17 && object[i] != 0x5a5a5a5au) ok = false;
        }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
