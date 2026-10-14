// Observed min at 0x002eb670, 16 bytes, a global function of the codec/keyboard library code. Two signed
// 32-bit arguments; returns the second argument unless the first is strictly smaller, in which case
// it returns the first (so equal values return the second, which is the same number). It has a
// neighbour, max, at 0x002eb680 with the mirrored comparison; max has no authored importer yet and is
// not supplied here. Callers seen: lgFrameEncode and lgFrameDecode. Nothing was executed on the EE.
extern "C" {
int min(int a, int b) {
    return a < b ? a : b;
}
}
