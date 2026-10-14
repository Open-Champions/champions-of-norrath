// Observed msg_save__5MummyPc at 0x0023bd58, 96 bytes. The Creature save routine writes first and
// returns a byte count n; then the words at object+960, +964 and +208 are stored in that order at
// buffer+n, buffer+n+4 and buffer+n+8, and n + 12 is returned. Nothing here ran on the EE.
extern "C" {
int msg_save__8CreaturePc(void* self, char* buffer);

int msg_save__5MummyPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int n = msg_save__8CreaturePc(self, buffer);
    unsigned* out = reinterpret_cast<unsigned*>(buffer + n);
    *out++ = *reinterpret_cast<unsigned*>(o + 960);
    *out++ = *reinterpret_cast<unsigned*>(o + 964);
    *out++ = *reinterpret_cast<unsigned*>(o + 208);
    return static_cast<int>(reinterpret_cast<char*>(out) - buffer);
}
}
