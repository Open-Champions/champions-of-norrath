// Observed unnamed C string in .rodata at 0x471248. AutoPropLP uses it as a format-string tag name for the counted tag lookups.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471248[];
const char norrath_ctors_ant_lantern_view_471248[] = "Light%dState%don";
static_assert(sizeof norrath_ctors_ant_lantern_view_471248 == 17, "the view ends at the terminating NUL");
