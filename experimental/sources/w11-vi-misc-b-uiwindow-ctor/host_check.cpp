// Host check only; not a build input. Fills the object with a pattern and checks that exactly the
// documented ranges become zero and every other byte keeps the pattern. The same fill was compared
// with the original in a private interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

int main() {
    static unsigned char object[512], expect[512];
    for (int i = 0; i < 512; ++i) object[i] = expect[i] = static_cast<unsigned char>(0x41 + i * 7);
    const int zero_ranges[][2] = {{4, 8}, {12, 8}, {20, 8}, {304, 48}, {352, 8}, {36, 4}, {40, 4}, {44, 4}, {48, 1}};
    for (const auto& r : zero_ranges) std::memset(expect + r[0], 0, r[1]);
    __10VIUIWindow(object);
    bool ok = std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
