// Observed: Color__12VIPrimBufferR9VIColor32 at 0x01102da0 is 32 bytes. It copies one 4-byte
// colour value from the argument (read with unaligned access, so no alignment is assumed) into
// the static object _12VIPrimBuffer$PrimColor (0x011b0148 in the pinned image) and returns 0.
// Inferred: that object is the colour applied to following vertices; VIColor32 is four bytes.
// Unknown: channel order, and the consumer of the static (an unresolved import here).
struct PackedColor32 {
    unsigned int value;
} __attribute__((packed));

extern PackedColor32 PrimColor __asm__("_12VIPrimBuffer$PrimColor");

extern "C" {
int Color__12VIPrimBufferR9VIColor32(void*, const PackedColor32* color) {
    PrimColor = *color;
    return 0;
}
}
