#include "un_modify_ram_slot_for_net_save.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

void* gameCam;
int nmPlayers;
unsigned char ramSlot[0xb29c8];
unsigned char tempPlayerStruct[23984];
unsigned char originalYaw[8];
unsigned char originalPos[48];
static int camera_fp, gate_fp, camera_calls, gate_calls;
static void* camera_arg;
extern "C" int GetSaveFootPrint__6Camera(void* c) { ++camera_calls; camera_arg = c; return camera_fp; }
extern "C" int jumpGateSaveFootPrint__Fv() { ++gate_calls; return gate_fp; }
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)

static int run(int camera, int gate, int players) {
    std::memset(ramSlot, 0xee, sizeof ramSlot);
    for (int i = 0; i < 23984; ++i) tempPlayerStruct[i] = (unsigned char)(i * 3 + 1);
    for (int i = 0; i < 8; ++i) originalYaw[i] = (unsigned char)(0xa0 + i);
    for (int i = 0; i < 48; ++i) originalPos[i] = (unsigned char)(0x10 + i);
    camera_fp = camera; gate_fp = gate; nmPlayers = players; camera_calls = gate_calls = 0;
    static int token;
    gameCam = &token;
    unModifyRamSlotForNetSave__Fv();
    CHECK(camera_calls == 3 && gate_calls == 3 && camera_arg == &token);
    std::vector<unsigned char> expected(sizeof ramSlot, 0xee);
    // build the expected image independently of the function
    *(unsigned short*)&expected[106] = (unsigned short)players;
    for (int b = 0; b < 4; ++b) expected[0x70 + camera + gate + 4100 + b] = (unsigned char)((unsigned)players >> (8 * b));
    for (int i = 0; i < 23984; ++i) expected[0x70 + camera + gate + 4118 + i] = (unsigned char)(i * 3 + 1);
    for (int n = 0; n < 4; ++n) {
        unsigned char* e = &expected[0x70 + camera + gate + 4104 + 23998 * n];
        e[0] = 0xa0 + 2 * n; e[1] = 0xa1 + 2 * n;
        for (int k = 0; k < 12; ++k) e[2 + k] = (unsigned char)(0x10 + 12 * n + k);
    }
    // entry 0's payload is the stage-1 copy; later payloads keep the fill byte
    CHECK(std::memcmp(expected.data(), ramSlot, sizeof ramSlot) == 0);
    return 0;
}

int main() {
    if (run(0, 0, 1)) return 1;
    if (run(0x1234, 0x56, 3)) return 1;
    if (run(100, 7, 0x12345678)) return 1;
    std::puts("ok");
    return 0;
}
