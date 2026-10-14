// Observed msg_load__5MummyPc at 0x0023bdb8, 128 bytes. The Creature load routine reads first and
// returns a byte count n. The words at buffer+n, +n+4 and +n+8 are then stored to object+960,
// +964 and +208, in that order. initType(this, 1) runs, and when the halfword state at +572 then
// equals 10 the word at +964 is cleared. Returns n + 12. Nothing here ran on the EE.
extern "C" {
int msg_load__8CreaturePc(void* self, char* buffer);
void initType__5Mummyb(void* self, bool flag);

int msg_load__5MummyPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int n = msg_load__8CreaturePc(self, buffer);
    const unsigned* in = reinterpret_cast<const unsigned*>(buffer + n);
    *reinterpret_cast<unsigned*>(o + 960) = *in++;
    *reinterpret_cast<unsigned*>(o + 964) = *in++;
    *reinterpret_cast<unsigned*>(o + 208) = *in++;
    initType__5Mummyb(self, true);
    if (*reinterpret_cast<short*>(o + 572) == 10) *reinterpret_cast<unsigned*>(o + 964) = 0;
    return static_cast<int>(reinterpret_cast<const char*>(in) - buffer);
}
}
