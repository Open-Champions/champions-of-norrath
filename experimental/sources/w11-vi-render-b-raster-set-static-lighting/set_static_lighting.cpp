// Observed: SetStaticLighting__8VIRasterRC10VIColor32F at 0x011141a0 is 48 bytes. It copies 16
// bytes from the color argument to the absolute address 0x70000200 (the top of the EE
// scratchpad region) with unaligned 64-bit loads and stores, and returns zero. The raster is not
// touched.
// Inferred: the 16 bytes are one 4-component color in a scratchpad lighting block.
// Unknown: the scratchpad block's overall layout and its reader.
extern "C" {
struct VIRaster;
struct VIColor32F;

struct Quad {
    unsigned long long low;
    unsigned long long high;
} __attribute__((packed));

int SetStaticLighting__8VIRasterRC10VIColor32F(VIRaster*, const VIColor32F* color) {
    const Quad* from = reinterpret_cast<const Quad*>(color);
    Quad* to = reinterpret_cast<Quad*>(0x70000200u);
    unsigned long long a = from->low;
    unsigned long long b = from->high;
    to->low = a;
    to->high = b;
    return 0;
}
}
