// Observed: ParseFontObj__10VIESFParse at 0x01176e48 is 1,180 bytes. Argument: parse (a0, the ESF
// parser record: creation-argument word at +0, progress-callback word at +4 and its argument word
// at +8, raster word at +12, dictionary word at +32, object file word at +36, and a scratch area
// from +40 that receives the palette). It keeps a 256-byte row buffer on the stack. In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second) (negative returns -1, the tag is not
//      examined); reads an unsigned id, three signed ints (count, rows, depth) and, only when the
//      16-bit second output is non-zero, a fourth signed int (flags, otherwise 1); ReadEnd__9VIObjFile
//      (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle): a hit with a type other than 12 returns -1, and type 12 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. otherwise CreateFont__8VIRasteri(raster, parse[0]) (negative returns -1) and
//      Font__8VIRasteri(raster, handle) give the font record; a table of count bytes is allocated
//      with __builtin_vec_new (null returns -1), a header is read (ReadBegin, negative returns -1),
//      count bytes are read into the table (negative returns -1) and ReadEnd (negative returns -1);
//   4. Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi(font, raster, dict, count, table,
//      rows, depth, flags) (negative returns -1); for a non-zero id Add__12VIDictionaryR10VIResource
//      Ui14VIResourceTypei(dict, font, id, 12, handle) (negative returns -1); then Lock__7VIRFontR8
//      VIRaster(font, raster) (negative returns -1) and a header read (ReadBegin, negative returns
//      -1);
//   5. for depth 0 and 1 (palettised) it reads an entry count and count*4 bytes into the scratch
//      area at parse+40 (negative returns -1) and, if IsPalettized__7VIRFont(font) is non-zero, calls
//      Palette__7VIRFontRiR12VIPixelStorePPUc(font, &locked, &store, &data) and
//      Convert32__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, parse+40, data, min(entries,
//      locked)) (each negative returning -1);
//   6. it takes the pixel store from PixelStore__7VIRFont(font) (a four-byte value returned in v0)
//      and, for each of count glyphs, calls the progress callback (when set), reads an unsigned
//      16-bit character code and a signed row byte count, calls BeginChar__7VIRFontUsRiPPUc(font,
//      code, &pitch, &data) (negative returns -1) and, for each of rows rows, reads row-byte-count
//      bytes into the row buffer and converts them with the converter selected by depth (0 Convert4,
//      1 Convert8, 2 Convert16, 3 Convert24, 4 Convert32; called with (parse, &store, row, data,
//      table[glyph index])), a depth above 4 or a negative result returning -1, with data advancing
//      by the pitch after every row; then EndChar__7VIRFont(font) (negative returns -1);
//   7. finally ReadEnd (negative returns -1), Unlock__7VIRFontR8VIRaster(font, raster) (negative
//      returns -1), frees the table with __builtin_vec_delete and returns 0.
// Inferred: resource type 12 means "bitmap font"; the table is the per-glyph pixel width (it is
// passed as the pixel count of each row conversion); a successful parse returns 0, not the handle
// (only the dictionary hit returns the handle). The failure paths after the table is allocated
// leak it. Unknown: the roles of the count, rows and flags fields, field names, and the VIRFont,
// VIRaster and VIDictionary layouts (the callees are not authored here).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
typedef void (*ProgressCallback)(void* argument);

