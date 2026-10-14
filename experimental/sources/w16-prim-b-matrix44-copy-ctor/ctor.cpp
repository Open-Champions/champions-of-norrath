// Observed: __10VIMatrix44RC10VIMatrix44 at 0x010d07a0 is 84 bytes. Arguments: the object being
// built (a0) and a source matrix (a1). It first writes float 0.0 over all sixteen floats of the
// object in four-float rows, then copies the four 16-byte rows of the source over it with
// quadword loads and stores (lq/sq, so both addresses are treated as 16-byte aligned). The
// return register holds a loop pointer, not the object.
// Inferred: the copy constructor; the zero fill is dead except when the source overlaps the
// object, in which case it would zero the source first. Unknown: why the zero fill is present;
// whether a caller reads the return value (none is known).
extern "C" {
void __10VIMatrix44RC10VIMatrix44(float* self, const float* source) {
    float* row = self;
    for (int i = 0; i < 4; ++i) {
        row[0] = 0.0f;
        row[1] = 0.0f;
        row[2] = 0.0f;
        row[3] = 0.0f;
        row += 4;
    }
    for (int i = 0; i < 16; ++i)
        self[i] = source[i];
}
}
