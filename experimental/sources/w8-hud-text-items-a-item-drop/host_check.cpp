// Host contract for dropItem__4itemG6Point3Pc. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstdint>
#include <initializer_list>
#include <cstring>
static char arena[256];
unsigned char* norrath_host_base = reinterpret_cast<unsigned char*>(reinterpret_cast<std::uintptr_t>(arena) & ~std::uintptr_t(0xffffffffu));
#include "item_drop.cpp"
static unsigned W(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
extern "C" {
unsigned norrath_player_view_31f540[8 * 4];
}
static int createCalls, allocCalls, proxyCalls, ctorCalls, seqNext;
static int proxyAnswers[2];
static unsigned char seen[72];
static char* seenName; static void* seenDesc; static int seenKind, seenFlags, allocBytes;
static void* allocResult;
static void* ctorBlock; static float ctorPoint[3]; static char *ctorA, *ctorB, *ctorC; static long ctorKind;
static int ctorType; static void* ctorDesc; static float ctorScale; static int order[8], orderN;
extern "C" {
void create__4itemPcPQ24item11descriptionii(char* name, void* d, int kind, int flags) {
    ++createCalls; order[orderN++] = 1; seenName = name; seenDesc = d; seenKind = kind; seenFlags = flags;
    std::memcpy(seen, d, 72);
}
void* blockAlloc__Fi(int n) { ++allocCalls; order[orderN++] = 2; allocBytes = n; return allocResult; }
int getProxyMeshId__4itemPQ24item11description(void* d) {
    order[orderN++] = 3; if (d != seenDesc) return -1000;
    return proxyAnswers[proxyCalls++ & 1];
}
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* b, float* pt, char* a, char* bb, char* c, long k, int t, void* d, float s) {
    ++ctorCalls; order[orderN++] = 4; ctorBlock = b; std::memcpy(ctorPoint, pt, 12); ctorA = a; ctorB = bb; ctorC = c;
    ctorKind = k; ctorType = t; ctorDesc = d; ctorScale = s;
}
}
int main() {
    for (int i = 0; i < 8; ++i) {
        norrath_player_view_31f540[i * 4] = W(arena + i * 8);
        norrath_player_view_31f540[i * 4 + 1] = W(arena + i * 8 + 4);
        norrath_player_view_31f540[i * 4 + 2] = 0; norrath_player_view_31f540[i * 4 + 3] = 0;
    }
    char name[] = "x";
    float pos[3] = {1.5f, -2.0f, 10.0f};
    // Null block: only create and blockAlloc run.
    allocResult = nullptr;
    dropItem__4itemG6Point3Pc(pos, name);
    if (createCalls != 1 || allocCalls != 1 || proxyCalls != 0 || ctorCalls != 0) return 1;
    if (allocBytes != 584 || seenName != name || seenKind != -1 || seenFlags != 0) return 2;
    if (orderN != 2 || order[0] != 1 || order[1] != 2) return 3;
    for (int i = 16; i < 32; ++i) if (seen[i] != 0) return 4;
    if (seen[32] != 0xff || seen[33] != 0xff) return 5;
    for (int o : {36, 44, 52, 60}) if (seen[o] != 0xff || seen[o + 1] != 0xff) return 6;
    // Live block.
    static char block[8];
    allocResult = block; proxyAnswers[0] = 3; proxyAnswers[1] = 5; proxyCalls = 0; orderN = 0;
    dropItem__4itemG6Point3Pc(pos, name);
    if (proxyCalls != 2 || ctorCalls != 1) return 7;
    if (ctorBlock != block || ctorDesc != seenDesc) return 8;
    if (ctorPoint[0] != 1.5f || ctorPoint[1] != -2.0f || ctorPoint[2] != 46.0f) return 9;
    if (std::strcmp(ctorA, "equipmnt.lmp") != 0) return 10;
    if (ctorB != arena + 3 * 8 || ctorC != arena + 5 * 8 + 4) return 11;
    if (ctorKind != 1 || ctorType != 1089 || ctorScale != 1.0f) return 12;
    if (orderN != 5 || order[0] != 1 || order[1] != 2 || order[2] != 3 || order[3] != 3 || order[4] != 4) return 13;
    if (pos[0] != 1.5f || pos[2] != 10.0f) return 14;
    return 0;
}
