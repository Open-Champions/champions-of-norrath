// Observed .rodata C string at 0x476b90. Jump gate chooses between this text and another at one site.
// The view is the characters plus the terminating NUL. Following zero bytes are padding and not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_476b90[];
const char norrath_ctors_ant_lantern_view_476b90[] = "jumpgate";
static_assert(sizeof norrath_ctors_ant_lantern_view_476b90 == 9, "the view ends at the terminating NUL");
