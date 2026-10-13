// Observed unnamed rodata C string at 0x46cd70. Callers pass this address as text.
// The view is the characters plus the terminating NUL. Zero padding after it is not part of this object.
// PushTrigger passes it to findTagString.
extern "C" const char norrath_ctors_lava_wraith_view_46cd70[];
const char norrath_ctors_lava_wraith_view_46cd70[] = "iconScript";
static_assert(sizeof norrath_ctors_lava_wraith_view_46cd70 == 11, "the view ends at the terminating NUL");
