// Observed unnamed .rodata C string at 0x472c38. Callers pass this address as text.
// The view is the characters plus the terminating NUL (18 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_472c38[];
const char norrath_ctors_lava_wraith_view_472c38[] = "mummydesertshaman";
static_assert(sizeof norrath_ctors_lava_wraith_view_472c38 == 18, "the view ends at the terminating NUL");
