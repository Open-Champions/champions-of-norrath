// Host check only; not a build input. The call leaves a pattern-filled object unchanged.
#include "free.cpp"
#include <cstdio>
#include <cstring>

int main() {
    static unsigned char object[1024], expect[1024];
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(expect, object, sizeof expect);
    Free__12VIWndMessage(object);
    bool ok = std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
