// Observed: OnKeyUp__12VIWndOptionsi at 0x010c63c8 is 8 bytes: it returns at once. It reads no argument and
// writes no memory. This is the OnKeyUp slot (table offset 32) of the _vt$12VIWndOptions table at 0x0120bd88.
// Inferred: the Options window ignores key releases.
// Unknown: what the one integer argument would be (a key code, from the name only).
extern "C" void OnKeyUp__12VIWndOptionsi(void*, int) {}
