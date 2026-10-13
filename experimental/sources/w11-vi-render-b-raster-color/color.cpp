// Observed: Color__8VIRasterRC10VIColor32F at 0x011132b0 is 40 bytes. It copies 16 bytes from the
// color argument to offset 32 of the raster using two unaligned 64-bit loads and stores, and
// returns zero. The source is read as two doublewords at +0 and +8.
// Inferred: the 16 bytes are one 4-component color (four 32-bit components).
// Unknown: whether the components are floats or integers; this copy does not say.
extern "C" {
struct VIRaster;
struct VIColor32F;

struct Quad {
    unsigned long long low;
    unsigned long long high;
} __attribute__((packed));

int Color__8VIRasterRC10VIColor32F(VIRaster* raster, const VIColor32F* color) {
    const Quad* from = reinterpret_cast<const Quad*>(color);
    Quad* to = reinterpret_cast<Quad*>(reinterpret_cast<char*>(raster) + 32);
    unsigned long long a = from->low;
    unsigned long long b = from->high;
    to->low = a;
    to->high = b;
    return 0;
}
}
