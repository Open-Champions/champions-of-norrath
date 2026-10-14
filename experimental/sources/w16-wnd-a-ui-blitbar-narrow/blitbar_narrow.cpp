// Observed: BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii at 0x0102f548 is 264 bytes. It has
// the arguments of the 16-bit-text overload at 0x0102cc48 (this; text; colour and position passed by reference to copies;
// two texture ids; width; blend; alpha float; two stack words) but takes an 8-bit text. It widens the text into a 512-byte
// stack buffer with str8to16__8VIStringPUsUiPCc(buffer, 512, text), copies the colour (16 bytes) and the position (two
// words) into fresh locals, and calls the 16-bit overload with the buffer, those copies and every other argument unchanged;
// its result is returned.
// Inferred: a convenience wrapper that lets callers pass a narrow string.
// Unknown: the enum names behind the integer arguments and the meaning of the last two stack words.
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};
struct VIColor32F {
    float v[4];
};
void str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
int BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(
    void* self, const unsigned short* text, const VIColor32F& color_argument, const VIVect2I& pos, int left, int middle,
    int width, int blend, float alpha, unsigned flags, int extra);

int BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(
    void* self, const char* text, const VIColor32F& color_argument, const VIVect2I& pos, int left, int middle, int width,
    int blend, float alpha, unsigned flags, int extra) {
    unsigned short wide[256];
    str8to16__8VIStringPUsUiPCc(wide, 512, text);
    const VIColor32F color = color_argument;
    const VIVect2I position = pos;
    return BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(self, wide, color, position, left,
                                                                                         middle, width, blend, alpha, flags, extra);
}
}
