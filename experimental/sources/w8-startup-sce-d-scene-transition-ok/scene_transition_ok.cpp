// sceneTransitionOK.1041 at 0x00486b1c, four .sbss bytes (the next symbol,
// lastNmQ.1088, is 4 bytes later). The image value is zero. The original is a
// compiler-numbered local of the dialog code; it is global here under that
// exact symbol name so the dialog unit, which names it with an asm label, can
// resolve it. Only the int type is used by that unit.
extern "C" {
int sceneTransitionOK asm("sceneTransitionOK.1041");
}
