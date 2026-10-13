// Observed msg_load__6GoblinPc at 0x0022f148, 144 bytes.
// Calls the Creature load routine on the buffer, which returns a byte count n. Four words are then
// read from buffer+n and stored to object+972, +988, +992 and +208 (the last store lands before the
// next call). initType__6Goblinb(object, 1) is then called, and when the signed halfword at +572
// is 10 the word at +996 is cleared. Returns n + 16.
// Inferred: the pair of this and msg_save__6GoblinPc. initType__6Goblinb is an unresolved import.
// Nothing here ran on the EE.
extern "C" {
int msg_load__8CreaturePc(void* self, char* buffer);
void initType__6Goblinb(void* self, bool flag);
}

extern "C" int msg_load__6GoblinPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const int n = msg_load__8CreaturePc(self, buffer);
    const unsigned* in = reinterpret_cast<const unsigned*>(buffer + n);
    *reinterpret_cast<unsigned*>(object + 972) = in[0];
    *reinterpret_cast<unsigned*>(object + 988) = in[1];
    *reinterpret_cast<unsigned*>(object + 992) = in[2];
    *reinterpret_cast<unsigned*>(object + 208) = in[3];
    initType__6Goblinb(self, true);
    if (*reinterpret_cast<short*>(object + 572) == 10) *reinterpret_cast<unsigned*>(object + 996) = 0;
    return n + 16;
}
