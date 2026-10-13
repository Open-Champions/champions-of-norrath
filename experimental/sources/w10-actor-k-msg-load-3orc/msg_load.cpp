// Observed msg_load__3OrcPc at 0x0022b728, 140 bytes. Calls the Creature load routine, then
// reads four 32-bit words that follow the bytes it consumed and stores them at object+1116,
// +972, +1112 and +208 (the mirror of the save routine). It then calls initType__3Orcb with the
// object and 1. If the halfword at +572 equals 10, the word at +1124 is cleared. Returns the
// Creature count plus 16. Nothing here ran on the EE.
extern "C" {
int msg_load__8CreaturePc(void* self, char* buffer);
void initType__3Orcb(void* self, int flag);

int msg_load__3OrcPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int* in = reinterpret_cast<const int*>(buffer + msg_load__8CreaturePc(self, buffer));
    *reinterpret_cast<int*>(o + 1116) = *in++;
    *reinterpret_cast<int*>(o + 972) = *in++;
    *reinterpret_cast<int*>(o + 1112) = *in++;
    *reinterpret_cast<int*>(o + 208) = *in++;
    initType__3Orcb(self, 1);
    if (*reinterpret_cast<short*>(o + 572) == 10) *reinterpret_cast<int*>(o + 1124) = 0;
    return static_cast<int>(reinterpret_cast<const char*>(in) - buffer);
}
}
