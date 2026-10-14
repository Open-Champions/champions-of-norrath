// Observed unnamed C string in .rodata at 0x471150. AutoContainer passes it as a tag name to both a string-valued and an integer tag lookup.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471150[];
const char norrath_ctors_ant_lantern_view_471150[] = "extraLength";
static_assert(sizeof norrath_ctors_ant_lantern_view_471150 == 12, "the view ends at the terminating NUL");
