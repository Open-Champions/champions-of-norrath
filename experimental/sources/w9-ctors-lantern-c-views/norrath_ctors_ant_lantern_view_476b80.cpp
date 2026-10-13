// Observed unnamed C string in .rodata at 0x476b80. The JumpGate constructor selects it or the string at 0x476b90 by a byte flag.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_476b80[];
const char norrath_ctors_ant_lantern_view_476b80[] = "jumphurt";
static_assert(sizeof norrath_ctors_ant_lantern_view_476b80 == 9, "the view ends at the terminating NUL");
