#include <cstring>
#include <cstdio>
#define strcpy unit_strcpy
#define strcmp unit_strcmp
#include "player_global_init.cpp"
#undef strcpy
#undef strcmp
extern "C" char* unit_strcpy(char* d, const char* s) { return strcpy(d, s); }
extern "C" int unit_strcmp(const char* a, const char* b) { return strcmp(a, b); }
alignas(8) unsigned char gSkillTree[5 * 3592 + 64];
SkillTreeEntry WarriorSkillTree[14], ClericSkillTree[10], ShadowKnightSkillTree[9], RangerSkillTree[7], WizardSkillTree[10];
static void fillTree(SkillTreeEntry* t, int count, int seed) {
    memset(t, 0x77, sizeof(SkillTreeEntry) * (count + 1));
    for (int i = 0; i < count; ++i) {
        snprintf(t[i].name, 32, "skill%d_%d", seed, i);
        t[i].b32 = (signed char)(i - 3); t[i].b33 = (signed char)(seed + i); t[i].b34 = (signed char)(-i);
        t[i].b35 = 0x42; t[i].f36 = 0.25f * i + seed; t[i].q40 = 0x1122334455667788LL * (seed + 1) + i;
        t[i].w48 = 0x31313131; t[i].w52 = 0x52525252;
    }
    strcpy(t[count].name, "END");
    t[count].b32 = 9; t[count].q40 = 99;
}
// independent model of the expected object
static void expectTree(unsigned char* obj, int row, const SkillTreeEntry* src) {
    for (int i = 0;; ++i) {
        unsigned char* e = obj + row * 3592 + i * 56;
        if (!strcmp(src[i].name, "END")) { memcpy(e, "END", 4); return; }
        memcpy(e, src[i].name, strlen(src[i].name) + 1);
        e[32] = (unsigned char)src[i].b32; e[33] = (unsigned char)src[i].b33; e[34] = (unsigned char)src[i].b34;
        memcpy(e + 36, &src[i].f36, 4); memcpy(e + 40, &src[i].q40, 8);
    }
}
int main() {
    fillTree(WarriorSkillTree, 13 - 1, 1); fillTree(ClericSkillTree, 8, 2);
    fillTree(ShadowKnightSkillTree, 7, 3); fillTree(RangerSkillTree, 5, 4); fillTree(WizardSkillTree, 8, 5);
    unsigned char expect[sizeof gSkillTree];
    memset(gSkillTree, 0xcc, sizeof gSkillTree);
    memset(expect, 0xcc, sizeof expect);
    for (int row = 0; row < 5; ++row)
        for (int i = 0; i < 64; ++i) { unsigned char* e = expect + row * 3592 + i * 56; e[32] = 0xff; e[33] = 1; memset(e + 48, 0, 4); }
    expectTree(expect, 0, WarriorSkillTree); expectTree(expect, 1, ClericSkillTree);
    expectTree(expect, 2, ShadowKnightSkillTree); expectTree(expect, 3, RangerSkillTree); expectTree(expect, 4, WizardSkillTree);
    playerGlobalInit__Fv();
    if (memcmp(gSkillTree, expect, sizeof gSkillTree)) {
        for (unsigned i = 0; i < sizeof gSkillTree; ++i) if (gSkillTree[i] != expect[i]) { printf("diff at %u\n", i); break; }
        return 1;
    }
    if (sizeof(SkillTreeEntry) != 56) return 2;
    // the sources are not modified
    if (strcmp(WarriorSkillTree[0].name, "skill1_0")) return 3;
    return 0;
}
