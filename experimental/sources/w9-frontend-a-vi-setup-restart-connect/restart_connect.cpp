// Observed: RestartConnect__7VISetup at 0x01150bb8 is 120 bytes. With the object as this, it runs
// SetRoot__15VIWindowManagerP5VIWnd(this + 0xaf64, this + 0xafe8), then
// SetFocus__5VIWndP5VIWndb(this + 0xaf64, this + 0xafe8, 1), Free__12VIWndPatcher(this + 0xd5b0),
// Init__12VIWndPatcher(this + 0xd5b0) and RestartConnect__11VIWndSplash(this + 0xafe8).
// Inferred: this + 0xaf64 is a window manager, this + 0xafe8 the splash window and this + 0xd5b0 the
// patcher window, from the mangled method owners.
// Unknown: the member types beyond those names.

extern "C" {
void SetRoot__15VIWindowManagerP5VIWnd(void* object, void*);
void SetFocus__5VIWndP5VIWndb(void* object, void*, bool);
void Free__12VIWndPatcher(void* object);
void Init__12VIWndPatcher(void* object);
void RestartConnect__11VIWndSplash(void* object);
}

extern "C" void RestartConnect__7VISetup(void* self) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    SetRoot__15VIWindowManagerP5VIWnd(base + 0xaf64, base + 0xafe8);
    SetFocus__5VIWndP5VIWndb(base + 0xaf64, base + 0xafe8, 1);
    Free__12VIWndPatcher(base + 0xd5b0);
    Init__12VIWndPatcher(base + 0xd5b0);
    RestartConnect__11VIWndSplash(base + 0xafe8);
}
