// Observed unnamed C string in .rodata at 0x470e58. AutoProp passes it as a format string to sprintf together with the object name and an integer.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_470e58[];
const char norrath_ctors_ant_lantern_view_470e58[] = "%s%d.anm";
static_assert(sizeof norrath_ctors_ant_lantern_view_470e58 == 9, "the view ends at the terminating NUL");
