// Observed unnamed rodata C string at 0x471310. Callers pass this address as text.
// The view is the characters plus the terminating NUL. 3 zero bytes follow the NUL and are not part of this object.
// AntTrap passes it as the second lumpFindResource argument.
extern "C" const char norrath_actors_a_view_471310[];
const char norrath_actors_a_view_471310[] = "anttrapa.anm";
static_assert(sizeof norrath_actors_a_view_471310 == 13, "the view ends at the terminating NUL");
