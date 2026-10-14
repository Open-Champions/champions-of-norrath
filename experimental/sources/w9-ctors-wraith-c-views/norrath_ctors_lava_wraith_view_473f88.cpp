// Observed unnamed .rodata C string at 0x473f88. Callers pass this address as text.
// The view is the characters plus the terminating NUL (21 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_473f88[];
const char norrath_ctors_lava_wraith_view_473f88[] = "skeletonsworddhwblue";
static_assert(sizeof norrath_ctors_lava_wraith_view_473f88 == 21, "the view ends at the terminating NUL");
