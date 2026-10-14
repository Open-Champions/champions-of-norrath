// Observed: Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii at 0x0103efa8 is 204 bytes. Arguments are
// (this, ui, position, size, a, b, c), the vectors being pairs of words read through pointers.
// It stores ui at this+0 and calls GetHeight__6VIFontP8VIRasterP12VIDictionary with (ui+56,
// word at ui+72, word at ui+80). It copies position to this+4/+8 and size to this+12/+16, stores a
// at this+20, 18 at this+28, b at this+32, and ((height - 4) * b + 36) at this+24, stores c at
// this+360, calls SetUiColors__10VIUIWindow(this), then stores 1 at this+36 and returns 0.
// Inferred: this+24 is a pixel height built from b text lines of the font height plus 36 of
// frame; ui+56 is an embedded font object, ui+72 and ui+80 are its raster and dictionary.
// Unknown: meaning of a, c and the constant 18 (the first two are only stored).
extern "C" {
int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary);
void SetUiColors__10VIUIWindow(void* self);

struct Vect2I {
    int x, y;
};

int Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* self, void* ui, const Vect2I* position, const Vect2I* size, int a,
                                              int b, int c) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* ui_bytes = static_cast<unsigned char*>(ui);
    *reinterpret_cast<unsigned*>(bytes) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(ui));
    const int height = GetHeight__6VIFontP8VIRasterP12VIDictionary(
        ui_bytes + 56,
        reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(ui_bytes + 72))),
        reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(ui_bytes + 80))));
    *reinterpret_cast<Vect2I*>(bytes + 4) = *position;
    *reinterpret_cast<Vect2I*>(bytes + 12) = *size;
    *reinterpret_cast<int*>(bytes + 20) = a;
    *reinterpret_cast<int*>(bytes + 28) = 18;
    *reinterpret_cast<int*>(bytes + 24) = (height - 4) * b + 36;
    *reinterpret_cast<int*>(bytes + 32) = b;
    *reinterpret_cast<int*>(bytes + 360) = c;
    SetUiColors__10VIUIWindow(self);
    *reinterpret_cast<int*>(bytes + 36) = 1;
    return 0;
}
}
