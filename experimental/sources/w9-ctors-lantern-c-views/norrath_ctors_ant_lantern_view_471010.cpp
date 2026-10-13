// Observed unnamed C string in .rodata at 0x471010. AutoContainer passes it as a tag name to an integer tag lookup.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471010[];
const char norrath_ctors_ant_lantern_view_471010[] = "Explodeable";
static_assert(sizeof norrath_ctors_ant_lantern_view_471010 == 12, "the view ends at the terminating NUL");
