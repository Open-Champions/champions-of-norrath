// Observed: BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi at 0x0102c998, 400 bytes. Arguments: the UI
// object, a texture id, a position pair and a size pair (both passed by reference), a blend mode, a float alpha
// and a flags word. It differs from the position-only overload as follows. The effective size starts at (0, 0);
// its width is the size pair's x, replaced by tex[4] when that x is -1, and its height is the size pair's y,
// replaced by tex[5] when that y is -1. The source position is (tex[2], tex[3]) as before. The string, hash,
// dictionary lookup, index preset of -1, Blit call (position copy, source position, effective size) and the
// results (-1 for a null record or a failed Find, otherwise 0) are as in the position-only overload.
// Inferred: -1 in either component means "use the record's own extent".
// Unknown: as in the position-only overload.
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};
struct VIStringTable;

const int* GetUITexture__F9VIUITexId(int texture);
const unsigned short* GetString__13VIStringTableii(VIStringTable* table, int id, int extra);
void str16to8__8VIStringPcUiPCUs(char* destination, unsigned size, const unsigned short* source);
unsigned VIHashResourceID_Icon__FPCc(const char* name);
int Find__12VIDictionaryUiR14VIResourceTypeRi(unsigned dictionary, unsigned hash, int& type, int& index);
void Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(unsigned raster, int index, unsigned flags,
                                                         const VIVect2I& at, const VIVect2I& source,
                                                         const VIVect2I& size, int blend, float alpha);

int BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(char* ui, int texture, VIVect2I at, VIVect2I size,
                                                              int blend, float alpha, unsigned flags) {
    const int* tex = GetUITexture__F9VIUITexId(texture);
    if (!tex) return -1;
    VIVect2I extent;
    extent.x = 0;
    extent.y = 0;
    VIVect2I source;
    source.x = tex[2];
    source.y = tex[3];
    extent.x = size.x == -1 ? tex[4] : size.x;
    extent.y = size.y == -1 ? tex[5] : size.y;
    char name[64];
    name[0] = 0;
    str16to8__8VIStringPcUiPCUs(
        name, 64, GetString__13VIStringTableii(reinterpret_cast<VIStringTable*>(ui + 16), tex[1],
                                                *reinterpret_cast<int*>(ui + 48)));
    const unsigned hash = VIHashResourceID_Icon__FPCc(name);
    VIVect2I position;
    position.x = at.x;
    position.y = at.y;
    VIVect2I used = extent;
    int type;
    int index = -1;
    if (!Find__12VIDictionaryUiR14VIResourceTypeRi(*reinterpret_cast<unsigned*>(ui + 80), hash, type, index)) return -1;
    Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(*reinterpret_cast<unsigned*>(ui + 72), index, flags, position,
                                                        source, used, blend, alpha);
    return 0;
}
}
