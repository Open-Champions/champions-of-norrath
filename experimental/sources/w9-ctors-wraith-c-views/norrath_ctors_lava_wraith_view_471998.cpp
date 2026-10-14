// Observed unnamed .rodata C string at 0x471998. Callers pass this address as text.
// The view is the characters plus the terminating NUL (11 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_471998[];
const char norrath_ctors_lava_wraith_view_471998[] = "orc2shaman";
static_assert(sizeof norrath_ctors_lava_wraith_view_471998 == 11, "the view ends at the terminating NUL");
