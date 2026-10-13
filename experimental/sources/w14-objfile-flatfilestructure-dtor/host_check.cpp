// Host check only; not a build input.
#include "dtor.cpp"
#include <cstdio>
#include <string>
static int object;
static std::string trace;
static int base_flags = -1;
extern "C" void _$_19VIFlatFileDirectory(void* self, int flags) { trace += self == &object ? "base;" : "base?;"; base_flags = flags; }
extern "C" void __builtin_delete(void* p) { trace += p == &object ? "delete;" : "delete?;"; }
int main() {
    bool ok = true;
    const int flag_values[] = {0, 1, 2, 3, -1, -2};
    for (int f : flag_values) {
        trace.clear();
        base_flags = -1;
        _$_19VIFlatFileStructure(&object, f);
        if (trace != ((f & 1) ? "base;delete;" : "base;") || base_flags != 2) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
