// Observed unnamed rodata C string at 0x46cd30. Callers pass this address as text.
// The view is the characters plus the terminating NUL. Zero padding after it is not part of this object.
// PushTrigger compares the object name with it (strcmp).
extern "C" const char norrath_ctors_lava_wraith_view_46cd30[];
const char norrath_ctors_lava_wraith_view_46cd30[] = "ladddown";
static_assert(sizeof norrath_ctors_lava_wraith_view_46cd30 == 9, "the view ends at the terminating NUL");
