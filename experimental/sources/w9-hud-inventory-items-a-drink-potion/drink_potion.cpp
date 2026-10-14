// Observed drinkPotion__6Playeriiii at 0x001a8358, 2088 bytes, in the pinned SLUS_205.65.
// this (a Player), kind, and three more ints that the first branch passes unchanged to
// msgInventoryGridDropItem__Fiiciiii (here named page, cell and extra).
// Per-player record rec = player + this[960] * 23984 (this[960] is the player index). The
// floats used: rec[172] current, rec[176] maximum, rec[180] pending (all one stat, the
// first, drunk by kind 0 and indexed to the first potion list), and rec[196], rec[200],
// rec[204] the same triple for the second stat (kind 1 and above). The function first
// refreshes rec[184] = min(0.49999997, rec[176] * 0.0020833332) and rec[188] =
// min(0.49999997, rec[200] * 0.0020833332) with the EE min.s, and forms
// first = rec[172] + rec[180] and second = rec[196] + rec[204].
// Early returns: this[12] & 0x20000 set; rec[172] <= 0.
// 1. For kind >= 2: when scriptGetGameState("PLAY_firstDrink") is 0, set it to 1 and show a
//    help message with textGetString("HelpfulHint") and textGetString("TutPotionUseBody")
//    through multiHudAddHelpMessage__FiPUsT1i(this[960], first text, second text, 0).
// 2. kind == 0: return unless rec[176] > first. Otherwise an inventory scan: for each of
//    nine names of the first potion list (a table of string pointers, "leshealingpotion"
//    ... "fullhealpotion") the last row i < rec[14168] of the 72-byte rows at rec+9560
//    whose item's name (the string pointed to by the first word of the row's first word)
//    equals the name gives found[k] = i, else -1. The target is
//    trunc(rec[176] - first + 1.0) / 60, limited to 8. If found[target] is not negative it is
//    chosen, else the highest k with found[k] >= 0, else none (-1).
//    No potion: fadeTimer[player*7 + 1] = 120, fadeIconY[player*7 + 1] =
//    hudGetFadeIconPos__Fii(player, 1), return.
//    Otherwise pending += (chosen + 1) * 60 and, when rec[172] + pending would pass rec[176],
//    pending = rec[176] - rec[172] (rec[180]).
//  kind >= 1: the same with the second list ("lesrejuvpotion" ... "fullrejuvpotion"), the
//    second triple (rec[200] must exceed second), the chosen slot taken as 7 - kind for
//    kind != 1 (not range checked and not checked against found, as in the image) and by
//    the same rule as above for kind == 1; no potion uses index 2 instead of 1.
//    Pending is rec[204].
// 3. Both: lumpFindResource__FPcT0("fx.lmp", "spel_cas_curelw.anm") then
//    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(result, *(rec+21872), 1.0f).
//    When the signed byte interfaceOn is nonzero, netPlayerIsLocal__Fi(player) is nonzero
//    and the word invGrid is nonzero: with cellRow = invGrid + player * 0x23988 + 0x17bb0 +
//    page * 5400 + cell * 72, msgInventoryGridDropItem__Fiiciiii(player, halfword at
//    cellRow+34, byte at (word at cellRow)+29, page, cell, extra, 2). Otherwise the halfword
//    at rec[9564 + 72 * found[chosen]] is decremented, and when it becomes zero
//    removeItemFromInventory__Fiiii(player, found[chosen], 2, 0) is called.
//    Then playerGetPotionsLeft__Fii(player, kind-list 0 or 1): a result from 1 to 5 sets
//    fadeTimer[player*7 + 4 (list 0) or 5 (list 1)] = 240 and the matching fadeIconY entry to
//    hudGetFadeIconPos__Fii(player, 4 or 5). Finally playerRecomputeWeight__Fi(player).
// The image also holds a branch for "kind == 0" in the second list's code and "kind != 0"
// in the first's; both are unreachable and not reproduced.
// Unknown: what the kinds above 1 stand for, the 5400 and 72 byte row sizes' names, and the
// meaning of the rec fields beyond the arithmetic.
// Not executed on hardware; callees and objects are defined elsewhere.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#endif
extern "C" {
extern unsigned char player[];
extern signed char interfaceOn;
extern int invGrid;
extern int fadeTimer[];
extern int fadeIconY[];
int scriptGetGameState__FPc(const char* key);
void scriptSetGameState__FPci(const char* key, int value);
unsigned short* textGetString__FPc(const char* key);
void multiHudAddHelpMessage__FiPUsT1i(int player, unsigned short* first, unsigned short* second, int flag);
int strcmp(const char* a, const char* b);
int hudGetFadeIconPos__Fii(int player, int slot);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
int netPlayerIsLocal__Fi(int player);
void msgInventoryGridDropItem__Fiiciiii(int player, int half, char kind, int page, int cell, int extra, int two);
void removeItemFromInventory__Fiiii(int player, int index, int two, int zero);
int playerGetPotionsLeft__Fii(int player, int list);
void playerRecomputeWeight__Fi(int player);
}

