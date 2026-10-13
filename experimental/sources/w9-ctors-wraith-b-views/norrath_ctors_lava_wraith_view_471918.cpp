// Observed unnamed rodata C string at 0x471918. Callers pass this address as text.
// The view is the characters plus the terminating NUL. Zero padding after it is not part of this object.
// Referenced by the Orc constructor.
extern "C" const char norrath_ctors_lava_wraith_view_471918[];
const char norrath_ctors_lava_wraith_view_471918[] = "orcleader";
static_assert(sizeof norrath_ctors_lava_wraith_view_471918 == 10, "the view ends at the terminating NUL");
