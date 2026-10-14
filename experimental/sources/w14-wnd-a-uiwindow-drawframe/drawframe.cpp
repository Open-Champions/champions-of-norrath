// Observed: DrawFrame__10VIUIWindowiiiiUi at 0x0103d220 is 824 bytes. Arguments: this (a window whose word 0 is the
// UI object, ints at +12 x, +16 y, +20 w, +24 h, +28 inner offset, and the float at +40); four ints a, b, c, d; and
// unsigned flags. With ui = the word at this, font = ui + 56, raster = the word at ui + 72 and dictionary = the word at
// ui + 80, and GetHeight__6VIFontP8VIRasterP12VIDictionary(font, raster, dictionary) called twice (h1, then h2):
//  - X0 = x - (w >> 1) and Y0 = y - (h >> 1) (arithmetic shifts);
//  - position P = (X0 + a - 4, Y0 + inner + (h2 - 4) * b + 5 - 7), size S = (c + 8, e) with e = d * (h1 - 4), plus 4
//    when (flags & 0x40) is zero. (The original also stores several unused companion pairs in locals; they are not
//    reproduced.)
//  - a 64-byte parameter is cleared with memset(param, 0, 64) and its sixteen ints are set: p[0] = 101, p[2] = 102,
//    p[4] = 103, p[6] = 104, p[8] = 106, p[10] = 102, p[12] = 105, p[14] = 104, the rest 0.
//  - GetUITexture__F9VIUITexId(101) and (106) are called and their results ignored.
//  - StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(ui, 61, P, S, 3, f * 0.1) is called where f is the
//    float at this+40 and the factor has bits 0x3dcccccc.
//  - BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(ui, P, S, param, 0, f, 1) is called.
//  - t = GetUITexture__F9VIUITexId(128) is called; then, only when (flags & 0x20) is nonzero:
//    TickPulse__C10VIUIWindow(this) is called and BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 128,
//    (P.x - t[4] - 3, P.y + (S.y >> 1) - (t[5] >> 1)), 3, the pulse float, 0) is called; then TickPulse again and the
//    same BlitEx at (P.x + S.x + 3, the same y) with the new pulse float and the last argument 1.
//  No result is produced.
// Inferred: this draws a dialog frame sized in text rows (the font height minus 4 per row) with a soft shadow quad,
// and with flag 0x20 two blinking arrow marks (texture 128) on both sides; flag 0x40 selects whether the frame is
// four units taller; the float at +40 is the open progress also read by IsOpen__C10VIUIWindow and is used as alpha.
// Unknown: the ids' names, the units of the pulse, what a, b, c, d mean, and the result type (void here).
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};
struct VIUIFrameParam {
    int v[16];
};
int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary);
const int* GetUITexture__F9VIUITexId(int texture);
int StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void* ui, int texture, VIVect2I pos, VIVect2I size,
                                                                   int blend, float alpha);
int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void* ui, const VIVect2I& pos, const VIVect2I& size,
                                                                        const VIUIFrameParam& parameter, int blend,
                                                                        float alpha, unsigned flag);
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void* ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags);
float TickPulse__C10VIUIWindow(const void* self);
void* memset(void* destination, int value, unsigned size);

static inline void* ptr(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}

void DrawFrame__10VIUIWindowiiiiUi(void* self, int a, int b, int c, int d, unsigned flags) {
    const int* words = static_cast<const int*>(self);
    unsigned char* ui = static_cast<unsigned char*>(ptr(static_cast<unsigned>(words[0])));
    const float alpha = reinterpret_cast<const float*>(words)[10];
    const int h1 = GetHeight__6VIFontP8VIRasterP12VIDictionary(
        ui + 56, ptr(*reinterpret_cast<unsigned*>(ui + 72)), ptr(*reinterpret_cast<unsigned*>(ui + 80)));
    const int x0 = words[3] - (words[5] >> 1);
    const int y0 = words[4] - (words[6] >> 1);
    const int body = d * (h1 - 4);
    const int h2 = GetHeight__6VIFontP8VIRasterP12VIDictionary(
        ui + 56, ptr(*reinterpret_cast<unsigned*>(ui + 72)), ptr(*reinterpret_cast<unsigned*>(ui + 80)));
    VIVect2I pos;
    pos.x = x0 + a - 4;
    pos.y = y0 + words[7] + (h2 - 4) * b + 5 - 7;
    VIVect2I size;
    size.x = c + 8;
    size.y = (flags & 0x40) ? body : body + 4;
    VIUIFrameParam param;
    memset(&param, 0, 64);
    GetUITexture__F9VIUITexId(101);
    GetUITexture__F9VIUITexId(106);
    param.v[0] = 101;
    param.v[2] = 102;
    param.v[4] = 103;
    param.v[6] = 104;
    param.v[8] = 106;
    param.v[10] = 102;
    param.v[12] = 105;
    param.v[14] = 104;
    StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(ui, 61, pos, size, 3,
                                                                   alpha * __builtin_bit_cast(float, 0x3dcccccc));
    BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(ui, pos, size, param, 0, alpha, 1);
    const int* arrow = GetUITexture__F9VIUITexId(128);
    if (flags & 0x20) {
        VIVect2I at;
        at.x = pos.x - arrow[4] - 3;
        at.y = pos.y + (size.y >> 1) - (arrow[5] >> 1);
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 128, at, 3, TickPulse__C10VIUIWindow(self), 0);
        at.x = pos.x + size.x + 3;
        at.y = pos.y + (size.y >> 1) - (arrow[5] >> 1);
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 128, at, 3, TickPulse__C10VIUIWindow(self), 1);
    }
}
}
