#include <cstring>
#include <cstdlib>
struct In { unsigned char b[126]; };
struct Out { unsigned char b[8]; };
extern "C" In inMsg[8];
extern "C" Out outMsg;
extern "C" int frontEndRunning;
extern "C" void textInit__Fv();
In inMsg[8];
Out outMsg;
int frontEndRunning;

int main() {
    memset(inMsg, 0x77, sizeof inMsg);
    memset(&outMsg, 0x55, sizeof outMsg);
    frontEndRunning = 3;
    textInit__Fv();
    if (inMsg[3].b[5] != 0x77 || outMsg.b[0] != 0x55) return 1;
    frontEndRunning = 0;
    textInit__Fv();
    for (int e = 0; e < 8; e++)
        for (int i = 0; i < 126; i++)
            if (inMsg[e].b[i] != (i == 2 ? 0xfe : 0)) return 2;
    const unsigned char want[8] = {1, 0, 0x55, 32, 0, 0x55, 0x55, 0x55};
    if (memcmp(&outMsg, want, 8)) return 3;
    return 0;
}
