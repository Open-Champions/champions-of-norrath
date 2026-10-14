// Host check only; not a build input.
#include "setmode.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char sWndMcErr[0x210];
static std::string trace;
extern "C" int scePrintf(const char* format, ...) {
    trace += format;
    return 0;
}

int main() {
    bool ok = true;
    for (int mode : {0, 1, 4, -3}) {
        std::memset(sWndMcErr, 0x5a, sizeof sWndMcErr);
        trace.clear();
        McErrSetMode__Fi(mode);
        ok = ok && *reinterpret_cast<int*>(sWndMcErr + 432) == mode && trace == "McErrSetMode (Mode = %d)\n";
        for (int i = 0; i < 0x210; ++i)
            if ((i < 432 || i >= 436) && sWndMcErr[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
