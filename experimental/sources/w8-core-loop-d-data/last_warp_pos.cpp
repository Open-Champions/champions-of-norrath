// Observed lastWarpPos at 0x004842d8, 4 bytes (a local symbol). Every file byte is zero, so the definition is
// zero-initialized storage. Unknown: whether the word is a position or an index; callers declare it unsigned.
extern "C" {
unsigned lastWarpPos = 0;
}
