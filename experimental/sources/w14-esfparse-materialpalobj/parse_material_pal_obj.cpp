// Observed: ParseMaterialPalObj__10VIESFParse at 0x01171a28 is 644 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, raster word at +12, dictionary word at +32,
// object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1, and so does a tag
//      other than 0x1111 (without ReadEnd); reads an unsigned id; ReadEnd__9VIObjFile (negative
//      returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 3 returns -1, and type 3 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. when ObjectVersion__9VIObjFile(file) is non-zero it expects a 0x1001 header (otherwise -1)
//      and then, once per NumSubObjects__9VIObjFile(file) (re-read before each pass), a header
//      with tag 0x1000 followed by ParseSurfaceObj__10VIESFParse(parse); any failure returns -1;
//      the 0x1001 block is closed with ReadEnd (negative returns -1);
//   4. ReadBegin again (negative returns -1, as does a tag other than 0x1101); n =
//      NumSubObjects(file); handle = CreateMaterialPal__8VIRasterii(raster, n, parse[0]) (negative
//      returns -1); for a non-zero id the palette record from MaterialPal__8VIRasteri(raster,
//      handle) is registered with Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict,
//      record, id, 3, handle) (negative returns -1);
//   5. for each of the n entries it reads a header (a tag other than 0x1100 returns -1; a negative
//      ReadBegin result is not checked on its own because the stale tag is then compared), gets the
//      entry with GetMaterialPalEntry__8VIRasterii(raster, handle, index) (null returns -1) and
//      calls ParseMaterial__10VIESFParseP10VIMaterial(parse, entry) (negative returns -1);
//   6. ReadEnd (negative returns -1), a second ReadEnd and the handle (-1 when that is negative).
// Inferred: resource type 3 means "material palette"; 0x1111 is the palette container tag, 0x1001
// the optional surface array of old (version != 0) files, 0x1101 the entry list and 0x1100 each
// entry. Unknown: field names, and the note in step 5 only records that the original compares the
// tag even when ReadBegin failed (the tag then holds whatever it held before).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
int ObjectVersion__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int ParseSurfaceObj__10VIESFParse(void* parse);
int CreateMaterialPal__8VIRasterii(void* raster, int count, int argument);
void* MaterialPal__8VIRasteri(void* raster, int handle);
void* GetMaterialPalEntry__8VIRasterii(void* raster, int handle, int index);
int ParseMaterial__10VIESFParseP10VIMaterial(void* parse, void* material);

int ParseMaterialPalObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* raster = NORRATH_WORD_ADDRESS(words[3]);
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x1111) return -1;
    Read__9VIObjFileRUi(file, &id);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 3) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    if (ObjectVersion__9VIObjFile(file) != 0) {
        unsigned short array_tag;
        unsigned short array_second;
        if (ReadBegin__9VIObjFileRUsT1(file, &array_tag, &array_second) < 0) return -1;
        if (array_tag != 0x1001) return -1;
        for (int i = 0; i < NumSubObjects__9VIObjFile(file); ++i) {
            unsigned short surface_tag;
            unsigned short surface_second;
            if (ReadBegin__9VIObjFileRUsT1(file, &surface_tag, &surface_second) < 0) return -1;
            if (surface_tag != 0x1000) return -1;
            if (ParseSurfaceObj__10VIESFParse(parse) < 0) return -1;
        }
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
    }
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x1101) return -1;
    int count = NumSubObjects__9VIObjFile(file);
    handle = CreateMaterialPal__8VIRasterii(raster, count, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    void* record = MaterialPal__8VIRasteri(raster, handle);
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, record, id, 3, handle) < 0) return -1;
    for (int i = 0; i < count; ++i) {
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        if (tag != 0x1100) return -1;
        void* entry = GetMaterialPalEntry__8VIRasterii(raster, handle, i);
        if (entry == 0) return -1;
        if (ParseMaterial__10VIESFParseP10VIMaterial(parse, entry) < 0) return -1;
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
