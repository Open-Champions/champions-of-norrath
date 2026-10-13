// Host check only; not a build input.
#include "settitle.cpp"
#include <cstdio>
#include <string>

static int words[32];
static std::string trace;
extern "C" void str16to8__8VIStringPcUiPCUs(char* dest, unsigned size, const unsigned short* text) {
    trace += std::to_string(dest - reinterpret_cast<char*>(words)) + "," + std::to_string(size) + "," + (text ? "text" : "null") + ";";
}

int main() {
    static const unsigned short title[2] = {0x41, 0};
    bool ok = true;
    words[6] = 100; words[7] = 5; trace.clear();
    SetTitle__10VIUIWindowPCUs(words, title);
    ok = ok && words[7] == 24 && words[6] == 100 - 10 + 48 && trace == "48,256,text;";
    words[6] = 100; words[7] = 24; trace.clear();
    SetTitle__10VIUIWindowPCUs(words, nullptr);
    ok = ok && words[7] == 18 && words[6] == 100 - 48 + 36 && trace == "48,256,null;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
