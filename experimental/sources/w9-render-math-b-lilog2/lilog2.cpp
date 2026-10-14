// Observed lilog2__Fl at 0x0015a030, extent 64. Takes one 32-bit signed value.
// A negative value returns 0. Otherwise the value is shifted right (arithmetic)
// once; if that is zero the result is 0; else the result counts how many further
// one-bit shifts it takes to reach zero. For inputs 1 and above this is
// floor(log2(value)). Inputs 0 and 1 both return 0.
// Inferred: the name suggests an integer base-2 logarithm; the 0 result for
// non-positive inputs is observed, not documented. Unknown: callers' contracts.
extern "C" int lilog2__Fl(long value) {
    int result = 0;
    if (value >= 0) {
        while ((value >>= 1) != 0) {
            ++result;
        }
    }
    return result;
}
