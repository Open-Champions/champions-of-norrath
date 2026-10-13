// Observed unnamed C string in .rodata at 0x4711f0. AutoPropLP passes it as a format string to sprintf together with an integer.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_4711f0[];
const char norrath_ctors_ant_lantern_view_4711f0[] = "Shadowballemitter%d";
static_assert(sizeof norrath_ctors_ant_lantern_view_4711f0 == 20, "the view ends at the terminating NUL");
