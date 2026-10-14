// Observed: StrcpyS at 0x002e5040, 44 bytes, pinned SLUS_205.65, a local function of the network library (opened by
// sceInetAddress2String and sceInetAddress2Name). With (object, destination, buffer, word_offset): when destination is
// non-null it calls strcpy(destination, buffer + word_offset * 4); the object is not used. It always returns 0.
// Inferred: copies the reply text out of a request/reply block at a word offset; a null destination means the caller does
// not want the text. Unknown: why the object is passed (the sibling MemcpyD uses it for a bound check).
extern "C" {
char* strcpy(char* destination, const char* source);

int StrcpyS(void* object, char* destination, void* buffer, int word_offset) {
    (void)object;
    if (destination != nullptr) strcpy(destination, reinterpret_cast<const char*>(buffer) + word_offset * 4);
    return 0;
}
}
