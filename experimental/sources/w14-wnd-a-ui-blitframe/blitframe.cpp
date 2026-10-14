// Observed: BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc at 0x0102d7c8 is 1088 bytes.
// Arguments: this; pos and size (two ints each, passed by pointer); a 64-byte frame parameter (sixteen ints p[0..15],
// passed by pointer and copied into a local); a blend mode; the float alpha (f12); and a byte flag (only the low 8
// bits count). It returns 0 without drawing when size.x <= 0 or size.y <= 0. Otherwise, with right = pos + size, it
// calls GetUITexture__F9VIUITexId for p[0], p[4], p[8], p[12] in that order (records t0, t1, t2, t3; ints t[4] and t[5]
// are the width and height as in BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef) and returns -1, drawing
// nothing, when any is null. Then it makes these calls in order (all with this, blend and alpha):
//  1. BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi at (pos.x + (flag ? t0[4] : 0), pos.y), texture p[2],
//     length size.x - (flag ? t0[4] : 0) - t1[4], last argument p[3];
//  2. BlitHorz at (pos.x + (flag ? t3[4] : 0), right.y - t3[5]), texture p[10], length size.x - (flag ? t3[4] : 0) -
//     t2[4], last argument p[11];
//  3. when flag: BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi at (pos.x, pos.y + t0[5]), texture p[14],
//     length size.y - t0[5] - t3[5], last argument p[15];
//  4. BlitVert at (right.x - t1[4], pos.y + t1[5]), texture p[6], length size.y - t1[5] - t2[5], last argument p[7];
//  5. BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi (this, texture, at, blend, alpha, flags) at pos with
//     texture p[0] and flags p[1], only when flag is nonzero;
//  6. the same at (right.x - t1[4], pos.y) with texture p[4] and flags p[5];
//  7. when flag: the same at (pos.x, right.y - t3[5]) with texture p[12] and flags p[13];
//  8. the same at (right.x - t2[4], right.y - t2[5]) with texture p[8] and flags p[9].
// The result is 0.
// Inferred: this draws a frame from corner and edge textures; the parameter is eight (texture, flags) pairs in the
// order top-left corner, top edge, top-right corner, right edge, bottom-right corner, bottom edge, bottom-left corner,
// left edge. The flag selects whether the left side (left corners and the left edge) is drawn.
// Unknown: the parameter type's real fields and name, the meaning of the pair's second member, and the enum names.
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
const int* GetUITexture__F9VIUITexId(int texture);
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void* ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags);
int BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void* ui, VIVect2I at, int texture, int length, int blend,
                                                               float alpha, unsigned flags);
int BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void* ui, VIVect2I at, int texture, int length, int blend,
                                                               float alpha, unsigned flags);

int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void* self, const VIVect2I& pos,
                                                                        const VIVect2I& size,
                                                                        const VIUIFrameParam& parameter, int blend,
                                                                        float alpha, unsigned flag_argument) {
    const VIUIFrameParam p = parameter;
    const bool left_side = (flag_argument & 0xff) != 0;
    if (size.x <= 0 || size.y <= 0) return 0;
    VIVect2I right;
    right.x = pos.x + size.x;
    right.y = pos.y + size.y;
    const int* t0 = GetUITexture__F9VIUITexId(p.v[0]);
    const int* t1 = GetUITexture__F9VIUITexId(p.v[4]);
    const int* t2 = GetUITexture__F9VIUITexId(p.v[8]);
    const int* t3 = GetUITexture__F9VIUITexId(p.v[12]);
    if (!t0 || !t1 || !t2 || !t3) return -1;
    VIVect2I at;
    int length;
    at.x = pos.x + (left_side ? t0[4] : 0);
    at.y = pos.y;
    length = left_side ? size.x - t0[4] - t1[4] : size.x - t1[4];
    BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(self, at, p.v[2], length, blend, alpha, p.v[3]);
    at.x = pos.x + (left_side ? t3[4] : 0);
    at.y = right.y - t3[5];
    length = left_side ? size.x - t3[4] - t2[4] : size.x - t2[4];
    BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(self, at, p.v[10], length, blend, alpha, p.v[11]);
    if (left_side) {
        at.x = pos.x;
        at.y = pos.y + t0[5];
        BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(self, at, p.v[14], size.y - t0[5] - t3[5], blend, alpha,
                                                                    p.v[15]);
    }
    at.x = right.x - t1[4];
    at.y = pos.y + t1[5];
    BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(self, at, p.v[6], size.y - t1[5] - t2[5], blend, alpha, p.v[7]);
    if (left_side) {
        at.x = pos.x;
        at.y = pos.y;
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(self, p.v[0], at, blend, alpha, p.v[1]);
    }
    at.x = right.x - t1[4];
    at.y = pos.y;
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(self, p.v[4], at, blend, alpha, p.v[5]);
    if (left_side) {
        at.x = pos.x;
        at.y = right.y - t3[5];
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(self, p.v[12], at, blend, alpha, p.v[13]);
    }
    at.x = right.x - t2[4];
    at.y = right.y - t2[5];
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(self, p.v[8], at, blend, alpha, p.v[9]);
    return 0;
}
}
