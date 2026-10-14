// Observed worldSetFarClip__Ff at 0x0017da20, 28 bytes.
// The limit bits 0x44e10000 are 1800. c.lt.s compares that limit with the
// argument. When the limit is strictly smaller, the argument is replaced
// with the limit. The resulting register is stored to farClip (gp-4120).
// A comparison that is not true, including equality and unordered values,
// stores the original argument.
extern "C" float farClip;

namespace {
float bits_float(unsigned bits) {
    union {
        unsigned bits;
        float value;
    } converted;
    converted.bits = bits;
    return converted.value;
}
}

extern "C" void worldSetFarClip__Ff(float distance) {
    const float limit = bits_float(0x44e10000u);
    if (limit < distance) distance = limit;
    farClip = distance;
}
