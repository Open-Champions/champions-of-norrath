// Observed: BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi at 0x0102e750, 464 bytes. The vertical twin of
// BlitHorz: arguments are the UI object, a position pair (by reference), a texture id, a height, a blend mode,
// a float alpha and a flags word. The position is copied first. A height of zero or less returns 0 without any
// call. Otherwise tex = GetUITexture__F9VIUITexId(id) (null: result -1). When word 1 of the record is 1:
// while copy.y + tex[5] < pos.y + height (pos.y re-read each time), the position-only BlitEx overload draws the
// record at the copy and copy.y advances by tex[5]; then the position-and-size overload draws the remainder at
// the copy with size (tex[4], pos.y + height - copy.y). When word 1 is not 1, the position-and-size overload is
// called once at the original position with size (tex[4], height). The result is 0 in every drawing case.
// Inferred: the span repeats the texture down a height; other records are stretched.
// Unknown: as for BlitHorz.
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
int BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(void* ui, int texture, VIVect2I at, VIVect2I size,
                                                              int blend, float alpha, unsigned flags);

int BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void* ui, VIVect2I pos, int texture, int height,
                                                               int blend, float alpha, unsigned flags) {
    if (height <= 0) return 0;
    VIVect2I cursor;
    cursor.x = pos.x;
    cursor.y = pos.y;
    const int* tex = GetUITexture__F9VIUITexId(texture);
    if (!tex) return -1;
    if (tex[1] == 1) {
        while (cursor.y + tex[5] < pos.y + height) {
            VIVect2I at;
            at.x = cursor.x;
            at.y = cursor.y;
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, texture, at, blend, alpha, flags);
            cursor.y = cursor.y + tex[5];
        }
        VIVect2I at;
        at.x = cursor.x;
        at.y = cursor.y;
        VIVect2I size;
        size.x = tex[4];
        size.y = pos.y + height - cursor.y;
        BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(ui, texture, at, size, blend, alpha, flags);
        return 0;
    }
    VIVect2I size;
    size.x = tex[4];
    size.y = height;
    VIVect2I at;
    at.x = cursor.x;
    at.y = cursor.y;
    VIVect2I used = size;
    BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(ui, texture, at, used, blend, alpha, flags);
    return 0;
}
}
