// Observed in the pinned SLUS_205.65: five initialised data objects, each a table of
// 56-byte entries ending with an entry named "END" (all fields zero):
//   WarriorSkillTree 0x0041fce8, 728 bytes (13 entries)
//   ClericSkillTree  0x0041ffc0, 504 bytes (9 entries)
//   WizardSkillTree  0x004201b8, 504 bytes (9 entries)
//   RangerSkillTree  0x004203b0, 336 bytes (6 entries)
//   ShadowKnightSkillTree 0x00420500, 448 bytes (8 entries)
// Entry layout (from the image bytes and from playerGlobalInit__Fv, which copies
// these tables into gSkillTree): a 32-byte NUL-padded name, three signed bytes at
// +32..+34, one byte at +35, a float at +36, a 64-bit value at +40, and two words
// at +48 and +52. The values below are the observed initial bytes. The 64-bit
// value in every populated entry has a single bit set. Field meanings and the
// units of the float are not established here.
// The image gives the five objects no relocations (no pointers inside them).
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
#if defined(__mips__) || defined(__x86_64__) || defined(__aarch64__)
static_assert(sizeof(SkillTreeEntry) == 56, "skill tree entry stride");
#endif

SkillTreeEntry WarriorSkillTree[13] = {
    {"Blunt", 0, 20, 1, 0, 0.0f, 33554432LL, 0, 0},
    {"Slashing", 0, 20, 1, 0, 0.0f, 67108864LL, 0, 0},
    {"Slam", 0, 20, 1, 0, 0.0f, 131072LL, 0, 0},
    {"Endurance", 0, 20, 1, 0, 0.0f, 64LL, 0, 0},
    {"BindWound", 0, 5, 1, 0, 0.0f, 16LL, 0, 0},
    {"DualWield", 0, 20, 1, 0, 0.0f, 262144LL, 0, 0},
    {"GroundPound", 0, 20, 0, 0, 0.0f, 1024LL, 0, 0},
    {"bullrush", 0, 20, 3, 0, 5.0f, 8LL, 0, 0},
    {"Evade", 0, 1, 1, 0, 0.0f, 524288LL, 0, 0},
    {"MuleKick", 0, 1, 1, 0, 0.0f, 1048576LL, 0, 0},
    {"DeadEye", 0, 1, 5, 0, 0.0f, 2097152LL, 0, 0},
    {"DualWield", 0, 1, 1, 0, 0.0f, 262144LL, 0, 0},
    {"END", 0, 0, 0, 0, 0.0f, 0LL, 0, 0},
};
SkillTreeEntry ClericSkillTree[9] = {
    {"Blunt", 0, 20, 1, 0, 0.0f, 33554432LL, 0, 0},
    {"Root", 0, 1, 1, 0, 0.0f, 16777216LL, 0, 0},
    {"Resurrection", 0, 1, 1, 0, 0.0f, 4194304LL, 0, 0},
    {"Gate", 0, 1, 1, 0, 0.0f, 8388608LL, 0, 0},
    {"Endurance", 0, 5, 2, 0, 0.0f, 64LL, 0, 0},
    {"BindWound", 0, 5, 1, 0, 0.0f, 16LL, 0, 0},
    {"Meditation", 0, 5, 1, 0, 0.0f, 512LL, 0, 0},
    {"DeadEye", 0, 1, 5, 0, 0.0f, 2097152LL, 0, 0},
    {"END", 0, 0, 0, 0, 0.0f, 0LL, 0, 0},
};
SkillTreeEntry ShadowKnightSkillTree[8] = {
    {"Blunt", 0, 20, 1, 0, 0.0f, 33554432LL, 0, 0},
    {"Slashing", 0, 20, 1, 0, 0.0f, 67108864LL, 0, 0},
    {"DualWield", 0, 1, 1, 0, 0.0f, 262144LL, 0, 0},
    {"Endurance", 0, 5, 2, 0, 0.0f, 64LL, 0, 0},
    {"BindWound", 0, 5, 1, 0, 0.0f, 16LL, 0, 0},
    {"Meditation", 0, 5, 1, 0, 0.0f, 512LL, 0, 0},
    {"DeadEye", 0, 1, 5, 0, 0.0f, 2097152LL, 0, 0},
    {"END", 0, 0, 0, 0, 0.0f, 0LL, 0, 0},
};
SkillTreeEntry RangerSkillTree[6] = {
    {"Blunt", 0, 20, 1, 0, 0.0f, 33554432LL, 0, 0},
    {"Slashing", 0, 20, 1, 0, 0.0f, 67108864LL, 0, 0},
    {"Root", 0, 1, 1, 0, 0.0f, 16777216LL, 0, 0},
    {"DualWield", 0, 1, 1, 0, 0.0f, 262144LL, 0, 0},
    {"Endurance", 0, 5, 2, 0, 0.0f, 64LL, 0, 0},
    {"END", 0, 0, 0, 0, 0.0f, 0LL, 0, 0},
};
SkillTreeEntry WizardSkillTree[9] = {
    {"Blunt", 0, 20, 1, 0, 0.0f, 33554432LL, 0, 0},
    {"shock", 0, 20, 16, 0, 0.0f, 2048LL, 0, 0},
    {"BindWound", 0, 5, 1, 0, 0.0f, 16LL, 0, 0},
    {"Root", 0, 1, 1, 0, 0.0f, 16777216LL, 0, 0},
    {"Gate", 0, 1, 1, 0, 0.0f, 8388608LL, 0, 0},
    {"Endurance", 0, 5, 2, 0, 0.0f, 64LL, 0, 0},
    {"Meditation", 0, 5, 1, 0, 0.0f, 512LL, 0, 0},
    {"Root", 0, 1, 1, 0, 0.0f, 16777216LL, 0, 0},
    {"END", 0, 0, 0, 0, 0.0f, 0LL, 0, 0},
};
}
