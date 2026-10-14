// Observed inventoryDrawPlayerColor__Fiii at 0x001b1f08, 220 bytes (int color,
// int slot, int shop). A color number of 4 or more (unsigned compare) does
// nothing. Otherwise a texture is looked up with lumpFindResource__FPcT0 and
// drawn with textureDrawColor__FP8_textureiiiUi at x = slot * 320 + 131, y = 21,
// integer argument 1 and the color word 0x80808080:
//   * shop nonzero: lump "shop.lmp", otherwise lump "inventry.lmp";
//   * the resource is one of four tag textures chosen by the color number: 0
//     "invtagred.tex", 1 "invtaggreen.tex", 2 "invtagyellow.tex", 3
//     "invtagblue.tex" (the image copies the four string addresses into a local
//     array first).
// Unknown: what the tag textures mark on the screen, and what the argument value
// 1 of the draw means.
extern "C" {
void* lumpFindResource__FPcT0(char*, char*);
void textureDrawColor__FP8_textureiiiUi(void*, int, int, int, unsigned);

void inventoryDrawPlayerColor__Fiii(int color, int slot, int shop) {
    char* names[4] = {const_cast<char*>("invtagred.tex"), const_cast<char*>("invtaggreen.tex"),
                      const_cast<char*>("invtagyellow.tex"), const_cast<char*>("invtagblue.tex")};
    if (static_cast<unsigned>(color) < 4u) {
        void* texture = lumpFindResource__FPcT0(const_cast<char*>(shop ? "shop.lmp" : "inventry.lmp"), names[color]);
        textureDrawColor__FP8_textureiiiUi(texture, slot * 320 + 131, 21, 1, 0x80808080u);
    }
}
}
