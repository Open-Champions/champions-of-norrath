// Observed: SetIcon__16VIIconDefinitionPvi at 0x01065eb8 is 116 bytes. With (this, data, size) it stores
// the pointer data as a 32-bit word at +964 and size at +968, then copies the 11 bytes of the string
// "static.ico" (including its terminator) to each of +260, +324 and +388. Nothing is returned.
// Inferred: the three 64-byte slots at +260, +324 and +388 hold the icon file names of the icon
// definition's three states (which state each stands for is not established; all three get the same name)
// and data/size describe the icon image in memory.
// Unknown: the slot meanings, the field names, and what the later code does when a slot is overwritten.
extern "C" {
void SetIcon__16VIIconDefinitionPvi(void* self, void* data, int size) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    words[241] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(data));
    words[242] = size;
    __builtin_memcpy(bytes + 260, "static.ico", 11);
    __builtin_memcpy(bytes + 324, "static.ico", 11);
    __builtin_memcpy(bytes + 388, "static.ico", 11);
}
}
