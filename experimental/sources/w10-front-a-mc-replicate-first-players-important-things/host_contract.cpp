#include "mc_replicate_first_players_important_things.cpp"

#include <cstdio>
#include <cstring>

unsigned char ramSlot[200000];
void* gameCam;
int gameNetReplicateQuestItems;
static const int camBytes = 30, gateBytes = 50;
extern "C" {
int GetSaveFootPrint__6Camera(void*) { return camBytes; }
int jumpGateSaveFootPrint__Fv() { return gateBytes; }
void MC_ByteCopy__FPvT0i(void* to, void* from, int n) { std::memmove(to, from, n); }
}

int main() {
    for (int i = 0; i < 200000; ++i) ramSlot[i] = static_cast<unsigned char>(i * 31 + 7);
    static unsigned char before[200000];
    unsigned char* p = ramSlot + 112 + camBytes + gateBytes + 4104;
    unsigned char* e0 = p + 14;
    std::strcpy(reinterpret_cast<char*>(e0 + 36), "hero");
    std::strcpy(reinterpret_cast<char*>(e0 + 23998 + 36), "a much longer previous name");
    std::memcpy(before, ramSlot, sizeof ramSlot);
    MC_ReplicateFirstPlayersImportantThings__Fv();
    if (gameNetReplicateQuestItems != 1) return 1;
    for (int n = 1; n < 4; ++n) {
        unsigned char* h = p + 23998 * n;
        if (std::memcmp(h, p, 14) != 0) return 10 + n;
        unsigned char* r = h + 14;
        if (std::strcmp(reinterpret_cast<char*>(r + 36), "hero") != 0) return 20 + n;
        if (std::memcmp(r + 68, e0 + 68, 12) != 0) return 30 + n;
        if (std::memcmp(r + 17768, e0 + 17768, 4100) != 0) return 40 + n;
        // bytes outside the four copied ranges keep their value, and so do the first's
        if (r[45] != before[r - ramSlot + 45] || r[17767] != before[r - ramSlot + 17767] ||
            r[17768 + 4100] != before[r - ramSlot + 17768 + 4100] || r[80] != before[r - ramSlot + 80])
            return 50 + n;
    }
    if (std::memcmp(ramSlot + 112 + camBytes + gateBytes + 4104, before + 112 + camBytes + gateBytes + 4104, 23998) != 0)
        return 60;
    std::puts("mc replicate first players important things passed");
    return 0;
}
