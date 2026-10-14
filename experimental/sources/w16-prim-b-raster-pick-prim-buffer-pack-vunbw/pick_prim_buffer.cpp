// Observed: PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal at
// 0x0111eee0 is 8 bytes. It ignores all four arguments and returns 0.
// Inferred: a placeholder pick routine for the buffer layout whose name ends in VUNBW (the caller
// PickPrimBuffer__8VIRasterR13VIPickSegmentii selects it for primitive buffer type 5); it never
// reports a hit. Unknown: whether the layout was left unimplemented or genuinely cannot be picked.
extern "C" {
int PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(void*, void*, void*, void*) {
    return 0;
}
}
