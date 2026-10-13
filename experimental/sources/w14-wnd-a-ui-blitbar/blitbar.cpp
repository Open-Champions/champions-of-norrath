// Observed: BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii at 0x0102cc48 is 744 bytes.
// Arguments: this; a 16-bit text pointer; a colour (four floats, passed by pointer and copied into a local); pos (two
// ints, passed by pointer); a left texture id; a middle texture id; a width; a blend mode; the float alpha (f12); two
// stack arguments, flags (unsigned) and extra (int). It looks up GetUITexture__F9VIUITexId(left) and then
// GetUITexture__F9VIUITexId(middle) (tl and tm); when either is null it returns -1 and draws nothing. Otherwise,
// with at = pos at first:
//  1. BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(this, left, at, blend, alpha, 0); then at.x += tl[4].
//  2. When tm[1] == 1: BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(this, at, middle, width - 2*tl[4],
//     blend, alpha, 0). Otherwise BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(this, middle, at,
//     (width - 2*tl[4], tm[5]), blend, alpha, 0).
//  3. at.x = pos.x + width - tl[4]; the same BlitEx overload as in step 1 with the left texture and the last argument 1.
//  4. A text position is built from an offset (0, 3): when flags == 1 it is (pos.x + tl[4] - 0, pos.y + tm[5]/2 - 3);
//     otherwise (pos.x + width/2 - 0, pos.y + tm[5]/2 - 3), where each half is a signed division by two that rounds
//     toward zero. DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii(this, text, x, y, colour copy, flags, extra) is
//     called with it. The result is 0.
// Inferred: a three-part horizontal bar (left cap, stretched middle, mirrored right cap) with a centred caption;
// flag 1 in step 3 mirrors the cap, and flags == 1 selects left-aligned caption text. tl[4] is a texture width and
// tm[5] a texture height as in BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef.
// Unknown: the enum names, the real meaning of flags and extra, and the (0, 3) offset's purpose.
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
const int* GetUITexture__F9VIUITexId(int texture);
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void* ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags);
int BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(void* ui, int texture, VIVect2I at, VIVect2I size, int blend,
                                                              float alpha, unsigned flags);
int BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void* ui, VIVect2I at, int texture, int width, int blend,
                                                               float alpha, unsigned flags);
int DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii(void* ui, const unsigned short* text, int x, int y,
                                                       const VIColor32F& color, unsigned flags, int extra);

int BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(
    void* self, const unsigned short* text, const VIColor32F& color_argument, const VIVect2I& pos, int left, int middle,
    int width, int blend, float alpha, unsigned flags, int extra) {
    const VIColor32F color = color_argument;
    VIVect2I offset;
    offset.x = 0;
    offset.y = 0;
    VIVect2I at;
    at.x = pos.x;
    at.y = pos.y;
    const int* left_record = GetUITexture__F9VIUITexId(left);
    const int* middle_record = GetUITexture__F9VIUITexId(middle);
    if (!left_record || !middle_record) return -1;
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(self, left, at, blend, alpha, 0);
    at.x += left_record[4];
    if (middle_record[1] == 1) {
        BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(self, at, middle, width - left_record[4] * 2, blend, alpha, 0);
    } else {
        VIVect2I size;
        size.x = width - left_record[4] * 2;
        size.y = middle_record[5];
        BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(self, middle, at, size, blend, alpha, 0);
    }
    at.x = pos.x + width - left_record[4];
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(self, left, at, blend, alpha, 1);
    offset.y = 3;
    VIVect2I text_at;
    if (flags == 1) {
        text_at.x = pos.x + left_record[4] - 0;
        text_at.y = pos.y + middle_record[5] / 2 - offset.y;
    } else {
        text_at.x = pos.x + width / 2 - offset.x;
        text_at.y = pos.y + middle_record[5] / 2 - offset.y;
    }
    DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii(self, text, text_at.x, text_at.y, color, flags, extra);
    return 0;
}
}
