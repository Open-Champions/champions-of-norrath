// Observed unnamed .rodata C string at 0x473f30. Callers pass this address as text.
// The view is the characters plus the terminating NUL (13 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_473f30[];
const char norrath_ctors_lava_wraith_view_473f30[] = "skeletonblue";
static_assert(sizeof norrath_ctors_lava_wraith_view_473f30 == 13, "the view ends at the terminating NUL");
