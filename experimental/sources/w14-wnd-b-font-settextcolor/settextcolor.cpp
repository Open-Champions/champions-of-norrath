// Observed: SetTextColor__6VIFontRC10VIColor32F at 0x01012150 is 36 bytes. With (this, color) it copies 16
// bytes from color to this (two unaligned 8-byte moves each way) and returns nothing.
// Inferred: VIColor32F is a 16-byte value (four 32-bit floats by the name) and the font keeps its current text
// colour in its first 16 bytes. The unaligned moves show that neither pointer is known to be 8-byte aligned.
// Unknown: the member layout of VIColor32F and of the font object beyond this copy.
struct VIColor16 {
    int w[4];
} __attribute__((packed));

extern "C" void SetTextColor__6VIFontRC10VIColor32F(VIColor16* self, const VIColor16* color) {
    *self = *color;
}
