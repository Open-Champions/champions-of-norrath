// Observed: BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi at 0x0102e580, 464 bytes. Arguments: the UI
// object, a position pair (by reference), a texture id, a width, a blend mode, a float alpha and a flags word.
// The position is copied first. A width of zero or less returns 0 without any call. Otherwise
// tex = GetUITexture__F9VIUITexId(id) (null: result -1). When word 1 of the record is 1, the span is tiled:
// while copy.x + tex[4] < pos.x + width (pos.x re-read each time), the position-only BlitEx overload draws the
// record at the copy and copy.x advances by tex[4]; then the position-and-size overload draws the remainder at
// the copy with size (pos.x + width - copy.x, tex[5]). When word 1 is not 1, the position-and-size overload is
// called once at the original position with size (width, tex[5]). The result is 0 in every drawing case.
// Inferred: the span repeats the texture across a width; a record whose word 1 is not 1 is stretched instead
// of tiled (word 1 also selects the string passed to GetString by BlitEx, here only compared with 1).
// Unknown: why only the value 1 tiles, the BlitEx return values (ignored), and the names of the ids.
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

int BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void* ui, VIVect2I pos, int texture, int width,
                                                               int blend, float alpha, unsigned flags) {
    if (width <= 0) return 0;
    VIVect2I cursor;
    cursor.x = pos.x;
    cursor.y = pos.y;
    const int* tex = GetUITexture__F9VIUITexId(texture);
    if (!tex) return -1;
    if (tex[1] == 1) {
        while (cursor.x + tex[4] < pos.x + width) {
            VIVect2I at;
            at.x = cursor.x;
            at.y = cursor.y;
            BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, texture, at, blend, alpha, flags);
            cursor.x = cursor.x + tex[4];
        }
        VIVect2I at;
        at.x = cursor.x;
        at.y = cursor.y;
        VIVect2I size;
        size.x = pos.x + width - cursor.x;
        size.y = tex[5];
        BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(ui, texture, at, size, blend, alpha, flags);
        return 0;
    }
    VIVect2I size;
    size.x = width;
    size.y = tex[5];
    VIVect2I at;
    at.x = cursor.x;
    at.y = cursor.y;
    VIVect2I used = size;
    BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(ui, texture, at, used, blend, alpha, flags);
    return 0;
}
}
