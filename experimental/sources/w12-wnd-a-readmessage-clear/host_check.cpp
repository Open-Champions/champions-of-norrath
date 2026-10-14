// Host check only; not a build input. Also compared with the original in a private interpreter.
#include "clear.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[512];
    std::memset(object, 0x5a, sizeof object);
    Clear__16VIWndReadMessage(object);
    bool ok = object[412] == 0 && object[413] == 0;
    for (unsigned i = 0; i < sizeof object; ++i)
        if (!(i == 412 || i == 413) && object[i] != 0x5a) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