static const char* const firstList[10] = {
    "leshealingpotion", "smallhealpotion", "minorhealpotion", "healingpotion", "largehealpotion",
    "greathealpotion", "exhealingpotion", "gianthealpotion", "fullhealpotion", 0};
static const char* const secondList[10] = {
    "lesrejuvpotion", "smallrejuvpotion", "minorrejuvpotion", "rejuvpotion", "largerejuvpotion",
    "greatrejuvpotion", "exrejuvpotion", "giantrejuvpotion", "fullrejuvpotion", 0};

static inline float single_from_bits(unsigned bits) {
    union { unsigned u; float f; } v;
    v.u = bits;
    return v.f;
}

static inline float minSingle(float a, float b) {
#if defined(__mips__)
    float result;
    __asm__("min.s %0,%1,%2" : "=f"(result) : "f"(a), "f"(b));
    return result;
#else
    return a < b ? a : b;
#endif
}

static inline float& field(unsigned char* rec, int offset) { return *reinterpret_cast<float*>(rec + offset); }

static void findPotions(unsigned char* rec, const char* const* names, int* found) {
    for (int k = 0; k < 9; ++k)
        found[k] = -1;
    for (int k = 0; k < 9; ++k) {
        for (int i = 0; i < *reinterpret_cast<int*>(rec + 14168); ++i) {
            const unsigned itemWord = *reinterpret_cast<unsigned*>(rec + 9560 + 72 * i);
            const unsigned nameWord = *reinterpret_cast<unsigned*>(NORRATH_PTR(itemWord));
            if (strcmp(reinterpret_cast<const char*>(NORRATH_PTR(nameWord)), names[k]) == 0)
                found[k] = i;
        }
    }
}

static int chooseByNeed(const int* found, float missing) {
    int target = static_cast<int>(missing) / 60;
    if (target >= 9)
        target = 8;
    if (found[target] >= 0)
        return target;
    for (int k = 8; k >= 0; --k)
        if (found[k] >= 0)
            return k;
    return -1;
}

static void noPotion(unsigned char* self, int slot) {
    const int index = *reinterpret_cast<int*>(self + 960);
    fadeTimer[index * 7 + slot] = 120;
    fadeIconY[index * 7 + slot] = hudGetFadeIconPos__Fii(index, slot);
}

