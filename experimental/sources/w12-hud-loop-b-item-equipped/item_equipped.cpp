// Observed itemEquipped__FiPQ24item11description at 0x001b0440, 200 bytes. Player records are 23984
// bytes apart starting at player. A null item, or an item whose first word (a description pointer)
// is null, gives 0. With a description:
//   - if the description byte at +29 is nonzero, or the byte at +28 equals 8, the item is equipped
//     when it equals one of the sixteen 4-byte words at record offset 21896;
//   - otherwise it is equipped when it equals one of the three words at record offset 21960.
// Result is 1 or 0. No memory is written.
// Inferred: 21896 is the carried-item slot array (see inventoryItemCheckSlot, same range), and the
// three words at 21960 are equipment slots.
// Unknown: the meaning of description bytes +28 and +29 beyond these tests.
extern "C" {
extern unsigned char player[];
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline int readWord(const unsigned char* p) {
    int v;
    __builtin_memcpy(&v, p, 4);
    return v;
}
inline const unsigned char* pointerOf(int word) {
#ifdef W12_HUD_HOST
    return reinterpret_cast<const unsigned char*>(hostWordAddress(static_cast<unsigned>(word)));
#else
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline bool anyEqual(const unsigned char* words, int count, int wanted) {
    for (int i = 0; i < count; ++i)
        if (readWord(words + i * 4) == wanted) return true;
    return false;
}
}

extern "C" int itemEquipped__FiPQ24item11description(int playerIndex, void* item) {
    const unsigned char* record = player + playerIndex * 23984;
    if (item == nullptr) return 0;
    const unsigned char* description = pointerOf(readWord(static_cast<const unsigned char*>(item)));
    if (description == nullptr) return 0;
    const int wanted = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(item));
    if (static_cast<signed char>(description[29]) != 0 || static_cast<signed char>(description[28]) == 8)
        return anyEqual(record + 21896, 16, wanted) ? 1 : 0;
    return anyEqual(record + 21960, 3, wanted) ? 1 : 0;
}
