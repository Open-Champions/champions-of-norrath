// Observed: IsOpen__C10VIUIWindow at 0x0103f7d8 is 36 bytes. It compares the float at +40 of this with 1.0f and
// returns 1 when they are equal, otherwise 0.
// Inferred: the float at +40 is an open/close progress value that reaches exactly 1.0 when the window is fully open
// (the neighbouring IsOpening__C10VIUIWindow tests it for less than 1.0).
// Unknown: the field name and whether the result type is bool or int.
extern "C" {
int IsOpen__C10VIUIWindow(const void* self) {
    return static_cast<const float*>(self)[10] == 1.0f;
}
}
