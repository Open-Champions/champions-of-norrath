// Host check only; not a build input.
#include "dtor.cpp"
#include <cstdio>
#include <string>
static int object;
static std::string trace;
extern "C" void Unload__13VIStringTable(void* self) { trace += self == &object ? "unload;" : "unload?;"; }
extern "C" void __builtin_delete(void* p) { trace += p == &object ? "delete;" : "delete?;"; }
int main() {
    bool ok = true;
    const int flag_values[] = {0, 1, 2, 3, -1, -2};
    for (int f : flag_values) {
        trace.clear();
        _$_13VIStringTable(&object, f);
        if (trace != ((f & 1) ? "unload;delete;" : "unload;")) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
