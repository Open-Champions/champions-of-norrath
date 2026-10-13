// Observed unnamed rodata C string at 0x46cd60. Callers pass this address as text.
// The view is the characters plus the terminating NUL. Zero padding after it is not part of this object.
// PushTrigger passes it to findTagString.
extern "C" const char norrath_ctors_lava_wraith_view_46cd60[];
const char norrath_ctors_lava_wraith_view_46cd60[] = "iconValue";
static_assert(sizeof norrath_ctors_lava_wraith_view_46cd60 == 10, "the view ends at the terminating NUL");
