// Observed .sdata C string at 0x485410. Callers pass it as text.
// The view ends at the terminating NUL. Alignment bytes after that NUL are a
// separate placement gap, not reads through this symbol.
extern "C" const char norrath_actor_view_485410[];
const char norrath_actor_view_485410[] = "pitch";
