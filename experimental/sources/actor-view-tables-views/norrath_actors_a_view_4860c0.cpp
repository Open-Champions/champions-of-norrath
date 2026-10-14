// Observed unnamed sdata C string at 0x4860c0. Callers pass this address as text.
// The view is the characters plus the terminating NUL. One zero byte follows the NUL and is not part of this object.
// FireBomb uses it as the fireOK tag.
extern "C" const char norrath_actors_a_view_4860c0[];
const char norrath_actors_a_view_4860c0[] = "fireOK";
static_assert(sizeof norrath_actors_a_view_4860c0 == 7, "the view ends at the terminating NUL");
