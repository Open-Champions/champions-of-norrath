// Observed: BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi at 0x0102c830, 356 bytes. Arguments: the UI
// object, a texture id, a by-value pair of ints (passed by reference), a blend mode, a float alpha and an
// unsigned flags word. tex = GetUITexture__F9VIUITexId(id); when it is null the result is -1. Otherwise the
// record words 2 and 3 form a source position, words 4 and 5 a source size, and word 1 selects a string:
// GetString__13VIStringTableii(table at ui + 16, tex[1], the int at ui + 48) is converted into a 64-byte buffer
// (first byte cleared beforehand) with str16to8__8VIStringPcUiPCUs(buffer, 64, string), and
// VIHashResourceID_Icon__FPCc(buffer) gives a hash. Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary word at
// ui + 80, hash, &type, &index) is called with index preset to -1; type is not initialised. When Find returns zero
// the result is -1. Otherwise Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(raster word at ui + 72, index,
// flags, copy of the destination pair, source position, source size, blend, alpha) is called and the result is 0.
// Inferred: the hash names a resource (an icon or sheet image) in the dictionary; the call blits the record's
// rectangle at the given position with its natural size. Callers pass flags 0 or 1 (see DrawFrame).
// Unknown: the Blit and Find result types (the Blit result is ignored; Find is only tested for zero), the meaning
// of the third int at ui + 48, and VIResourceType (an int stands in).
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

int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(char* ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags) {
    const int* tex = GetUITexture__F9VIUITexId(texture);
    if (!tex) return -1;
    VIVect2I source;
    source.x = tex[2];
    source.y = tex[3];
    VIVect2I size;
    size.x = tex[4];
    size.y = tex[5];
    char name[64];
    name[0] = 0;
    str16to8__8VIStringPcUiPCUs(
        name, 64, GetString__13VIStringTableii(reinterpret_cast<VIStringTable*>(ui + 16), tex[1],
                                                *reinterpret_cast<int*>(ui + 48)));
    const unsigned hash = VIHashResourceID_Icon__FPCc(name);
    VIVect2I position;
    position.x = at.x;
    position.y = at.y;
    int type;
    int index = -1;
    if (!Find__12VIDictionaryUiR14VIResourceTypeRi(*reinterpret_cast<unsigned*>(ui + 80), hash, type, index)) return -1;
    Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef(*reinterpret_cast<unsigned*>(ui + 72), index, flags, position,
                                                        source, size, blend, alpha);
    return 0;
}
}
