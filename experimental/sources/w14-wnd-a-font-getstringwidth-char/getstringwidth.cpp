// Observed: GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc at 0x01012010 is 140 bytes. With (this, raster, dictionary,
// text) it returns 0 when dictionary is null. Otherwise it hashes the 8-byte name _6VIFont$sm_FontName with
// VIHashResourceID, stores that hash in a function-local 4-byte word (it is never read here) and calls
// FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, hash, 12, &found). When that returns zero the result is 0;
// otherwise it is GetStringWidth__8VIRasteriPCc(raster, found, text).
// Observed: the name bytes in the image are the string "ArialUI". this is unused.
// Inferred: type 12 is the font resource type and found is a font id inside the dictionary.
// Unknown: the name of the local word and of type 12, and whether the result is a pixel width in pixels (it is
// forwarded unchanged from the raster).
extern "C" {
extern const char _6VIFont$sm_FontName[];
unsigned VIHashResourceID(const char* name);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* out);
int GetStringWidth__8VIRasteriPCc(void* raster, int font, const char* text);

int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void* self, void* raster, void* dictionary, const char* text) {
    (void)self;
    int width = 0;
    if (dictionary) {
        static volatile unsigned ResourceID;
        int found;
        const unsigned id = VIHashResourceID(_6VIFont$sm_FontName);
        ResourceID = id;
        if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id, 12, &found))
            width = GetStringWidth__8VIRasteriPCc(raster, found, text);
    }
    return width;
}
}
