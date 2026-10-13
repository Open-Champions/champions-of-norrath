// Observed msg_save__6GoblinPc at 0x0022f0d8, 112 bytes.
// Calls the Creature save routine on the buffer, which returns a byte count n. Four words are then
// stored after it, in this order: the words at object+972, +988, +992 and +208. Returns n + 16.
// Unknown: what the saved words mean. Nothing here ran on the EE.
extern "C" int msg_save__8CreaturePc(void* self, char* buffer);

extern "C" int msg_save__6GoblinPc(void* self, char* buffer) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    const int n = msg_save__8CreaturePc(self, buffer);
    unsigned* out = reinterpret_cast<unsigned*>(buffer + n);
    out[0] = *reinterpret_cast<const unsigned*>(object + 972);
    out[1] = *reinterpret_cast<const unsigned*>(object + 988);
    out[2] = *reinterpret_cast<const unsigned*>(object + 992);
    out[3] = *reinterpret_cast<const unsigned*>(object + 208);
    return n + 16;
}
