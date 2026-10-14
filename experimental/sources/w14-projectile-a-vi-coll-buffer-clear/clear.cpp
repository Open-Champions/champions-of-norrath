// Observed Clear__12VICollBuffer at 0x0108b9e0, 124 bytes. O is the object. If the word at O+88 is nonzero it is passed to
// __builtin_vec_delete (an array delete). Afterwards these are stored: words at +8, +12, +16, +44, +48, +52, +56, +60, +64, +68, +72,
// +76, +84 and +88 become 0, byte +80 becomes 0 and byte +81 becomes 255. The routine returns the integer 0 (the register is set
// to zero explicitly). Nothing else is read or written. The caller _$_12VICollBuffer passes only the object.
// Unknown: the member names and layout of the collision buffer (which words are counters and which are pointers), and whether the
// word at +88 is an array that the class allocated itself. Nothing here ran on the EE.
extern "C" {
void __builtin_vec_delete(void* pointer);

int Clear__12VICollBuffer(void* object) {
    unsigned char* const o = static_cast<unsigned char*>(object);
    unsigned* const pointer = reinterpret_cast<unsigned*>(o + 88);
    if (*pointer != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*pointer)));
    *pointer = 0;
    *reinterpret_cast<unsigned*>(o + 8) = 0;
    *reinterpret_cast<unsigned*>(o + 12) = 0;
    o[81] = 0xff;
    *reinterpret_cast<unsigned*>(o + 16) = 0;
    o[80] = 0;
    for (unsigned offset = 44; offset <= 76; offset += 4) *reinterpret_cast<unsigned*>(o + offset) = 0;
    *reinterpret_cast<unsigned*>(o + 84) = 0;
    return 0;
}
}
