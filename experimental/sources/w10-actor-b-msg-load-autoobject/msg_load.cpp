// Observed msg_load__10AutoObjectPc at 0x224ab0, 8 bytes. Returns zero without
// reading the object or the stream pointer; the stream is not advanced.
// Inferred: zero is the count of stream bytes consumed, as in the derived
// classes' loaders that return a byte count. Unknown: why the base has no state.

extern "C" int msg_load__10AutoObjectPc(void* self, char* stream) {
    (void)self;
    (void)stream;
    return 0;
}
