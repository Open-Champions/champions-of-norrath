// Host check only; not a build input.
#include "dtor.cpp"
#include <cstdio>
#include <string>
static unsigned char object[0x80];
static std::string trace;
extern "C" void __builtin_delete(void* p) { trace += p == object ? "delete;" : "delete?;"; }
int main() {
    bool ok = true;
    for (int flags : {0, 1, 2, 3, 4, 5, -1, -2}) {
        trace.clear();
        for (auto& b : object) b = 0x5a;
        _$_14VIEffectVolume(object, flags);
        if (trace != ((flags & 1) ? "delete;" : "")) ok = false;
        for (auto b : object) if (b != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
