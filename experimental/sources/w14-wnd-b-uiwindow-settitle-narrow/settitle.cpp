// Observed: SetTitle__10VIUIWindowPCc at 0x0103f1d0 is 76 bytes. With (this, text) it stores 24 in the word at
// +28 when text is nonzero and 18 when text is null. It then moves the word at +24 by twice the change of the
// word at +28 (+24 = +24 - 2 * old(+28) + 2 * new(+28)), and calls strcpy(this + 48, text). The call is made
// for a null text as well; nothing is returned.
// Inferred: the same title-bar geometry as the 16-bit overload (SetTitle__10VIUIWindowPCUs), with the text kept
// as an 8-bit string in the buffer at +48.
// Unknown: the field names, and what strcpy does with a null source (the original does not guard it).
extern "C" {
char* strcpy(char* dest, const char* src);

void SetTitle__10VIUIWindowPCc(void* self, const char* text) {
    int* words = static_cast<int*>(self);
    const int height = text ? 24 : 18;
    const int old_height = words[7];
    words[7] = height;
    words[6] = words[6] - old_height * 2 + height * 2;
    strcpy(reinterpret_cast<char*>(self) + 48, text);
}
}
