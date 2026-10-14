// Observed unnamed .rodata C string at 0x471978. Callers pass this address as text.
// The view is the characters plus the terminating NUL (11 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_471978[];
const char norrath_ctors_lava_wraith_view_471978[] = "orc2leader";
static_assert(sizeof norrath_ctors_lava_wraith_view_471978 == 11, "the view ends at the terminating NUL");
