// Observed inventorySetAnim__FiP14AnimationStatei at 0x001b01b0, 648 bytes.
// Arguments: a player index, an AnimationState pointer, and an integer flag.
// Player records are 23984 bytes apart at `player` (0x00408600). Fields used:
//   record+21872 (word)  nothing happens when it is zero (meaning not established)
//   record+84 (word)     selects a lump name; values 10 and above do nothing
//   record+23582 (byte)  selects an idle animation resource name
// Lump names (type -> name): 0 ragnar2.lmp, 1 sonya2.lmp, 2 elf_mwo2.lmp,
// 3 elf_fwo2.lmp, 4 hem_high.lmp, 5 elf_fhi2.lmp, 6 eru_male.lmp, 7 eru_fem.lmp,
// 8 elf_mdk.lmp, 9 elf_fdk2.lmp. The name is copied to a stack buffer, as the
// original does, and passed to lumpFindResource__FPcT0 together with the resource
// name chosen by the byte (1 dhs_idleb.anm, 2 dha_idleb.anm, 3 l_idleb.anm,
// 4 b_idleb.anm, 5 nw_idleb.anm, 7 dw_idleb.anm, anything else ohw_idleb.anm).
// The strings are rodata of the original; the type/state meanings are inferred
// only from the strings. If animGetAnimComponent__FP14AnimationStateP15AnimationHeader
// (state, header) returns nonzero nothing more happens. Otherwise the resource is
// looked up a second time and animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi
// (state, header, rate, 67) is called, where rate is 0.3 (the float constant at 0x00480598,
// read gp-relative) or 0.0 when the integer flag is nonzero.
// The meaning of 67 is not established. The callees and objects are not defined here.
extern "C" {
extern unsigned char player[];
void* lumpFindResource__FPcT0(char* lump, char* name);
void* animGetAnimComponent__FP14AnimationStateP15AnimationHeader(void* state, void* header);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
}

static const char* const lumpNames[10] = {
    "ragnar2.lmp", "sonya2.lmp",   "elf_mwo2.lmp", "elf_fwo2.lmp", "hem_high.lmp",
    "elf_fhi2.lmp", "eru_male.lmp", "eru_fem.lmp",  "elf_mdk.lmp",  "elf_fdk2.lmp",
};

extern "C" void inventorySetAnim__FiP14AnimationStatei(int playerIndex, void* state, int noBlend) {
    unsigned char* record = player + playerIndex * 23984;
    if (*reinterpret_cast<int*>(record + 21872) == 0) return;
    unsigned type = *reinterpret_cast<unsigned*>(record + 84);
    if (type >= 10) return;
    char lump[16];
    const char* src = lumpNames[type];
    int i = 0;
    do { lump[i] = src[i]; } while (src[i++] != 0);
    float rate = 0.3f;
    if (noBlend != 0) rate = 0.0f;
    const char* resource;
    switch (static_cast<signed char>(record[23582])) {
    case 1: resource = "dhs_idleb.anm"; break;
    case 2: resource = "dha_idleb.anm"; break;
    case 3: resource = "l_idleb.anm"; break;
    case 4: resource = "b_idleb.anm"; break;
    case 5: resource = "nw_idleb.anm"; break;
    case 7: resource = "dw_idleb.anm"; break;
    default: resource = "ohw_idleb.anm"; break;
    }
    if (animGetAnimComponent__FP14AnimationStateP15AnimationHeader(
            state, lumpFindResource__FPcT0(lump, const_cast<char*>(resource))) != nullptr) return;
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
        state, lumpFindResource__FPcT0(lump, const_cast<char*>(resource)), rate, 67);
}
