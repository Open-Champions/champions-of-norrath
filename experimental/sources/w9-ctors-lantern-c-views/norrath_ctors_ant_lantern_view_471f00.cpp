// Observed unnamed C string in .rodata at 0x471f00. The Goblin constructor compares the object name against it (case-insensitive).
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471f00[];
const char norrath_ctors_ant_lantern_view_471f00[] = "goblinleader";
static_assert(sizeof norrath_ctors_ant_lantern_view_471f00 == 13, "the view ends at the terminating NUL");
