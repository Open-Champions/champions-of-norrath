// Observed addItemToInventory__FiPQ24item11descriptioni at 0x001adf38,
// 856 bytes, pinned SLUS_205.65. Arguments: player index, a pointer to a
// 72-byte item description, and a flag. Returns a slot number or -1.
// Layout, observed: the description's word 0 is a pointer to an item
// definition whose first word is a name string and whose signed byte 29 is a
// kind. The slot array for (index, kind) begins at player + index*23984 + 328 +
// kind*4616: 64 slots of 72 bytes, then a signed count word at +4608.
// Observed behaviour:
//  1. If bit 0x100 of the description's 64-bit word at byte 8 is set, slots
//     0..63 are scanned (the count is not consulted). A slot matches when word
//     0, the 64-bit word at 8, and bytes 32 and 33 equal the description's. On a
//     match, when gameDifficulty is 1 and the flag is nonzero and the item name
//     equals the recall-potion identifier, the first-recall hint runs (below).
//     The halfword at byte 4 of the slot gains the description's halfword at
//     byte 4, then playerRecomputeWeight(index) and playerGetPotionsLeft(index,0)
//     and (index,1) run, and the slot number is returned.
//  2. Otherwise, when the item name equals the identifier "heart" or "shell",
//     the first `count` slots are scanned; a nonzero slot whose item name
//     equals the new item's name makes the function return -1.
//  3. The slot at index `count` must have a zero word 0, else -1 (a full array
//     reads the count word itself). The 72 bytes are copied there. If the flag is
//     nonzero and the name is the recall-potion identifier, the first-recall
//     hint runs. playerGetPotionsLeft(index,0) and (index,1) run, the count is
//     read again, the halfword at byte 34 of that slot is set to its low half,
//     the count increases by one, and the old count is returned.
// First-recall hint: if scriptGetGameState("PLAY_firstRecall") is zero, set it to
// 1, fetch the text for the two keys "HelpfulHint" and "TutRecallUseBody" in that
// order and call hudAddHelpMessage(index, first, second).
// Inferred: names of the fields; 72-byte extent and the use of gameDifficulty.
// Not executed on hardware. Callees belong to other packages.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#endif
extern "C" {
extern unsigned char player[];
extern int gameDifficulty;
int strcmp(const char* a, const char* b);
int scriptGetGameState__FPc(char* name);
void scriptSetGameState__FPci(char* name, int value);
unsigned short* textGetString__FPc(char* key);
void hudAddHelpMessage__FiPUsT1(int index, unsigned short* first, unsigned short* second);
void playerRecomputeWeight__Fi(int index);
int playerGetPotionsLeft__Fii(int index, int which);
}

static unsigned word(const unsigned char* p) { return *reinterpret_cast<const unsigned*>(p); }
static unsigned long long dword(const unsigned char* p) { return *reinterpret_cast<const unsigned long long*>(p); }
static unsigned short half(const unsigned char* p) { return *reinterpret_cast<const unsigned short*>(p); }
static const char* itemName(unsigned item) {
    return reinterpret_cast<const char*>(NORRATH_PTR(word(NORRATH_PTR(item))));
}

static void firstRecallHint(int index) {
    char* flag = const_cast<char*>("PLAY_firstRecall");
    if (scriptGetGameState__FPc(flag) != 0) return;
    scriptSetGameState__FPci(flag, 1);
    unsigned short* first = textGetString__FPc(const_cast<char*>("HelpfulHint"));
    unsigned short* second = textGetString__FPc(const_cast<char*>("TutRecallUseBody"));
    hudAddHelpMessage__FiPUsT1(index, first, second);
}

extern "C" int addItemToInventory__FiPQ24item11descriptioni(int index, unsigned char* description, int announce) {
    unsigned item = word(description);
    unsigned long long flags = dword(description + 8);
    signed char kind = static_cast<signed char>(NORRATH_PTR(item)[29]);
    unsigned char* slots = player + index * 23984 + 328 + kind * 4616;
    if (flags & 0x100) {
        for (int i = 0; i < 64; ++i) {
            unsigned char* slot = slots + i * 72;
            if (word(slot) != item) continue;
            if (dword(slot + 8) != flags) continue;
            if (slot[32] != description[32]) continue;
            if (slot[33] != description[33]) continue;
            if (gameDifficulty == 1 && announce != 0 && strcmp(itemName(item), "recallpotion") == 0)
                firstRecallHint(index);
            *reinterpret_cast<unsigned short*>(slot + 4) =
                static_cast<unsigned short>(half(slot + 4) + half(description + 4));
            playerRecomputeWeight__Fi(index);
            playerGetPotionsLeft__Fii(index, 0);
            playerGetPotionsLeft__Fii(index, 1);
            return i;
        }
    }
    if (strcmp(itemName(word(description)), "heart") == 0 || strcmp(itemName(word(description)), "shell") == 0) {
        for (int j = 0; j < *reinterpret_cast<int*>(slots + 4608); ++j) {
            unsigned other = word(slots + j * 72);
            if (other == 0) continue;
            if (strcmp(itemName(other), itemName(word(description))) == 0) return -1;
        }
    }
    int count = *reinterpret_cast<int*>(slots + 4608);
    unsigned char* target = slots + count * 72;
    if (word(target) != 0) return -1;
    for (int offset = 0; offset < 72; offset += 8)
        *reinterpret_cast<unsigned long long*>(target + offset) = dword(description + offset);
    if (announce != 0 && strcmp(itemName(word(description)), "recallpotion") == 0) firstRecallHint(index);
    playerGetPotionsLeft__Fii(index, 0);
    playerGetPotionsLeft__Fii(index, 1);
    int n = *reinterpret_cast<int*>(slots + 4608);
    *reinterpret_cast<unsigned short*>(slots + n * 72 + 34) = static_cast<unsigned short>(n);
    *reinterpret_cast<int*>(slots + 4608) = n + 1;
    return n;
}
