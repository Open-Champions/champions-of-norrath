// Host check only; not a build input. Flags 0 to 3 and some high bits: delete is called with the
// object exactly when bit 0 is set.
#include "dtor.cpp"
#include <cstdio>

static int deletes;
static void* last;
extern "C" void __builtin_delete(void* p) { ++deletes; last = p; }

int main() {
    static char object[8];
    bool ok = true;
    for (int flags = 0; flags < 8; ++flags) {
        deletes = 0; last = nullptr;
        dtor_10VIUIWindow(object, flags);
        ok = ok && (flags & 1 ? deletes == 1 && last == object : deletes == 0);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
