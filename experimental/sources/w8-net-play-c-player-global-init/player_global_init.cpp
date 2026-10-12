// Observed playerGlobalInit__Fv at 0x0019e3e0, 928 bytes. It only touches the
// 17960-byte object gSkillTree (0x00422b90), which is five rows of 3592 bytes;
// a row holds 64 entries of 56 bytes (plus 8 trailing bytes). The entry layout
// below is read from the stores and loads of this function and of
// playerGetFeatRank__Fix (which compares the 64-bit field at entry+40); field
// meanings are not established.
// 1. For every row 0..4 and entry 0..63: byte 32 = -1, byte 33 = 1, word at 48 = 0.
// 2. Five default tables are copied into rows, entry by entry, until an entry
//    whose name string equals "END": WarriorSkillTree -> row 0, ClericSkillTree
//    -> row 1, ShadowKnightSkillTree -> row 2, RangerSkillTree -> row 3,
//    WizardSkillTree -> row 4 (executed in the order row 0, 1, 2, 4, 3). For each
//    copied entry the name is strcpy'd, then bytes 32..34, the float at 36 and
//    the 64-bit value at 40 are copied; bytes 35, 44..47 and 52..55 are left as
//    they were. The terminating entry's name is not copied; instead the first
//    4 bytes of the string "END" (including its NUL) are stored at the start of
//    the destination entry.
extern "C" {
struct SkillTreeEntry {
    char name[32];
    signed char b32, b33, b34;
    unsigned char b35;
    float f36;
    long long q40;
    int w48;
    int w52;
};
extern unsigned char gSkillTree[];
extern SkillTreeEntry WarriorSkillTree[];
extern SkillTreeEntry ClericSkillTree[];
extern SkillTreeEntry ShadowKnightSkillTree[];
extern SkillTreeEntry RangerSkillTree[];
extern SkillTreeEntry WizardSkillTree[];
char* strcpy(char* dest, const char* src);
int strcmp(const char* a, const char* b);

alignas(4) static const char endName[4] = {'E', 'N', 'D', 0};

static void copyTree(SkillTreeEntry* dest, const SkillTreeEntry* src) {
    for (int i = 0;; ++i) {
        if (strcmp(src[i].name, endName) == 0) {
            *(unsigned*)dest[i].name = *(const unsigned*)endName;
            return;
        }
        strcpy(dest[i].name, src[i].name);
        dest[i].b32 = src[i].b32;
        dest[i].b33 = src[i].b33;
        dest[i].b34 = src[i].b34;
        dest[i].f36 = src[i].f36;
        dest[i].q40 = src[i].q40;
    }
}

void playerGlobalInit__Fv() {
    for (int i = 0; i < 64; ++i) {
        for (int row = 0; row < 5; ++row) {
            SkillTreeEntry* e = (SkillTreeEntry*)(gSkillTree + row * 3592 + i * 56);
            e->b32 = -1;
            e->b33 = 1;
            e->w48 = 0;
        }
    }
    copyTree((SkillTreeEntry*)(gSkillTree + 0 * 3592), WarriorSkillTree);
    copyTree((SkillTreeEntry*)(gSkillTree + 1 * 3592), ClericSkillTree);
    copyTree((SkillTreeEntry*)(gSkillTree + 2 * 3592), ShadowKnightSkillTree);
    copyTree((SkillTreeEntry*)(gSkillTree + 4 * 3592), WizardSkillTree);
    copyTree((SkillTreeEntry*)(gSkillTree + 3 * 3592), RangerSkillTree);
}
}
