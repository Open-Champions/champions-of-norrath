// Observed p1 at 0x008d5938 and p2 at 0x008d5948, file-local .bss objects with 16 bytes
// between each and the next (v1 at 0x008d5958, v2 at 0x008d5968, hvecs at 0x008d5978).
// The authored position-hermite initialiser copies three floats into each and adds 61.0 to
// the third. A 12-byte point padded to a 16-byte slot is inferred from that spacing and
// the use; no code read so far touches the fourth word.
// Unknown: whether the slot is a quadword-aligned vector type in the original source.
struct alignas(16) Slot {
    float x, y, z;
};
extern "C" {
Slot p1;
Slot p2;
}
