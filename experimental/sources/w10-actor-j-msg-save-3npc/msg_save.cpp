// Observed msg_save__3NPCPc at 0x002331a0, 144 bytes. Writes six 32-bit words into the buffer
// and returns 24. The Creature save routine is not called. In order: the string-pool indexes
// (objectSaveAddToStringPool) of the texts at object+960, +992 and +1024; the signed halfword
// at +1066 widened to a word; the word at +244; the float at +176. Nothing here ran on the EE.
extern "C" {
unsigned objectSaveAddToStringPool__FPCc(const char* text);

int msg_save__3NPCPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    int* out = reinterpret_cast<int*>(buffer);
    int* const begin = out;
    *out++ = static_cast<int>(objectSaveAddToStringPool__FPCc(reinterpret_cast<const char*>(o + 960)));
    *out++ = static_cast<int>(objectSaveAddToStringPool__FPCc(reinterpret_cast<const char*>(o + 992)));
    *out++ = static_cast<int>(objectSaveAddToStringPool__FPCc(reinterpret_cast<const char*>(o + 1024)));
    *out++ = *reinterpret_cast<short*>(o + 1066);
    *out++ = *reinterpret_cast<int*>(o + 244);
    *reinterpret_cast<float*>(out++) = *reinterpret_cast<float*>(o + 176);
    return static_cast<int>(reinterpret_cast<unsigned char*>(out) - reinterpret_cast<unsigned char*>(begin));
}
}
