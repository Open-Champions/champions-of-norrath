// Observed: VertexGroup__12VICollBufferSc at 0x0108bc30 is 12 bytes. It stores the low byte of
// its argument at offset 80 of the object and returns 0.
// Inferred: the byte is the group tag applied to following vertices (the prim-buffer sibling keeps
// a similar value in a static). Unknown: signedness of any later read; only the store is observed.
extern "C" {
int VertexGroup__12VICollBufferSc(unsigned char* self, signed char group) {
    self[80] = static_cast<unsigned char>(group);
    return 0;
}
}
