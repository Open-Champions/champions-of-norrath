// Observed unnamed C string in .rodata at 0x471230. AutoPropLP uses it as a format-string tag name for the counted tag lookups.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471230[];
const char norrath_ctors_ant_lantern_view_471230[] = "Particle%dState%doff";
static_assert(sizeof norrath_ctors_ant_lantern_view_471230 == 21, "the view ends at the terminating NUL");
