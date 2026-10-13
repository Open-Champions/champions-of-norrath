// Observed: BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef at 0x0102d448 is 896 bytes. With (this, texture,
// pos, size, blend, alpha), where pos and size are two-int values (x, y) passed by reference copies, it first calls
// GetUITexture__F9VIUITexId(texture); a null result returns -1 and draws nothing. Otherwise, with t the returned record
// (ints: t[0] a texture handle, t[1] a kind, t[4] and t[5] tile width and height), it returns 0 after:
//  - kind != 1: one call of BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi (the pos+size overload)
//    with (this, t[0], pos, size, blend, alpha, 0);
//  - kind == 1 (tiled): the rectangle (pos, size) is covered by tile-sized pieces, drawn with the original texture
//    id (not t[0]). Let at start at pos, rest_x = rest_y = 0. For each column while at.x + t[4] < pos.x + size.x: for
//    each row while at.y + t[5] < pos.y + size.y, BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi
//    (this, texture, at, blend, alpha, 0) is called and at.y += t[5]; then rest_y = pos.y + size.y - at.y, at.y = pos.y
//    and at.x += t[4]. Then rest_x = pos.x + size.x - at.x. While at.y + t[5] < pos.y + size.y (checked first), the
//    pos+size overload draws (at, (rest_x, t[5])) and at.y += t[5]. Then at.x = pos.x and, while at.x + t[4] < pos.x +
//    size.x, it draws (at, (t[4], rest_y)) and at.x += t[4]. Finally it draws (at, (rest_x, rest_y)).
//    All the overload calls pass flags 0. rest_y keeps its initial 0 when the column loop never ran.
// Inferred: this is a tiled fill of a UI rectangle: full tiles, then a right strip, bottom strip and corner cropped by
// the remainders; kind 1 is "tiled" and anything else is a stretched single draw.
// Observed: the original receives pos and size as pointers to caller-made copies, so VIVect2I has a non-trivial copy.
// Unknown: the record layout beyond the four fields used, the meaning of t[0] versus the texture id, the enum
// names, and whether the strips are cropped or scaled by the overload.
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};
const int* GetUITexture__F9VIUITexId(int texture);
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void* ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags);
int BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(void* ui, int texture, VIVect2I at, VIVect2I size, int blend,
                                                              float alpha, unsigned flags);

int BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void* self, int texture, VIVect2I pos, VIVect2I size,
                                                              int blend, float alpha) {
    int rest_x = 0;
    int rest_y = 0;
    const int* record = GetUITexture__F9VIUITexId(texture);
    if (!record) return -1;
    VIVect2I at = pos;
    if (record[1] != 1) {
        VIVect2I whole = size;
        BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(self, record[0], pos, whole, blend, alpha, 0);
        return 0;
    }
    while (at.x + record[4] < pos.x + size.x) {
        while (at.y + record[5] < pos.y + size.y) {
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(self, texture, at, blend, alpha, 0);
            at.y += record[5];
        }
        rest_y = pos.y + size.y - at.y;
        at.y = pos.y;
        at.x += record[4];
    }
    rest_x = pos.x + size.x - at.x;
    while (at.y + record[5] < pos.y + size.y) {
        VIVect2I strip;
        strip.x = rest_x;
        strip.y = record[5];
        BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(self, texture, at, strip, blend, alpha, 0);
        at.y += record[5];
    }
    at.x = pos.x;
    while (at.x + record[4] < pos.x + size.x) {
        VIVect2I strip;
        strip.x = record[4];
        strip.y = rest_y;
        BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(self, texture, at, strip, blend, alpha, 0);
        at.x += record[4];
    }
    VIVect2I corner;
    corner.x = rest_x;
    corner.y = rest_y;
    BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(self, texture, at, corner, blend, alpha, 0);
    return 0;
}
}
