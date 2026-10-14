// Observed unnamed .rodata C string at 0x472cb0. Callers pass this address as text.
// The view is the characters plus the terminating NUL (17 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_472cb0[];
const char norrath_ctors_lava_wraith_view_472cb0[] = "mummyeliteshaman";
static_assert(sizeof norrath_ctors_lava_wraith_view_472cb0 == 17, "the view ends at the terminating NUL");
