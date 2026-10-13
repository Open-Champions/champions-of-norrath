// Host check only; not a build input.
#include "t.cpp"
#include <cstdio>
#include <string>

unsigned short sConvert16[1024];
static std::string trace;
extern "C" void str8to16__8VIStringPUsUiPCc(unsigned short* dest, unsigned size, const char* text) {
    trace += std::string(dest == sConvert16 ? "buf" : "other") + "," + std::to_string(size) + "," + text + ";";
}

int main() {
    const bool ok = T__10VIUIWindowPCc(nullptr, "ab") == sConvert16 && trace == "buf,2048,ab;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
