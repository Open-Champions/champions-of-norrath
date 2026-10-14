// Observed equipItem__FiPQ24item11descriptioni at 0x001aec78, 1836 bytes,
// pinned SLUS_205.65. Arguments: player index, a pointer to an item
// description, and a quiet flag. Notation: P = player + index*23984;
// item = the description's word 0 (pointer to an item definition);
// type = signed byte 28 of the item, hand = signed byte 29, sub = byte 30.
// Records at P, observed: A[t] = word at P+21896+4*t (the equipped
// description of each type), B[n] = word at P+21960+4*n, an extra word at
// P+21968 (B[2]), bytes at P+23572 and P+23582.
// Observed flow:
//  1. Return when the item word is zero or type is 15.
//  2. inventoryCheckWieldingStatus(index, desc) nonzero => refusal: if
//     netPlayerIsLocal, play the "cant do" animation from the lump "fx.lmp";
//     then show a help message made from the text key "ElfNoBigTitle" and the
//     key at position `status` of a nine-entry table of text keys (entry 0 is
//     the word 0x00484848 and is not used); the last argument is 0 when the
//     word at inventory+56 equals index, else whether the word at inventory+172
//     does. Return.
//  3. Choose slot: -1; for hand 0 and type != 8: sub 4 or 6 => 1, run script
//     "*bowEquipNotify", set game state "bowEquipped" to 1; else slot 0, run
//     script "*weaponEquipNotify". Type 3 => slot 0 and a nonzero B[0] is
//     unequipped by a recursive call with flag 0.
//  4. A slot other than -1 is stored at P+23572. For hand 0 the two words
//     B[0], B[1] equal to desc are cleared; for type 0 with feat rank
//     (0x40000) positive and B[0] a type-0 description, A[0] is re-equipped
//     when it differs and P+21968 receives desc, else B[slot] receives desc.
//  5. If A[type] is not already desc: hand 0 stores sub at P+23582 and
//     handles type 2 (clear A[0], A[1], A[2]; for sub other than 4/6 also clear
//     P+21968 and A[3]) or type 0 (clear A[2], A[0] when A[2] is a hand-0
//     item; with feat rank positive and A[0] set: clear A[3], P+23582 = 7,
//     target A[1]). Unless type is 8 it then calls resetPlayerAnimations and
//     inventorySetAnim. Hand 1 with type 3 may clear A[2] and B[stored slot]
//     (P+23582 = 5) or clear P+21968 and A[1] (P+23582 = 0), each followed
//     by the reset and animation calls. Hand 2 with type 5 retargets to A[6]
//     when A[5] is set and A[6] is clear. Finally A[target] = desc.
//  6. With a zero flag, a sound name is chosen from the 64-bit flags at
//     desc+8 and the item (a chain of tests on 0x800000, 0x200000, type 3,
//     0x400000, 0x40000, 0x100000, 0x80000, 0x1000000 and others, with sub
//     ranges); the name is copied by sprintf and played when
//     netPlayerIsLocal.
//  7. Always inventorySetAnim(index, animation record, 0) and
//     playerRecomputeAbilityModifiers().
// Inferred: field names. The per-player animation record array (264-byte
// records at 0x7cc5c8, a local symbol "animState" of the inventory unit) is
// imported as norrath_inventory_anim_state_view. Not executed on hardware.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#define NORRATH_WORD(p) static_cast<unsigned>(reinterpret_cast<unsigned long>(p))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#define NORRATH_WORD(p) reinterpret_cast<unsigned>(p)
#endif
extern "C" {
extern unsigned char player[];
extern unsigned char inventory[];
extern unsigned char norrath_inventory_anim_state_view[];
int inventoryCheckWieldingStatus__FiPQ24item11description(int index, void* description);
int netPlayerIsLocal__Fi(int index);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
unsigned short* textGetString__FPc(char* key);
void multiHudAddHelpMessage__FiPUsT1i(int index, unsigned short* first, unsigned short* second, int which);
void scriptStart__FPc(char* name);
void scriptSetGameState__FPci(char* name, int value);
long long playerGetFeatRank__Fix(int index, long long feat);
void resetPlayerAnimations__Fi(int index);
void inventorySetAnim__FiP14AnimationStatei(int index, void* record, int mode);
void playerRecomputeAbilityModifiers__Fv();
int sprintf(char* buffer, const char* format, ...);
void equipItem__FiPQ24item11descriptioni(int index, unsigned char* description, int quiet);
}

static unsigned word(const unsigned char* p) { return *reinterpret_cast<const unsigned*>(p); }
static void put(unsigned char* p, unsigned value) { *reinterpret_cast<unsigned*>(p) = value; }
static unsigned char* at(unsigned w) { return NORRATH_PTR(w); }
static int type(unsigned item) { return static_cast<signed char>(at(item)[28]); }
static int hand(unsigned item) { return static_cast<signed char>(at(item)[29]); }
static int sub(unsigned item) { return static_cast<signed char>(at(item)[30]); }

static void animate(int index) {
    resetPlayerAnimations__Fi(index);
    inventorySetAnim__FiP14AnimationStatei(index, norrath_inventory_anim_state_view + index * 264, 0);
}

