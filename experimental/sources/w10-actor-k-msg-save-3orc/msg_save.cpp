// Observed msg_save__3OrcPc at 0x0022b6b8, 108 bytes. Calls the Creature save routine with the
// same buffer, then appends four 32-bit words after the bytes it wrote: the words at object+1116,
// +972, +1112 and +208, in that order. Returns the Creature count plus 16. Nothing here ran on
// the EE.
extern "C" {
int msg_save__8CreaturePc(void* self, char* buffer);

int msg_save__3OrcPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    char* const begin = buffer;
    int* out = reinterpret_cast<int*>(buffer + msg_save__8CreaturePc(self, buffer));
    *out++ = *reinterpret_cast<int*>(o + 1116);
    *out++ = *reinterpret_cast<int*>(o + 972);
    *out++ = *reinterpret_cast<int*>(o + 1112);
    *out++ = *reinterpret_cast<int*>(o + 208);
    return static_cast<int>(reinterpret_cast<char*>(out) - begin);
}
}
