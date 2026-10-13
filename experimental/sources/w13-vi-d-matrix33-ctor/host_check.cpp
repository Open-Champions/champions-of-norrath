// Host check only; not a build input. The Zero stub records the call and the object bytes at call time.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

static unsigned char object[0x80], seen[0x80];
static int calls;
static void* arg;
extern "C" void Zero__10VIMatrix33(void* self) { ++calls; arg = self; std::memcpy(seen, self, sizeof seen); }

int main() {
    unsigned char expect[sizeof object];
    std::memset(object, 0x5a, sizeof object);
    std::memset(expect, 0x5a, sizeof expect);
    std::memset(expect, 0, 36);
    __10VIMatrix33(object);
    bool ok = calls == 1 && arg == object && std::memcmp(seen, expect, sizeof seen) == 0 && std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
