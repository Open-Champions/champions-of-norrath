#include "mc_get_net_game_info.cpp"

#include <cstdio>
#include <cstring>

unsigned char ramSlot[0x20000];
Camera* gameCam;
static const char* seen_key;
static unsigned short wide[] = {0x0041, 0x1142, 0x0043, 0};

int GetSaveFootPrint__6Camera(Camera*) { return 100; }
int jumpGateSaveFootPrint__Fv() { return 8; }
unsigned short* textGetString__FPc(char* key) {
    seen_key = key;
    return wide;
}

int main() {
    char name[8];
    std::memset(name, 0x77, sizeof name);
    ramSlot[108] = 0xfd;
    ramSlot[112 + 100 + 8 + 4104 + 14 + 88] = 0x85;
    int first = 0;
    int second = 0;
    MC_GetNetGameInfo__FPiT0Pc(&first, &second, name);
    if (seen_key != reinterpret_cast<char*>(ramSlot)) return 1;
    if (std::memcmp(name, "A\x42" "C\0\x77", 5) != 0 || name[5] != 0x77) return 2;
    if (second != -3 || first != -123) return 3;
    wide[0] = 0;
    MC_GetNetGameInfo__FPiT0Pc(&first, &second, name);
    if (name[0] != 0 || name[1] != 0x42) return 4;
    std::puts("mc get net game info passed");
    return 0;
}
