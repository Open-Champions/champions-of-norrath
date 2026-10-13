// Observed unnamed C string in .rodata at 0x46fd38. The AutoChest constructor compares the object name against it (case-insensitive). The constructor loads this address as 0x470d38; the import name carries 46fd38.
// The view is the characters plus the terminating NUL; zero padding after it is not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_46fd38[];
const char norrath_ctors_ant_lantern_view_46fd38[] = "deathweb";
static_assert(sizeof norrath_ctors_ant_lantern_view_46fd38 == 9, "the view ends at the terminating NUL");
