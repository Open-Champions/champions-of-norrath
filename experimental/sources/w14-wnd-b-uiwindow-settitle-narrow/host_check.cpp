// Host check only; not a build input.
#include "settitle.cpp"
#include <cstdio>
#include <string>

static int words[32];
static std::string trace;
extern "C" char* strcpy(char* dest, const char* src) {
    trace += std::to_string(dest - reinterpret_cast<char*>(words)) + "," + (src ? src : "null") + ";";
    return dest;
}

int main() {
    bool ok = true;
    words[6] = 100; words[7] = 5; trace.clear();
    SetTitle__10VIUIWindowPCc(words, "A");
    ok = ok && words[7] == 24 && words[6] == 100 - 10 + 48 && trace == "48,A;";
    words[6] = 100; words[7] = 24; trace.clear();
    SetTitle__10VIUIWindowPCc(words, nullptr);
    ok = ok && words[7] == 18 && words[6] == 100 - 48 + 36 && trace == "48,null;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
