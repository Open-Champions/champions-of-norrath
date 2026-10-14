// Host check only; not a build input. The interface call gets 0 and only +1920/+1924 change.
#include "end_connect.cpp"
#include <cstdio>
#include <cstring>

static int calls, seen = -1;
extern "C" int ezNetCtlDownInterface(int value) { ++calls; seen = value; return 9; }

int main() {
    static unsigned char object[2048], expect[2048];
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(expect, object, sizeof expect);
    std::memset(expect + 1920, 0, 8);
    EndConnect__12VIWndConnect(object);
    bool ok = calls == 1 && seen == 0 && std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
