// Observed inventoryItemCheckSlot__FiPQ24item11description at 0x001b1d20, 84 bytes. Player records
// are 23984 bytes apart starting at player. The sixteen 4-byte words at record offset 21896 are
// compared with the item pointer in order; the result is the index of the first equal word, or
// -100000000 (0xfa0a1f00) when none matches. No memory is written.
// Inferred: the sixteen words are the backpack-style slot pointers of the record; the name of the
// record field is not established here.
// Unknown: what a caller does with the sentinel besides comparing it.
extern "C" {
extern unsigned char player[];

int inventoryItemCheckSlot__FiPQ24item11description(int playerIndex, void* item) {
    const unsigned char* slots = player + playerIndex * 23984 + 21896;
    const int wanted = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(item));
    for (int i = 0; i < 16; ++i) {
        int word;
        __builtin_memcpy(&word, slots + i * 4, 4);
        if (word == wanted) return i;
    }
    return -100000000;
}
}