static const char* soundName(unsigned long long flags, unsigned item) {
    if (flags & 0x800000) {
        if (static_cast<unsigned>(static_cast<unsigned char>(sub(item)) - 1) < 4) return "inv_equip_lgwp_wd.anm";
        if (flags & 0x20000000) return "inv_equip_smwp_wd.anm";
        return "inv_equip_mdwp_wd.anm";
    }
    if (flags & 0x200000) return "inv_equip_chain.anm";
    if (type(item) == 3) return "inv_equip_sm_shield.anm";
    if (flags & 0x400000) return "inv_equip_plate.anm";
    if (flags & 0x40000) {
        if (static_cast<unsigned>(static_cast<unsigned char>(sub(item)) - 1) < 3) return "inv_equip_lgwp_mtl.anm";
        if (flags & 0x20000000) return "inv_equip_smwp_mtl.anm";
        return "inv_equip_mdwp_mtl.anm";
    }
    if (flags & 0x100000) return "inv_equip_leather.anm";
    if (flags & 0x80000) return "inv_equip_cloth.anm";
    if (flags & 0x1000000) return "inv_equip_potion.anm";
    return "inv_equip_jewelry.anm";
}

extern "C" void equipItem__FiPQ24item11descriptioni(int index, unsigned char* description, int quiet) {
    unsigned char* P = player + index * 23984;
    const unsigned self = NORRATH_WORD(description);
    unsigned item = word(description);
    if (item == 0) return;
    if (type(item) == 15) return;
    int status = inventoryCheckWieldingStatus__FiPQ24item11description(index, description);
    if (status != 0) {
        if (netPlayerIsLocal__Fi(index) != 0)
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("game_butn_cantdo.anm")), nullptr, 1.0f);
        const char* reasons[9] = {reinterpret_cast<const char*>(static_cast<unsigned long>(0x484848)),
                                  "NoWieldWrongClass", "NoWieldLowLevel", "NoWieldLowStr", "NoWieldLowInt",
                                  "NoWieldLowDex", "NoWieldLowSta", "NoWieldLowBlunt", "NoWieldLowSlashing"};
        int which = 0;
        if (static_cast<int>(word(inventory + 56)) != index) which = (static_cast<int>(word(inventory + 172)) == index);
        unsigned short* first = textGetString__FPc(const_cast<char*>("ElfNoBigTitle"));
        unsigned short* second = textGetString__FPc(const_cast<char*>(reasons[status]));
        multiHudAddHelpMessage__FiPUsT1i(index, first, second, which);
        return;
    }
    item = word(description);
    int slot = -1;
    if (hand(item) == 0 && type(item) != 8) {
        int s = sub(item);
        if (s == 4 || s == 6) {
            slot = 1;
            scriptStart__FPc(const_cast<char*>("*bowEquipNotify"));
            scriptSetGameState__FPci(const_cast<char*>("bowEquipped"), 1);
        } else {
            slot = 0;
            scriptStart__FPc(const_cast<char*>("*weaponEquipNotify"));
        }
    }
    item = word(description);
    if (type(item) == 3) {
        slot = 0;
        unsigned held = word(P + 21960);
        if (held != 0) equipItem__FiPQ24item11descriptioni(index, at(held), 0);
    }
    if (slot != -1) {
        P[23572] = static_cast<unsigned char>(slot);
        item = word(description);
        if (hand(item) == 0) {
            for (int n = 0; n < 2; ++n)
                if (word(P + 21960 + n * 4) == self) put(P + 21960 + n * 4, 0);
            bool combined = false;
            if (type(item) == 0 && playerGetFeatRank__Fix(index, 0x40000) > 0) {
                unsigned held = word(P + 21960);
                if (held != 0 && type(word(at(held))) == 0) {
                    if (word(P + 21896) != held) equipItem__FiPQ24item11descriptioni(index, at(held), 0);
                    put(P + 21968, self);
                    combined = true;
                }
            }
            if (!combined) put(P + 21960 + slot * 4, self);
        }
    }
    item = word(description);
    int kind = type(item);
    int target = kind * 4;
    if (word(P + 21896 + kind * 4) != self) {
        if (hand(item) == 0) {
            P[23582] = static_cast<unsigned char>(sub(item));
            item = word(description);
            if (type(item) == 2) {
                put(P + 21904, 0);
                put(P + 21896, 0);
                put(P + 21900, 0);
                int s = sub(item);
                if (s != 4 && s != 6) {
                    put(P + 21968, 0);
                    put(P + 21908, 0);
                }
            } else if (type(item) == 0) {
                unsigned second = word(P + 21904);
                if (second != 0 && hand(word(at(second))) == 0) {
                    put(P + 21904, 0);
                    put(P + 21896, 0);
                }
                if (playerGetFeatRank__Fix(index, 0x40000) > 0 && word(P + 21896) != 0) {
                    put(P + 21908, 0);
                    P[23582] = 7;
                    target = 4;
                }
                item = word(description);
            }
            if (type(word(description)) != 8) {
                animate(index);
            }
        } else if (hand(item) == 1) {
            if (kind == 3) {
                unsigned second = word(P + 21904);
                if (second != 0 && hand(word(at(second))) == 0) {
                    int stored = static_cast<signed char>(P[23572]);
                    put(P + 21904, 0);
                    put(P + 21960 + stored * 4, 0);
                    P[23582] = 5;
                    animate(index);
                } else if (word(P + 21968) != 0) {
                    P[23582] = 0;
                    put(P + 21968, 0);
                    put(P + 21900, 0);
                    animate(index);
                }
            }
        } else if (hand(item) == 2) {
            if (kind == 5 && word(P + 21916) != 0) {
                int retarget = word(P + 21920) == 0 ? 6 : kind;
                target = retarget * 4;
            }
        }
        put(P + 21896 + target, self);
    }
    if (quiet == 0) {
        unsigned long long flags = *reinterpret_cast<unsigned long long*>(description + 8);
        char buffer[64];
        sprintf(buffer, soundName(flags, word(description)));
        if (netPlayerIsLocal__Fi(index) != 0)
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), buffer), nullptr, 1.0f);
    }
    inventorySetAnim__FiP14AnimationStatei(index, norrath_inventory_anim_state_view + index * 264, 0);
    playerRecomputeAbilityModifiers__Fv();
}
