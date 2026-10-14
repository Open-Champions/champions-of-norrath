// Host check only; not a build input. Only the word at +984 changes and the result is 0.
#include "init.cpp"
#include <cstdio>
#include <cstring>

int main() {
    static unsigned char object[1024], expect[1024];
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(expect, object, sizeof expect);
    std::memset(expect + 984, 0, 4);
    const int r = Init__12VIWndMessage(object);
    bool ok = r == 0 && std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
