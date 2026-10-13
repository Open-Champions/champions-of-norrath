// Observed: ParseSurfaceObj__10VIESFParse at 0x01171640 is 996 bytes. Argument: parse (a0, the ESF
// parser record: creation-argument word at +0, progress-callback word at +4 and its argument word
// at +8, raster word at +12, dictionary word at +32, object file word at +36, and a scratch area
// from +40 that receives the palette). It uses a 2,048-byte row buffer on the stack. In order it:
//   1. when the raster word is zero it calls ReadEnd__9VIObjFile (result ignored) and returns 0;
//   2. calls the callback word with the argument word when it is non-zero;
//   3. reads an unsigned id; when it is non-zero Find__12VIDictionaryUiR14VIResourceTypeRi (dict,
//      id, &type, &handle) is asked: a hit with a type other than 1 returns -1, and type 1 calls
//      ReadEnd (ignored) and returns the found handle;
//   4. otherwise CreateSurface__8VIRasteri(raster, parse[0]) (negative returns -1),
//      Surface__8VIRasteri(raster, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, surface, id, 1, handle)
//      (negative returns -1);
//   5. reads four signed ints (width, height, depth, mip count) and calls
//      Init__9VISurfaceR8VIRasterii10VIBitDepthi(surface, raster, width, height, depth, mips)
//      (negative returns -1);
//   6. for depth values 0 and 1 (palettised) it reads an entry count, then count*4 bytes into
//      the scratch area at parse+40 (negative returns -1); when the surface word at +40 is non-zero
//      it calls LockPalette__9VISurfaceRiR12VIPixelStorePPUc(surface, &locked, &store, &data), then
//      Convert32__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, parse+40, data, min(count,
//      locked)) and UnlockPalette__9VISurface(surface), each negative result returning -1;
//   7. when the mip count is positive and the surface word at +64 is positive it loops over mip
//      levels while level < mips and level < surface[+64]: LockMipLevel__9VISurfaceiRiR12VIPixelStore
//      PPUc(surface, level, &pitch, &store, &data), a signed row byte count, then for each of the
//      current height rows a read of that many bytes into the row buffer and a conversion selected
//      by depth (0 Convert4, 1 Convert8, 2 Convert16, 3 Convert24, 4 Convert32, each taking (parse,
//      &store, row, data, width)); a depth above 4 returns -1; after each row data advances by the
//      pitch; then UnlockMipLevel__9VISurfacei(surface, level). Width and height are halved
//      (arithmetic shift) and clamped to 1 for the next level. Any negative result returns -1;
//   8. finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: resource type 1 means "surface"; the rows are stored top to bottom with one conversion
// call per row into the locked mip level; the surface words at +40 and +64 act as a palette
// presence flag and a mip-level count. Unknown: the sizes of VIPixelStore (at most four bytes: a
// 4-byte slot is reserved in the original frame) and VISurface, and the Convert routines' bodies.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
typedef void (*ProgressCallback)(void* argument);

struct SurfacePixelStore { unsigned char bytes[4]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFilePUci(void* file, unsigned char* buffer, int size);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSurface__8VIRasteri(void* raster, int argument);
void* Surface__8VIRasteri(void* raster, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int Init__9VISurfaceR8VIRasterii10VIBitDepthi(void* surface, void* raster, int width, int height, int depth, int mips);
int LockPalette__9VISurfaceRiR12VIPixelStorePPUc(void* surface, int* count, SurfacePixelStore* store, unsigned char** data);
int UnlockPalette__9VISurface(void* surface);
int LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(void* surface, int level, int* pitch, SurfacePixelStore* store, unsigned char** data);
int UnlockMipLevel__9VISurfacei(void* surface, int level);
int Convert4__10VIESFParseR12VIPixelStorePUcT2i(void* parse, SurfacePixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert8__10VIESFParseR12VIPixelStorePUcT2i(void* parse, SurfacePixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert16__10VIESFParseR12VIPixelStorePUcT2i(void* parse, SurfacePixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert24__10VIESFParseR12VIPixelStorePUcT2i(void* parse, SurfacePixelStore* store, unsigned char* source, unsigned char* destination, int count);
int Convert32__10VIESFParseR12VIPixelStorePUcT2i(void* parse, SurfacePixelStore* store, unsigned char* source, unsigned char* destination, int count);

int ParseSurfaceObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* raster = NORRATH_WORD_ADDRESS(words[3]);
    unsigned id;
    int type;
    int handle;
    int width;
    int height;
    int depth;
    int mips;
    SurfacePixelStore store;
    unsigned char* data;
    unsigned char row[2048];
    if (words[3] == 0) {
        ReadEnd__9VIObjFile(file);
        return 0;
    }
    if (words[1] != 0) reinterpret_cast<ProgressCallback>(NORRATH_WORD_ADDRESS(words[1]))(NORRATH_WORD_ADDRESS(words[2]));
    Read__9VIObjFileRUi(file, &id);
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 1) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSurface__8VIRasteri(raster, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* surface = static_cast<unsigned char*>(Surface__8VIRasteri(raster, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, surface, id, 1, handle) < 0) return -1;
    Read__9VIObjFileRi(file, &width);
    Read__9VIObjFileRi(file, &height);
    Read__9VIObjFileRi(file, &depth);
    Read__9VIObjFileRi(file, &mips);
    if (Init__9VISurfaceR8VIRasterii10VIBitDepthi(surface, raster, width, height, depth, mips) < 0) return -1;
    if (static_cast<unsigned>(depth) < 2) {
        int entries;
        int locked;
        Read__9VIObjFileRi(file, &entries);
        if (Read__9VIObjFilePUci(file, parse + 40, entries << 2) < 0) return -1;
        if (*reinterpret_cast<int*>(surface + 40) != 0) {
            if (LockPalette__9VISurfaceRiR12VIPixelStorePPUc(surface, &locked, &store, &data) < 0) return -1;
            if (Convert32__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, parse + 40, data, entries < locked ? entries : locked) < 0) return -1;
            if (UnlockPalette__9VISurface(surface) < 0) return -1;
        }
    }
    if (mips > 0 && *reinterpret_cast<int*>(surface + 64) > 0) {
        int level = 0;
        do {
            int pitch;
            int row_bytes;
            if (LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, level, &pitch, &store, &data) < 0) return -1;
            Read__9VIObjFileRi(file, &row_bytes);
            for (int y = 0; y < height; ++y) {
                if (Read__9VIObjFilePUci(file, row, row_bytes) < 0) return -1;
                int result;
                switch (depth) {
                case 0:
                    result = Convert4__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, width);
                    break;
                case 1:
                    result = Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, width);
                    break;
                case 2:
                    result = Convert16__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, width);
                    break;
                case 3:
                    result = Convert24__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, width);
                    break;
                case 4:
                    result = Convert32__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, row, data, width);
                    break;
                default:
                    return -1;
                }
                if (result < 0) return -1;
                data += pitch;
            }
            if (UnlockMipLevel__9VISurfacei(surface, level) < 0) return -1;
            width = (width >> 1) < 1 ? 1 : (width >> 1);
            height = (height >> 1) < 1 ? 1 : (height >> 1);
            ++level;
        } while (level < mips && level < *reinterpret_cast<int*>(surface + 64));
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
