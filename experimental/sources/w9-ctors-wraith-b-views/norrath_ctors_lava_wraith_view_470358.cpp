// Observed unnamed rodata C string at 0x470358. Callers pass this address as text.
// The view is the characters plus the terminating NUL. Zero padding after it is not part of this object.
// Referenced by the lava-wraith constructors.
extern "C" const char norrath_ctors_lava_wraith_view_470358[];
const char norrath_ctors_lava_wraith_view_470358[] = "spiderweb.tex";
static_assert(sizeof norrath_ctors_lava_wraith_view_470358 == 14, "the view ends at the terminating NUL");
