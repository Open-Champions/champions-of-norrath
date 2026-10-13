// Observed: SetTitle__10VIUIWindowPCUs at 0x0103f220 is 80 bytes. With (this, text) it stores 24 in the word
// at +28 when text is nonzero and 18 when text is null. It then moves the word at +24 by twice the change of
// the word at +28 (+24 = +24 - 2 * old(+28) + 2 * new(+28)), and calls
// str16to8__8VIStringPcUiPCUs(this + 48, 256, text) to convert the 16-bit text into the 256-byte buffer at +48.
// The call is made for a null text as well.
// Inferred: +28 is a title-bar height (24 with a title, 18 without) and +24 a window extent that includes it;
// both readings come from the arithmetic only.
// Unknown: the field names, and what str16to8 does with a null source.
extern "C" {
void str16to8__8VIStringPcUiPCUs(char* dest, unsigned size, const unsigned short* text);

void SetTitle__10VIUIWindowPCUs(void* self, const unsigned short* text) {
    int* words = static_cast<int*>(self);
    const int height = text ? 24 : 18;
    const int old_height = words[7];
    words[7] = height;
    words[6] = words[6] - old_height * 2 + height * 2;
    str16to8__8VIStringPcUiPCUs(reinterpret_cast<char*>(self) + 48, 256, text);
}
}
