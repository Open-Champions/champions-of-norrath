// Host check only; not a build input. Also compared with the original in a private interpreter.
#include "close.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[128];
    std::memset(object, 0x5a, sizeof object);
    Close__10VIUIWindowb(object, false);
    bool ok = object[40] == 0x5a && *reinterpret_cast<float*>(object + 44) == -1.0f;
    std::memset(object, 0x5a, sizeof object);
    Close__10VIUIWindowb(object, true);
    ok = ok && *reinterpret_cast<int*>(object + 40) == 0 && *reinterpret_cast<float*>(object + 44) == -1.0f && object[48] == 0x5a;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
