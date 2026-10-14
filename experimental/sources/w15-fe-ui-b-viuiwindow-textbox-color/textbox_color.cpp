// Observed: TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi at 0x0103f570, 32 bytes. Arguments: the window, a 16-bit
// string, four ints, a colour index and a flags word. It tail-jumps to the colour-reference overload
// TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi with the same string, four ints and flags, and as colour
// reference the 16-byte record at window + 304 + index * 16.
// Inferred: the window holds a table of 16-byte colour records starting at offset 304, indexed by the enum.
// Unknown: the number of table entries and the names of the enumerators.
extern "C" {
struct VIColor32F;

int TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* self, const unsigned short* text, int a, int b, int c, int d,
                                                  const VIColor32F& color, unsigned flags);

int TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* self, const unsigned short* text, int a, int b, int c, int d,
                                              int color, unsigned flags) {
    return TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(
        self, text, a, b, c, d, *reinterpret_cast<const VIColor32F*>(static_cast<char*>(self) + 304 + color * 16), flags);
}
}
