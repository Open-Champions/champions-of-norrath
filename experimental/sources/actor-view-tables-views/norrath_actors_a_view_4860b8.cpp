// Observed unnamed sdata C string at 0x4860b8. Callers pass this address as text.
// The view is the characters plus the terminating NUL. The next literal begins on the following byte.
// FireBomb uses it as the skydrop tag.
extern "C" const char norrath_actors_a_view_4860b8[];
const char norrath_actors_a_view_4860b8[] = "skydrop";
static_assert(sizeof norrath_actors_a_view_4860b8 == 8, "the view ends at the terminating NUL");