struct FontPixelStore { unsigned char bytes[4]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRUs(void* file, unsigned short* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFilePUci(void* file, unsigned char* buffer, int size);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateFont__8VIRasteri(void* raster, int argument);
void* Font__8VIRasteri(void* raster, int handle);
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi(void* font, void* raster, void* dictionary, int count, unsigned char* table, int rows, int depth, int flags);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int Lock__7VIRFontR8VIRaster(void* font, void* raster);
int Unlock__7VIRFontR8VIRaster(void* font, void* raster);
int IsPalettized__7VIRFont(void* font);
int Palette__7VIRFontRiR12VIPixelStorePPUc(void* font, int* count, FontPixelStore* store, unsigned char** data);
FontPixelStore PixelStore__7VIRFont(void* font);
int BeginChar__7VIRFontUsRiPPUc(void* font, unsigned short code, int* pitch, unsigned char** data);
int EndChar__7VIRFont(void* font);
int Convert4__10VIESFParseR12VIPixelStorePUcT2i(void* parse, FontPixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert8__10VIESFParseR12VIPixelStorePUcT2i(void* parse, FontPixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert16__10VIESFParseR12VIPixelStorePUcT2i(void* parse, FontPixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert24__10VIESFParseR12VIPixelStorePUcT2i(void* parse, FontPixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert32__10VIESFParseR12VIPixelStorePUcT2i(void* parse, FontPixelStore* store, unsigned char* source, unsigned char* destination, int count);

int ParseFontObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* raster = NORRATH_WORD_ADDRESS(words[3]);
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int type;
    int handle;
    int count;
    int rows;
    int depth;
    int flags = 1;
    FontPixelStore store;
    unsigned char* data;
    unsigned char row[256];
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRUi(file, &id);
    Read__9VIObjFileRi(file, &count);
    Read__9VIObjFileRi(file, &rows);
    Read__9VIObjFileRi(file, &depth);
    if (second != 0) Read__9VIObjFileRi(file, &flags);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 12) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateFont__8VIRasteri(raster, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    void* font = Font__8VIRasteri(raster, handle);
    unsigned char* table = static_cast<unsigned char*>(__builtin_vec_new(static_cast<unsigned>(count)));
    if (table == 0) return -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (Read__9VIObjFilePUci(file, table, count) < 0) return -1;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi(font, raster, dictionary, count, table, rows, depth, flags) < 0) return -1;
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, font, id, 12, handle) < 0) return -1;
    if (Lock__7VIRFontR8VIRaster(font, raster) < 0) return -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (static_cast<unsigned>(depth) < 2) {
        int entries;
        int locked;
        Read__9VIObjFileRi(file, &entries);
        if (Read__9VIObjFilePUci(file, parse + 40, entries << 2) < 0) return -1;
        if (IsPalettized__7VIRFont(font) != 0) {
            if (Palette__7VIRFontRiR12VIPixelStorePPUc(font, &locked, &store, &data) < 0) return -1;
            if (Convert32__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, parse + 40, data, entries < locked ? entries : locked) < 0) return -1;
        }
    }
    store = PixelStore__7VIRFont(font);
    for (int glyph = 0; glyph < count; ++glyph) {
        unsigned short code;
        int row_bytes;
        int pitch;
        if (words[1] != 0) reinterpret_cast<ProgressCallback>(NORRATH_WORD_ADDRESS(words[1]))(NORRATH_WORD_ADDRESS(words[2]));
        Read__9VIObjFileRUs(file, &code);
        Read__9VIObjFileRi(file, &row_bytes);
        if (BeginChar__7VIRFontUsRiPPUc(font, code, &pitch, &data) < 0) return -1;
        for (int y = 0; y < rows; ++y) {
            if (Read__9VIObjFilePUci(file, row, row_bytes) < 0) return -1;
            int result;
            switch (depth) {
            case 0:
                result = Convert4__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, table[glyph]);
                break;
            case 1:
                result = Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, table[glyph]);
                break;
            case 2:
                result = Convert16__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, table[glyph]);
                break;
            case 3:
                result = Convert24__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, table[glyph]);
                break;
            case 4:
                result = Convert32__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, table[glyph]);
                break;
            default:
                return -1;
            }
            if (result < 0) return -1;
            data += pitch;
        }
        if (EndChar__7VIRFont(font) < 0) return -1;
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (Unlock__7VIRFontR8VIRaster(font, raster) < 0) return -1;
    if (table != 0) __builtin_vec_delete(table);
    return 0;
}
}
