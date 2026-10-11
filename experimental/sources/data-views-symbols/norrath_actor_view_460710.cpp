// Observed .rodata C string at 0x460710. Callers pass it as text.
// The view ends at the terminating NUL. Alignment bytes after that NUL are a
// separate placement gap, not reads through this symbol.
extern "C" const char norrath_actor_view_460710[];
const char norrath_actor_view_460710[] = "initScript";
