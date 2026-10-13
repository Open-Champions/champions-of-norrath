// Observed unnamed rodata C string at 0x46d7b8. Callers pass this address as text.
// The view is the characters plus the terminating NUL. Zero padding after it is not part of this object.
// Referenced by the lava-wraith constructors.
extern "C" const char norrath_ctors_lava_wraith_view_46d7b8[];
const char norrath_ctors_lava_wraith_view_46d7b8[] = "lb_block";
static_assert(sizeof norrath_ctors_lava_wraith_view_46d7b8 == 9, "the view ends at the terminating NUL");
