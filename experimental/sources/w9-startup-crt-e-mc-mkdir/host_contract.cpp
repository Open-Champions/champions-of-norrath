#include <cstdio>
#include "mc_mkdir.cpp"
extern "C" int mcRunCmdNo = 0;
static int p, s, m, open_result; static const char* n;
extern "C" int sceMcOpen(int port, int slot, const char* name, int mode) {
    p = port; s = slot; n = name; m = mode; return open_result;
}
int main() {
    const char* name = "dir";
    open_result = 0;
    if (sceMcMkdir(1, 2, name) != 0) return 1;
    if (p != 1 || s != 2 || n != name || m != 64 || mcRunCmdNo != 11) return 2;
    mcRunCmdNo = 0; open_result = -3;
    if (sceMcMkdir(0, 0, name) != -3 || mcRunCmdNo != 0) return 3;
    open_result = 5;
    if (sceMcMkdir(0, 0, name) != 5 || mcRunCmdNo != 0) return 4;
    std::puts("sceMcMkdir contract passed");
    return 0;
}
