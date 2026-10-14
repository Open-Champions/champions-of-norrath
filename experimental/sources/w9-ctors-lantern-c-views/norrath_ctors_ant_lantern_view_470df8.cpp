// Observed unnamed C string in .rodata at 0x470df8. AutoProp and AutoContainer pass it as a tag name to a tag lookup.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_470df8[];
const char norrath_ctors_ant_lantern_view_470df8[] = "Breakable";
static_assert(sizeof norrath_ctors_ant_lantern_view_470df8 == 10, "the view ends at the terminating NUL");