static void drinkChosen(unsigned char* self, unsigned char* rec, int foundIndex, int page, int cell, int extra) {
    const int index = *reinterpret_cast<int*>(self + 960);
    void* header = lumpFindResource__FPcT0("fx.lmp", "spel_cas_curelw.anm");
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, NORRATH_PTR(*reinterpret_cast<unsigned*>(rec + 21872)), 1.0f);
    if (interfaceOn != 0 && netPlayerIsLocal__Fi(index) != 0 && invGrid != 0) {
        unsigned char* row = NORRATH_PTR(static_cast<unsigned>(invGrid)) +
                             index * 0x23988 + 0x17bb0 + page * 5400 + cell * 72;
        unsigned char* item = NORRATH_PTR(*reinterpret_cast<unsigned*>(row));
        msgInventoryGridDropItem__Fiiciiii(index, *reinterpret_cast<short*>(row + 34), static_cast<char>(item[29]),
                                           page, cell, extra, 2);
    } else {
        unsigned short* count = reinterpret_cast<unsigned short*>(rec + 9564 + 72 * foundIndex);
        *count = static_cast<unsigned short>(*count - 1);
        if (*count == 0)
            removeItemFromInventory__Fiiii(index, foundIndex, 2, 0);
    }
}

static void finishList(unsigned char* self, int list) {
    const int index = *reinterpret_cast<int*>(self + 960);
    const int left = playerGetPotionsLeft__Fii(index, list);
    if (static_cast<unsigned>(left - 1) < 5u) {
        fadeTimer[index * 7 + 4 + list] = 240;
        fadeIconY[index * 7 + 4 + list] = hudGetFadeIconPos__Fii(index, 4 + list);
    }
}

extern "C" void drinkPotion__6Playeriiii(unsigned char* self, int kind, int page, int cell, int extra) {
    if ((*reinterpret_cast<unsigned*>(self + 12) & 0x20000) != 0)
        return;
    const int index = *reinterpret_cast<int*>(self + 960);
    unsigned char* rec = player + index * 23984;
    if (field(rec, 172) <= 0.0f)
        return;
    const float scale = single_from_bits(0x3b088888);
    const float cap = single_from_bits(0x3effffff);
    field(rec, 184) = minSingle(cap, field(rec, 176) * scale);
    field(rec, 188) = minSingle(cap, field(rec, 200) * scale);
    const float first = field(rec, 172) + field(rec, 180);
    const float second = field(rec, 196) + field(rec, 204);
    if (kind >= 2) {
        if (scriptGetGameState__FPc("PLAY_firstDrink") == 0) {
            scriptSetGameState__FPci("PLAY_firstDrink", 1);
            unsigned short* body = textGetString__FPc("HelpfulHint");
            unsigned short* title = textGetString__FPc("TutPotionUseBody");
            multiHudAddHelpMessage__FiPUsT1i(index, body, title, 0);
        }
    }
    if (kind == 0) {
        if (field(rec, 176) <= first)
            return;
        int found[9];
        findPotions(rec, firstList, found);
        const int chosen = chooseByNeed(found, field(rec, 176) - first + 1.0f);
        if (chosen == -1) {
            noPotion(self, 1);
            return;
        }
        float pending = field(rec, 180) + static_cast<float>(chosen + 1) * 60.0f;
        const float limit = field(rec, 176);
        if (limit < field(rec, 172) + pending)
            pending = limit - field(rec, 172);
        field(rec, 180) = pending;
        drinkChosen(self, rec, found[chosen], page, cell, extra);
        finishList(self, 0);
    } else {
        if (field(rec, 200) <= second)
            return;
        int found[9];
        findPotions(rec, secondList, found);
        int chosen;
        if (kind != 1)
            chosen = 7 - kind;
        else
            chosen = chooseByNeed(found, field(rec, 200) - second + 1.0f);
        if (chosen == -1) {
            noPotion(self, 2);
            return;
        }
        float pending = field(rec, 204) + static_cast<float>(chosen + 1) * 60.0f;
        const float limit = field(rec, 200);
        if (limit < field(rec, 196) + pending)
            pending = limit - field(rec, 196);
        field(rec, 204) = pending;
        drinkChosen(self, rec, found[chosen], page, cell, extra);
        finishList(self, 1);
    }
    playerRecomputeWeight__Fi(index);
}
