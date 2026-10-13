// Observed: IsOpen__11VIGUIDialog at 0x01030ec0 is 84 bytes. With (this) it takes w = this + 128 (an
// embedded VIUIWindow) and returns 1 when IsOpening__C10VIUIWindow(w) is nonzero; otherwise it
// returns 1 when IsOpen__C10VIUIWindow(w) is nonzero and 0 when it is zero. IsOpen is not called
// when IsOpening already answered.
// Inferred: the dialog counts as open while its window is opening or open.
// Unknown: the type of the callees' results beyond zero/nonzero.
extern "C" {
int IsOpening__C10VIUIWindow(const void* window);
int IsOpen__C10VIUIWindow(const void* window);

int IsOpen__11VIGUIDialog(void* self) {
    const void* window = static_cast<unsigned char*>(self) + 128;
    return IsOpening__C10VIUIWindow(window) != 0 || IsOpen__C10VIUIWindow(window) != 0;
}
}
