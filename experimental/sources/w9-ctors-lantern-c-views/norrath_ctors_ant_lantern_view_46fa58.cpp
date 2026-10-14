// Observed unnamed C string in .rodata at 0x46fa58. The Ant constructor compares the object name against it (case-insensitive).
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_46fa58[];
const char norrath_ctors_ant_lantern_view_46fa58[] = "antblocker";
static_assert(sizeof norrath_ctors_ant_lantern_view_46fa58 == 11, "the view ends at the terminating NUL");
