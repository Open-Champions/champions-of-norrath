// Observed compareStatePackets__FP11statePacketT0i at 0x00129f00, 232 bytes, a local function
// (it is only reached from advanceRealState). Arguments: two state packets (first, second) and a
// partner index. Nothing happens when the first packet's word at +0 is zero or when the words at
// +8 of both packets are equal. Otherwise (a mismatch):
//   1. When the signed byte MC_Debug is nonzero: format a text with sprintf from a literal of the
//      image ("Out of sync (seed=%08x:%d)" and a newline) and the globals megatileSeed and
//      megatileRot into a local buffer, widen it with ustrcpy__FPUsPc into a second buffer, show it
//      through hudSetInfoMessage__FPUsiic (300, -1, 0), then advance the sound/effect random state
//      eRandState (state * 1664525 + 1013904223) three times and call engineSetScreenColor__Fiiii
//      with the three results' bits 16..23 and the alpha 128. The state is stored once, after the
//      third step.
//   2. In every mismatch the byte at partners[partner * 17632 + 17623] is set to 1.
// Inferred: the byte set at +17623 flags the partner as out of sync, and the +8 word is a checksum
// or sequence value of the packet. Unknown: the packet layout beyond those two words, and the
// fields that precede MC_Debug in meaning (the name suggests memory card debug; here it only gates
// the on-screen report). Nothing here ran on the EE.
extern "C" {
extern signed char MC_Debug;
extern int megatileSeed;
extern int megatileRot;
extern unsigned eRandState;
extern unsigned char partners[];
int sprintf(char* buffer, const char* format, ...);
void ustrcpy__FPUsPc(unsigned short* destination, char* source);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int ticks, int color, char flag);
void engineSetScreenColor__Fiiii(int red, int green, int blue, int alpha);
}

extern "C" void compareStatePackets__FP11statePacketT0i(unsigned* first, unsigned* second, int partner) {
    if (first[0] == 0) return;
    if (first[2] == second[2]) return;
    if (MC_Debug != 0) {
        char text[80];
        unsigned short wide[96];
        sprintf(text, "Out of sync (seed=%08x:%d)\n", megatileSeed, megatileRot);
        ustrcpy__FPUsPc(wide, text);
        hudSetInfoMessage__FPUsiic(wide, 300, -1, 0);
        unsigned state = eRandState * 1664525u + 1013904223u;
        unsigned state_two = state * 1664525u + 1013904223u;
        unsigned state_three = state_two * 1664525u + 1013904223u;
        eRandState = state_three;
        engineSetScreenColor__Fiiii((state >> 16) & 0xff, (state_two >> 16) & 0xff, (state_three >> 16) & 0xff, 128);
    }
    partners[partner * 17632 + 17623] = 1;
}
