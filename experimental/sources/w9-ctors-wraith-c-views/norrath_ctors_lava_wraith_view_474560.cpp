// Observed unnamed .rodata C string at 0x474560. Callers pass this address as text.
// The view is the characters plus the terminating NUL (13 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_474560[];
const char norrath_ctors_lava_wraith_view_474560[] = "woodelfspear";
static_assert(sizeof norrath_ctors_lava_wraith_view_474560 == 13, "the view ends at the terminating NUL");
