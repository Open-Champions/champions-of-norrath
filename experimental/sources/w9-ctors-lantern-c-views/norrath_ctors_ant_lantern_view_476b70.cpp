// Observed unnamed C string in .rodata at 0x476b70. The JumpGate constructor passes it as a tag name to an integer tag lookup.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_476b70[];
const char norrath_ctors_ant_lantern_view_476b70[] = "hideicon";
static_assert(sizeof norrath_ctors_ant_lantern_view_476b70 == 9, "the view ends at the terminating NUL");
