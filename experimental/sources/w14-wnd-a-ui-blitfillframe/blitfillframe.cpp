// Observed: BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc at 0x0102e0a0 is 544
// bytes. Arguments: this; pos and size (two ints each, by pointer); a 64-byte frame parameter p[0..15] (by pointer,
// copied into a local); a fill texture id; a blend mode; the float alpha (f12); a byte flag (low 8 bits only). It calls
// GetUITexture__F9VIUITexId for p[0], then p[8], then the fill texture id (records a, b, f) and returns -1 without
// drawing when any is null. Otherwise it calls BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(this, fill,
// (pos.x + a[4], pos.y + a[5]), (size.x - a[4] - b[4], size.y - a[5] - b[5]), blend, alpha), ignoring its result, and
// then returns the result of BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(this, pos, size, the
// local parameter copy, blend, alpha, flag).
// Inferred: a frame with a filled interior: the first and third corner textures (the top-left and bottom-right of
// the BlitFrame parameter order) give the margins that the fill rectangle is inset by.
// Unknown: the parameter type's real fields and name, and the enum names.
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
int BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void* ui, int texture, VIVect2I pos, VIVect2I size, int blend,
                                                              float alpha);
int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void* ui, const VIVect2I& pos, const VIVect2I& size,
                                                                        const VIUIFrameParam& parameter, int blend,
                                                                        float alpha, unsigned flag);

int BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(
    void* self, const VIVect2I& pos, const VIVect2I& size, const VIUIFrameParam& parameter, int fill, int blend, float alpha,
    unsigned flag_argument) {
    const VIUIFrameParam p = parameter;
    const unsigned flag = flag_argument & 0xff;
    const int* a = GetUITexture__F9VIUITexId(p.v[0]);
    const int* b = GetUITexture__F9VIUITexId(p.v[8]);
    const int* f = GetUITexture__F9VIUITexId(fill);
    if (!a || !b || !f) return -1;
    VIVect2I inner_pos;
    inner_pos.x = pos.x + a[4];
    inner_pos.y = pos.y + a[5];
    VIVect2I inner_size;
    inner_size.x = size.x - a[4] - b[4];
    inner_size.y = size.y - a[5] - b[5];
    BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(self, fill, inner_pos, inner_size, blend, alpha);
    return BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(self, pos, size, p, blend, alpha, flag);
}
}
