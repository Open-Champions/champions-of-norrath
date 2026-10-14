// Observed: CountLines__10VIUIWindowPCUsi at 0x0103f590, 48 bytes. Arguments: the window, a 16-bit string and an
// int n. It calls the colour-reference overload of TextBox
// (TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi, 0x0103c660) with (window, string, 0, 0, n, 256, the 16-byte
// record at offset 304 of the window, 0xffffffff) and returns that call's result unchanged.
// Inferred: the all-ones flag word is the same sentinel TextBox compares against; with it the call is a layout
// pass whose result is a line count, hence the name. Offset 304 is the first of the window's colour records.
// Unknown: the exact result semantics of the callee, and the meaning of 256 (taken to be a row limit).
extern "C" {
struct VIColor32F;

int TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* self, const unsigned short* text, int x, int y, int width,
                                                  int rows, const VIColor32F& color, unsigned flags);

int CountLines__10VIUIWindowPCUsi(void* self, const unsigned short* text, int width) {
    return TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(
        self, text, 0, 0, width, 256, *reinterpret_cast<const VIColor32F*>(static_cast<char*>(self) + 304), 0xffffffffu);
}
}
