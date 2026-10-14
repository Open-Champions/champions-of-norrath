// Observed: DrawVScroll__10VIUIWindowG8VIVect2IifUi at 0x0103f5c0, 280 bytes. Arguments: the window, a position P (two
// ints, passed by pointer and copied), an int height, a float f and an unsigned flags word that is not read. With ui the
// word at offset 0 of the window and alpha the window's float at +40:
//  1. knob = GetUITexture__F9VIUITexId(84).
//  2. BlitVertBar__4VIUIG8VIVect2I9VIUITexIdN22i16VIAlphaBlendModef(ui, P, 81, 83, 82, height, 3, alpha) is called with a
//     copy of P.
//  3. Only when 0.0 <= f and f <= 1.0 (both compares false for NaN): P.x is decreased by one and P.y increased by the
//     truncated integer (f * (float)(height - knob[5])), using the library int-to-float and truncating float-to-int
//     conversions; then BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, knob[0], P, 3, alpha, 0) is called.
// The knob record is read only in step 3 (its first and sixth words), so a null record is only a problem there.
// Inferred: a vertical slider: a bar made of three textures (81, 82 and 83) of the given height and a knob (texture 84)
// at fraction f of the travel; the horizontal sibling DrawHScroll uses the same shape.
// Unknown: the texture names, the meaning of the three bar textures, and the unused flags word.
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};

const int* GetUITexture__F9VIUITexId(int texture);
int BlitVertBar__4VIUIG8VIVect2I9VIUITexIdN22i16VIAlphaBlendModef(unsigned ui, VIVect2I at, int first, int second,
                                                                  int third, int height, int blend, float alpha);
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags);

void DrawVScroll__10VIUIWindowG8VIVect2IifUi(void* self, VIVect2I pos, int height, float f, unsigned flags) {
    (void)flags;
    const int* words = static_cast<const int*>(self);
    const float alpha = reinterpret_cast<const float*>(words)[10];
    const unsigned ui = static_cast<unsigned>(words[0]);
    VIVect2I p;
    p.x = pos.x;
    p.y = pos.y;
    const int* knob = GetUITexture__F9VIUITexId(84);
    VIVect2I bar;
    bar.x = p.x;
    bar.y = p.y;
    BlitVertBar__4VIUIG8VIVect2I9VIUITexIdN22i16VIAlphaBlendModef(ui, bar, 81, 83, 82, height, 3, alpha);
    if (0.0f <= f && f <= 1.0f) {
        p.x = p.x - 1;
        p.y = p.y + static_cast<int>(f * static_cast<float>(height - knob[5]));
        VIVect2I at;
        at.x = p.x;
        at.y = p.y;
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, knob[0], at, 3, alpha, 0);
    }
}
}
