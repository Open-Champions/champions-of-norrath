// Observed: ambientGroup 0x007dd8c0 is .bss, 10880 bytes to the next symbol: sixteen 680-byte
// groups (the layout is recorded in runAmbient__Fv and soundSetAmbientData__FPc). nmAmbientGroups
// is the 4-byte word at 0x004859bc, initially 0.
// Inferred: nothing.
// Unknown: whether sixteen is a deliberate limit; nothing checks it.
extern "C" {
alignas(16) unsigned char ambientGroup[680 * 16];
int nmAmbientGroups = 0;
}
