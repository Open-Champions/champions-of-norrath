// Host check only; not a build input. The Init stub records the call and the object bytes at call time.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$13VISceneFilter[8] __asm__("_vt$13VISceneFilter");
static unsigned char object[0x40], seen[0x40];
static int calls;
static void* arg;
extern "C" void Init__13VISceneFilter(void* self) { ++calls; arg = self; std::memcpy(seen, self, sizeof seen); }

int main() {
    unsigned char expect[sizeof object];
    std::memset(object, 0x5a, sizeof object);
    std::memset(expect, 0x5a, sizeof expect);
    unsigned vt = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13VISceneFilter));
    std::memcpy(expect + 16, &vt, 4);
    __13VISceneFilter(object);
    bool ok = calls == 1 && arg == object && std::memcmp(seen, expect, sizeof seen) == 0 && std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
