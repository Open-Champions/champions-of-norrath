// Observed: OnKeyUp__12VIWndPatcheri at 0x010ccc70 is 8 bytes: it returns at once.
// Inferred: key releases are ignored by this window. Unknown: the argument is the key code, from
// the sibling OnKeyDown routines taking one integer.
extern "C" {
void OnKeyUp__12VIWndPatcheri(void*, int) {}
}
