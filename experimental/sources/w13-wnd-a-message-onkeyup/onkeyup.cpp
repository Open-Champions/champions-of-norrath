// Observed: OnKeyUp__12VIWndMessagei at 0x010c1488 is 8 bytes: it returns at once. It reads no argument and
// writes no memory. This is the OnKeyUp slot (table offset 32) of the _vt$12VIWndMessage table at 0x0120aac8.
// Inferred: the Message window ignores key releases.
// Unknown: what the one integer argument would be (a key code, from the name only).
extern "C" void OnKeyUp__12VIWndMessagei(void*, int) {}
