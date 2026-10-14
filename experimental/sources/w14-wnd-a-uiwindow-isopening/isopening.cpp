// Observed: IsOpening__C10VIUIWindow at 0x0103f7a0 is 52 bytes. It returns 1 when the float at +40 of this is less
// than 1.0f and the float at +44 is equal to 1.0f; otherwise 0. The second float is only read when the first
// test passes.
// Inferred: +40 is the open progress value (see IsOpen__C10VIUIWindow) and +44 a second value that is exactly
// 1.0 in the same state; the combined meaning "still opening" is a guess from the name and shape.
// Unknown: the field names and whether the result type is bool or int.
extern "C" {
int IsOpening__C10VIUIWindow(const void* self) {
    const float* f = static_cast<const float*>(self);
    return f[10] < 1.0f && f[11] == 1.0f;
}
}
