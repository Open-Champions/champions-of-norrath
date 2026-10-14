#include "modify_ram_slot_for_net_save.cpp"

#include <cstdio>
#include <cstring>

static int camera_token;
void* gameCam = &camera_token;
unsigned char ramSlot[200000];
unsigned char originalYaw[8];
unsigned char originalPos[48];
unsigned char tempPlayerStruct[24000];
unsigned char gameOver;
static int fp_calls, jg_calls, nl_calls, local_index = 2;
extern "C" int GetSaveFootPrint__6Camera(void* c) { ++fp_calls; return c == &camera_token ? 32 : -1; }
extern "C" int jumpGateSaveFootPrint__Fv() { ++jg_calls; return 16; }
extern "C" int netGetLocalPlayerNm__Fi(int p) { ++nl_calls; return p == 0 ? local_index : -1; }

static const int kEntry = 112 + 32 + 16 + 4104;  // offset of entry 0 header

int main() {
    for (unsigned i = 0; i < sizeof ramSlot; ++i) ramSlot[i] = static_cast<unsigned char>(i * 31 + 7);
    unsigned char before[200000];
    std::memcpy(before, ramSlot, sizeof ramSlot);
    modifyRamSlotForNetSave__Fv();
    if (fp_calls != 5 || jg_calls != 5 || nl_calls != 3) return 1;
    for (int n = 0; n < 4; ++n) {
        const unsigned char* h = before + kEntry + 23998 * n;
        if (originalYaw[2 * n] != h[0] || originalYaw[2 * n + 1] != h[1]) return 2;
        if (std::memcmp(originalPos + 12 * n, h + 2, 12) != 0) return 3;
    }
    // entry 0 header now holds the local player's (index 2) header values
    const unsigned char* h2 = before + kEntry + 23998 * 2;
    if (ramSlot[kEntry] != h2[0] || ramSlot[kEntry + 1] != h2[1]) return 4;
    if (std::memcmp(ramSlot + kEntry + 2, h2 + 2, 12) != 0) return 5;
    // old record 0 is saved in tempPlayerStruct, and replaced by record 2
    if (std::memcmp(tempPlayerStruct, before + kEntry + 14, 23984) != 0) return 6;
    if (std::memcmp(ramSlot + kEntry + 14, before + kEntry + 14 + 23998 * 2, 23984) != 0) return 7;
    // record 1 and 2 stay as they were
    if (std::memcmp(ramSlot + kEntry + 14 + 23998, before + kEntry + 14 + 23998, 23984) != 0) return 8;
    if (std::memcmp(ramSlot + kEntry + 14 + 23998 * 2, before + kEntry + 14 + 23998 * 2, 23984) != 0) return 9;
    // the halfword at 106 and the word at base + 4100
    if (ramSlot[106] != 1 || ramSlot[107] != 0) return 10;
    const int w = 112 + 32 + 16 + 4100;
    if (ramSlot[w] != 1 || ramSlot[w + 1] != 0 || ramSlot[w + 2] != 0 || ramSlot[w + 3] != 0) return 11;
    if (std::memcmp(ramSlot, before, 106) != 0) return 12;  // gameOver was zero
    // gameOver replaces the first thirteen bytes
    gameOver = 1;
    std::memcpy(ramSlot, before, 13);
    modifyRamSlotForNetSave__Fv();
    if (std::memcmp(ramSlot, "gameComplete", 13) != 0) return 13;
    std::puts("modify ram slot for net save passed");
    return 0;
}
