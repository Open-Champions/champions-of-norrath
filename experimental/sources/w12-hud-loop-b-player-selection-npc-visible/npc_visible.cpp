// Observed playerSelectionNPCVisible__FP3NPC at 0x00279f48, 360 bytes: one argument (the NPC object),
// result 1 when the NPC should be drawn and 0 otherwise.
//  - selectMode 7 or 8 returns 1 at once and changes nothing.
//  - The NPC's "name" tag string (GameObject::findTagString, default 0) is matched with strcmp against
//    the first string of each of the ten 636-byte records of targets; index i is the first match, or
//    10 when none matches.
//  - The 64-bit word at NPC + 160 loses bit 0x200000. That bit is set again when i is 8 and the
//    signed byte at playerSelectInfo + curPlayer * 41 + 32 is below 8, or when the float delta is
//    nonzero, src is 7, dst is 8 and i is 9.
//  - Result: 1 when i is 10, otherwise 1 when i - byte + 1 is below 3 as an unsigned number (that is
//    i within -1..1 of the byte), else 0.
// Inferred: targets indices pair up as male/female of five races; the selection byte at +32 is the
// currently selected character index of the player slot curPlayer.
// Unknown: the meaning of bit 0x200000 and of delta, src and dst in this test.
extern "C" {
extern int selectMode;
extern int curPlayer;
extern int src;
extern int dst;
extern float delta;
extern signed char playerSelectInfo[];
extern unsigned char targets[];
char* findTagString__10GameObjectPci(void* self, const char* tag, int fallback);
int strcmp(const char* a, const char* b);
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline const char* targetName(int i) {
    unsigned word;
    __builtin_memcpy(&word, targets + i * 636, 4);
#ifdef W12_HUD_HOST
    return reinterpret_cast<const char*>(hostWordAddress(word));
#else
    return reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
}

extern "C" int playerSelectionNPCVisible__FP3NPC(void* npc) {
    if (static_cast<unsigned>(selectMode - 7) < 2u) return 1;
    const char* name = findTagString__10GameObjectPci(npc, "name", 0);
    int i = 0;
    while (i < 10 && strcmp(targetName(i), name) != 0) ++i;
    unsigned long long* flags = reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(npc) + 160);
    *flags &= ~0x200000ull;
    const signed char* info = reinterpret_cast<const signed char*>(playerSelectInfo) + curPlayer * 41;
    if (i == 8 && info[32] < 8) *flags |= 0x200000ull;
    if (delta != 0.0f && src == 7 && dst == 8 && i == 9) *flags |= 0x200000ull;
    if (i == 10) return 1;
    return static_cast<unsigned>(i - info[32] + 1) < 3u ? 1 : 0;
}
