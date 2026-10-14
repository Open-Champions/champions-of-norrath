// Host check only; not a build input. The McErr global forwards to OnDrawSelf__10VIWndMcErr; any
// other object draws nothing.
#include "ondrawself.cpp"
#include <cstdio>

unsigned char sWndMcErr[0x210];
static int calls;
static void* last;
extern "C" void OnDrawSelf__10VIWndMcErr(void* self) { ++calls; last = self; }

int main() {
    unsigned char other[0x210];
    OnDrawSelf__5VIWnd(other);
    bool ok = calls == 0;
    OnDrawSelf__5VIWnd(sWndMcErr);
    ok = ok && calls == 1 && last == sWndMcErr;
    OnDrawSelf__5VIWnd(sWndMcErr + 4);
    ok = ok && calls == 1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
