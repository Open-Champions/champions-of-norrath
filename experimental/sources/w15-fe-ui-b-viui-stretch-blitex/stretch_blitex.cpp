// Observed: StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef at 0x0102cb28, 284 bytes. Arguments: the
// UI object, a texture id, a destination position pair and destination size pair (both passed by reference), a
// blend mode and a float alpha. The texture record, name string, hash and dictionary lookup are exactly as in the
// BlitEx overloads (null record or failed Find: result -1; index preset to -1; type not initialised). On success it
// calls StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef(raster word at ui + 72, index, 0, the caller's
// position pair, the caller's size pair, source position (tex[2], tex[3]), source size (tex[4], tex[5]), blend,
// alpha) and returns that call's result unchanged.
// Inferred: the stretch variant maps the record's rectangle onto an arbitrary destination rectangle; the 0 is a
// flags word (the BlitEx overloads forward their flags here).
// Unknown: the result type of StretchBlit (an int is assumed), and the other unknowns of the BlitEx overloads.
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
int StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef(unsigned raster, int index, unsigned flags,
                                                                const VIVect2I& at, const VIVect2I& size,
                                                                const VIVect2I& source, const VIVect2I& source_size,
                                                                int blend, float alpha);

int StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(char* ui, int texture, VIVect2I at, VIVect2I size,
                                                                   int blend, float alpha) {
    const int* tex = GetUITexture__F9VIUITexId(texture);
    if (!tex) return -1;
    VIVect2I source;
    source.x = tex[2];
    source.y = tex[3];
    VIVect2I source_size;
    source_size.x = tex[4];
    source_size.y = tex[5];
    char name[64];
    name[0] = 0;
    str16to8__8VIStringPcUiPCUs(
        name, 64, GetString__13VIStringTableii(reinterpret_cast<VIStringTable*>(ui + 16), tex[1],
                                                *reinterpret_cast<int*>(ui + 48)));
    int type;
    int index = -1;
    if (!Find__12VIDictionaryUiR14VIResourceTypeRi(*reinterpret_cast<unsigned*>(ui + 80),
                                                    VIHashResourceID_Icon__FPCc(name), type, index))
        return -1;
    return StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef(*reinterpret_cast<unsigned*>(ui + 72), index, 0, at,
                                                                      size, source, source_size, blend, alpha);
}
}
