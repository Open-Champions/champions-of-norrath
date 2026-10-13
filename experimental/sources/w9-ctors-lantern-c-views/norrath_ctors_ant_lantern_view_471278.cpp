// Observed unnamed C string in .rodata at 0x471278. AutoPropLP compares the object name against it (case-insensitive).
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471278[];
const char norrath_ctors_ant_lantern_view_471278[] = "antlight";
static_assert(sizeof norrath_ctors_ant_lantern_view_471278 == 9, "the view ends at the terminating NUL");
