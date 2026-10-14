// Observed unnamed rodata C string at 0x46cd20. Callers pass this address as text.
// The view is the characters plus the terminating NUL. Zero padding after it is not part of this object.
// Tag text passed to findTagInt by PushTrigger.
extern "C" const char norrath_ctors_lava_wraith_view_46cd20[];
const char norrath_ctors_lava_wraith_view_46cd20[] = "hasPhysics";
static_assert(sizeof norrath_ctors_lava_wraith_view_46cd20 == 11, "the view ends at the terminating NUL");
