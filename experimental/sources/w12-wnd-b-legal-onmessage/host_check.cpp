// Host check only; not a build input. Passes the object's own low 32 address bits as the fifth
// argument, as the routine compares that word with this (no word is dereferenced).
#include "onmessage.cpp"
#include <cstdio>
#include <cstring>

static int calls;
extern "C" long long GetTime__7VITimer() { ++calls; return 0x1122334455667788ll; }

int main() {
    static unsigned char object[128], other[16];
    const int me = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(object));
    const int not_me = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(other));
    bool ok = true;
    struct Case { int message; int b; bool stores; };
    const Case cases[] = {{295, me, true}, {294, me, false}, {296, me, false}, {295, not_me, false}, {0, 0, false}};
    for (const Case& c : cases) {
        std::memset(object, 0x44, sizeof object);
        calls = 0;
        OnMessage__10VIWndLegalP5VIWndiii(object, other, c.message, 12, c.b);
        long long stored;
        std::memcpy(&stored, object + 40, 8);
        if (c.stores) {
            ok = ok && calls == 1 && stored == 0x1122334455667788ll;
        } else {
            ok = ok && calls == 0 && stored == 0x4444444444444444ll;
        }
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 40 || i >= 48) && object[i] != 0x44) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
