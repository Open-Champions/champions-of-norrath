// Observed .rodata C string at 0x4770d0. Female dark elf compares the actor name against this text (case-insensitive).
// The view is the characters plus the terminating NUL. Following zero bytes are padding and not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_4770d0[];
const char norrath_ctors_ant_lantern_view_4770d0[] = "femaledarkelfsoldier";
static_assert(sizeof norrath_ctors_ant_lantern_view_4770d0 == 21, "the view ends at the terminating NUL");
