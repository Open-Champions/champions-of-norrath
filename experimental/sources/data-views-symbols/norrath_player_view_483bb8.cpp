// Observed .sdata C string at 0x483bb8. Callers pass it as text.
// The view ends at the terminating NUL. Alignment bytes after that NUL are a
// separate placement gap, not reads through this symbol.
extern "C" const char norrath_player_view_483bb8[];
const char norrath_player_view_483bb8[] = "%s.lmp";
