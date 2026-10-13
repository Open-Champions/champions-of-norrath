// Host check only; not a build input. The argument lands unchanged at +392 and nowhere else.
#include "set_display_rows.cpp"
#include <cstdio>
#include <cstring>

int main() {
    static unsigned char object[512];
    const int values[] = {0, 1, 7, -3, 0x7fffffff};
    bool ok = true;
    for (int v : values) {
        std::memset(object, 0x3c, sizeof object);
        SetDisplayRows__9VIWndEditi(object, v);
        int got;
        std::memcpy(&got, object + 392, 4);
        if (got != v) ok = false;
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 392 || i >= 396) && object[i] != 0x3c) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
