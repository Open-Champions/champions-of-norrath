// Observed: __9VICollide at 0x01092608 is 212 bytes and returns nothing.
// It zeroes two 64-byte (16-float) blocks at offsets 16 and 80 (each by a
// float-store loop and then again with four 16-byte vector stores), zeroes 32
// three-float entries of 12 bytes from offset 144 (384 bytes, to offset 528),
// then tail-calls the VIPool<VICollBuffer*> constructor
// (__t6VIPool1ZP12VICollBuffer) on offset 528.
// Not byte-identical: the vector stores are written as plain float stores.
// Unknown: the full size of VICollide, the meaning of the 12-byte entries, and
// whether bytes 0 to 15 are used.
extern "C" {
void __t6VIPool1ZP12VICollBuffer(void* pool);

void __9VICollide(void* object) {
    float* base = static_cast<float*>(object);
    for (int index = 0; index < 16; ++index) base[4 + index] = 0.0f;
    for (int index = 0; index < 16; ++index) base[20 + index] = 0.0f;
    for (int index = 0; index < 96; ++index) base[36 + index] = 0.0f;
    __t6VIPool1ZP12VICollBuffer(base + 132);
}
}
