// Observed unnamed .rodata C string at 0x474040. Callers pass this address as text.
// The view is the characters plus the terminating NUL (12 bytes). Bytes after the NUL are
// padding or the next string and are not part of this object.
extern "C" const char norrath_ctors_lava_wraith_view_474040[];
const char norrath_ctors_lava_wraith_view_474040[] = "skeletonaxe";
static_assert(sizeof norrath_ctors_lava_wraith_view_474040 == 12, "the view ends at the terminating NUL");
