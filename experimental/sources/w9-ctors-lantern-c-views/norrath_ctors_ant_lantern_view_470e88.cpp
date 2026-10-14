// Observed unnamed C string in .rodata at 0x470e88. AutoProp passes it as a tag name to a tag lookup.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_470e88[];
const char norrath_ctors_ant_lantern_view_470e88[] = "DoubleSided";
static_assert(sizeof norrath_ctors_ant_lantern_view_470e88 == 12, "the view ends at the terminating NUL");
