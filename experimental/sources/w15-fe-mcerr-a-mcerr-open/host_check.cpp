// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the 32-bit table word translates
// to a host table through a tag. Checks both messages (adjusted this, sender, message number, mcErrOn at the
// time of each call), the stored window words, the two printed lines and the zero return.
#define NORRATH_HOST_CONTRACT 1
#include "mcerr.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char sWndMcErr[0x210];
int mcErrOn;
static unsigned char parent[128];
static VtEntry table[8];
static std::string trace;
static const unsigned TAG_TABLE = 0x2000;

extern "C" char* norrath_host_translate(unsigned address) {
    return address == TAG_TABLE ? reinterpret_cast<char*>(table) : nullptr;
}
extern "C" int scePrintf(const char* format, ...) {
    trace += std::string("P[") + format + "]";
    return 0;
}
static void message_hook(void* self, void* from, int message, void* arg, int extra) {
    trace += "msg:" + std::to_string(static_cast<unsigned char*>(self) - parent) + "," +
             (from == sWndMcErr ? "E" : "?") + "," + std::to_string(message) + "," + (arg == sWndMcErr ? "E" : "?") +
             "," + std::to_string(extra) + ",on=" + std::to_string(mcErrOn) + ";";
}

int main() {
    bool ok = true;
    table[6] = VtEntry{16, 6, message_hook};
    std::memcpy(parent + 32, &TAG_TABLE, 4);
    std::memset(sWndMcErr, 0x77, sizeof sWndMcErr);
    const int size[2] = {640, 448};
    const int r = McErr__FP5VIWndRC8VIVect2Ii(parent, size, 12);
    const int* w = reinterpret_cast<int*>(sWndMcErr);
    ok = ok && r == 0 && mcErrOn == 0;
    ok = ok && trace == "P[McErr (sWndMcErr = %d, Parent = %d, InitialScreen = %d)\n]msg:16,E,4352,E,0,on=0;"
                        "msg:16,E,4356,E,0,on=1;P[VIWndMcErr::GotoScreen %d\n]";
    ok = ok && w[103] == 12 && w[105] == 640 && w[106] == 448 && w[108] == 0 && w[102] == 0 && w[104] == 0 &&
         static_cast<unsigned>(w[107]) == static_cast<unsigned>(reinterpret_cast<unsigned long>(parent));
    ok = ok && w[101] == 0x77777777 && w[109] == 0x77777777 && w[110] == 0x77777777;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
