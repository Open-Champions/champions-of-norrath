// Observed: GetModelView__C8VIRaster at 0x01112f58 is 8 bytes in the pinned SLUS-20565 image. It ignores the object and returns the
// constant 0x70000000 (a lui with no low half).
// Inferred: 0x70000000 is the start of the EE scratchpad, so the model-view matrix lives there; the return is a pointer word.
// Unknown: the matrix layout at that address, who fills it, and whether anything else shares the scratchpad start.
// Nothing here ran on the EE; the constant is returned as an integer word, never dereferenced.
extern "C" {
unsigned GetModelView__C8VIRaster(const unsigned char*) {
    return 0x70000000u;
}
}
