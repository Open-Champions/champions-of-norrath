// Observed: OnKeyUp__11VIWndSplashi at 0x010d6f80 is 8 bytes: it returns at once.
// Inferred: key releases are ignored by this window. Unknown: the argument is the key code, from
// the sibling OnKeyDown routines taking one integer.
extern "C" {
void OnKeyUp__11VIWndSplashi(void*, int) {}
}
