// Observed unnamed C string in .rodata at 0x471000. AutoContainer appends it to a copy of a name (strcat).
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_471000[];
const char norrath_ctors_ant_lantern_view_471000[] = "_explode.tex";
static_assert(sizeof norrath_ctors_ant_lantern_view_471000 == 13, "the view ends at the terminating NUL");
