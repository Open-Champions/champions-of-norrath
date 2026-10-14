// Observed: OnKeyUp__12VIWndConnecti at 0x010a2bb8 is 8 bytes: it returns at once. It reads no argument and
// writes no memory. This is the OnKeyUp slot (table offset 32) of the _vt$12VIWndConnect table at 0x01207db8.
// Inferred: the Connect window ignores key releases.
// Unknown: what the one integer argument would be (a key code, from the name only).
extern "C" void OnKeyUp__12VIWndConnecti(void*, int) {}
