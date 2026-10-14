// Host check only; not a build input. The callees record the order and arguments of the calls.
#include "dtor.cpp"
#include <cstdio>
#include <string>
alignas(8) static unsigned char room[160];
static std::string trace;
extern "C" int Clear__10VIZoneRoom(unsigned char* r) { trace += "clear@" + std::to_string(r - room) + ";"; return 0; }
extern "C" void _$_t7VIArray1Zi(void* a, int f) { trace += "arr@" + std::to_string(static_cast<unsigned char*>(a) - room) + "," + std::to_string(f) + ";"; }
extern "C" void __builtin_delete(void* p) { trace += "delete@" + std::to_string(static_cast<unsigned char*>(p) - room) + ";"; }
int main() {
    bool ok = true;
    const int flag_values[] = {0, 1, 2, 3, -1, -2};
    for (int flags : flag_values) {
        trace.clear();
        _$_10VIZoneRoom(room, flags);
        std::string want = "clear@0;arr@68,2;arr@60,2;";
        if (flags & 1) want += "delete@0;";
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
