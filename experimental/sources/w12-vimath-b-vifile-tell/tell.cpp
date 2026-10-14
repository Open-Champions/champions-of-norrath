// Observed: Tell__6VIFile at 0x010caf48 is 8 bytes. It returns the 64-bit value stored at
// offset 16 of the file object and does nothing else.
// Inferred: the value is the current position; nothing in this routine proves that meaning.
// Unknown: the writers of that field and whether callers use the upper word.
extern "C" {
long long Tell__6VIFile(void* self) {
    return *reinterpret_cast<long long*>(static_cast<unsigned char*>(self) + 16);
}
}
