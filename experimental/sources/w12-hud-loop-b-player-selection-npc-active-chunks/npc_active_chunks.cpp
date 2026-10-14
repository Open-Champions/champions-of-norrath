// Observed playerSelectionGetNPCActiveChunks__FP3NPC at 0x0027a3f8, 604 bytes: one argument (the NPC
// object), 64-bit result.
//  - The NPC's "name" tag is matched with strcmp against the first string of each of the ten
//    636-byte records of targets; i is the first match, or 10 when none matches (the loop never
//    reads past record 9).
//  - If the NPC has a "pnum" tag, hair style and beard come from playerSelectInfo (41-byte records):
//    findTagInt("pnum") is called three times; the first result selects the record whose signed
//    byte +33 is the hair style, the second result is discarded, and the third selects the record
//    whose signed byte +36 is the beard. Without a "pnum" tag, hair style is hairStyleSetting[i] and
//    beard is beardSetting[i].
//  - The result starts as all ones. For each nonzero pointer word found at these offsets of record i
//    the 64-bit value it points to is cleared from the result:
//      record + 28 + 4 * hairStyle (skipped when hairStyle is -1),
//      record + 60 + 4 * beard     (skipped when beard is -1),
//      record + 92, record + 100, record + 96, record + 104.
// Inferred: the pointed-to values are chunk masks of the attachment objects the selection screen
// draws for this character, so the result is the mask of chunks that stay active.
// Unknown: what the chunk bits stand for.
extern "C" {
extern signed char playerSelectInfo[];
extern signed char hairStyleSetting[];
extern signed char beardSetting[];
extern unsigned char targets[];
char* findTagString__10GameObjectPci(void* self, const char* tag, int fallback);
int findTagInt__10GameObjectPci(void* self, const char* tag, int fallback);
int strcmp(const char* a, const char* b);
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline __UINTPTR_TYPE__ pointerOf(unsigned word) {
#ifdef W12_HUD_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
inline unsigned wordAt(const unsigned char* base, int offset) {
    unsigned word;
    __builtin_memcpy(&word, base + offset, 4);
    return word;
}
// Clears the 64-bit value pointed to by the word at base + offset from mask, when the word is set.
inline void clearChunks(unsigned long long& mask, const unsigned char* base, int offset) {
    const unsigned word = wordAt(base, offset);
    if (word == 0) return;
    unsigned long long value;
    __builtin_memcpy(&value, reinterpret_cast<const void*>(pointerOf(word)), 8);
    mask &= ~value;
}
}

extern "C" unsigned long long playerSelectionGetNPCActiveChunks__FP3NPC(void* npc) {
    unsigned long long mask = ~0ull;
    const char* name = findTagString__10GameObjectPci(npc, "name", 0);
    int i = 0;
    while (i < 10 && strcmp(reinterpret_cast<const char*>(pointerOf(wordAt(targets, i * 636))), name) != 0) ++i;
    int hair, beard;
    if (findTagString__10GameObjectPci(npc, "pnum", 0) != nullptr) {
        hair = playerSelectInfo[findTagInt__10GameObjectPci(npc, "pnum", 0) * 41 + 33];
        findTagInt__10GameObjectPci(npc, "pnum", 0);
        beard = playerSelectInfo[findTagInt__10GameObjectPci(npc, "pnum", 0) * 41 + 36];
    } else {
        hair = hairStyleSetting[i];
        beard = beardSetting[i];
    }
    const unsigned char* record = targets + i * 636;
    if (hair != -1) clearChunks(mask, record, 28 + hair * 4);
    if (beard != -1) clearChunks(mask, record, 60 + beard * 4);
    clearChunks(mask, record, 92);
    clearChunks(mask, record, 100);
    clearChunks(mask, record, 96);
    clearChunks(mask, record, 104);
    return mask;
}
