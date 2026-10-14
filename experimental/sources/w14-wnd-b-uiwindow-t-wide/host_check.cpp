// Host check only; not a build input.
#include "t.cpp"
#include <cstdio>

int main() {
    static const unsigned short s[2] = {0x41, 0};
    const bool ok = T__10VIUIWindowPCUs(nullptr, s) == s && T__10VIUIWindowPCUs(nullptr, nullptr) == nullptr;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
