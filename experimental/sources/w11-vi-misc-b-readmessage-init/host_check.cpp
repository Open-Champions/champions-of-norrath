// Host check only; not a build input. Clear runs once with the object, then the result is 0.
#include "init.cpp"
#include <cstdio>

static int calls;
static void* seen;
extern "C" void Clear__16VIWndReadMessage(void* self) { ++calls; seen = self; }

int main() {
    static char object[16];
    const int r = Init__16VIWndReadMessage(object);
    bool ok = r == 0 && calls == 1 && seen == object;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
