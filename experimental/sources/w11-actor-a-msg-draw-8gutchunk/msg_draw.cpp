// Observed msg_draw__8GutChunk at 0x001e5230, 72 bytes. Returns without doing anything when the global
// g_BloodLevel is zero and the signed byte netNetworkMode is non-zero. Otherwise, when bit 0 of the word at
// object+584 is clear, it calls msg_draw__6PhProp on the object (the callee's result is not used here).
// Inferred: this is the draw handler of the 8GutChunk class from its symbol name; the class's base is PhProp
// from the callee's name. What bit 0 of +584 means is unknown. Nothing here ran on the EE.
extern "C" {
extern int g_BloodLevel;
extern signed char netNetworkMode;
void msg_draw__6PhProp(void* self);

void msg_draw__8GutChunk(void* self) {
    if (g_BloodLevel == 0 && netNetworkMode != 0) return;
    if ((*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 584) & 1) == 0) msg_draw__6PhProp(self);
}
}
