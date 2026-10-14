// Observed unnamed C string in .rodata at 0x471ee0. The Goblin constructor compares the object name against it (case-insensitive).
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471ee0[];
const char norrath_ctors_ant_lantern_view_471ee0[] = "goblinsoldier";
static_assert(sizeof norrath_ctors_ant_lantern_view_471ee0 == 14, "the view ends at the terminating NUL");
