// Observed unnamed C string in .rodata at 0x470ff0. AutoContainer appends it to a copy of a name (strcat).
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_470ff0[];
const char norrath_ctors_ant_lantern_view_470ff0[] = "_explode.vif";
static_assert(sizeof norrath_ctors_ant_lantern_view_470ff0 == 13, "the view ends at the terminating NUL");
