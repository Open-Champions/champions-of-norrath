// Observed: Unload__13VIStringTable at 0x0101bbe8 is 88 bytes. If the word at
// offset 8 is nonzero it calls __builtin_vec_delete on it; if the word at offset 12
// is nonzero it does the same. The words at 12, 0, 4 and 8 are then all zero.
// The word at 8 is cleared last in the original and is not cleared in between.
extern "C" {
void __builtin_vec_delete(void* pointer);

void Unload__13VIStringTable(void* table) {
    void** words = static_cast<void**>(table);
    if (words[2] != nullptr) __builtin_vec_delete(words[2]);
    if (words[3] != nullptr) __builtin_vec_delete(words[3]);
    words[3] = nullptr;
    words[0] = nullptr;
    words[1] = nullptr;
    words[2] = nullptr;
}
}
