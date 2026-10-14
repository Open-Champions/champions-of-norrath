// Observed: ParseCSpritePlayList__10VIESFParseR9VICSprite at 0x01174380 is 772 bytes. Arguments:
// parse (a0; sound device word at +16, dictionary word at +32, object file word at +36) and
// sprite (a1, a character sprite). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &version): negative, or a tag other than 0x2910,
//      returns -1;
//   2. reads a signed count (Read__9VIObjFileRi); then for each index below the count a record,
//      starting from defaults (ids 0, the floats G and I as 1.0, H and J as 0.0): an unsigned
//      id A (Read__9VIObjFileRUi); when the 16-bit version is non-zero, an unsigned id B; a signed slot index; a float F; a signed value V; then, when the version is 2
//      or more, an unsigned id C, a float G, (version 3 or more: a float H), an unsigned id D, a
//      float I and (version 3 or more) a float J;
//   3. ids A and B, when non-zero, are resolved with
//      FindTyped__12VIDictionaryUi14VIResourceTypeRi(dict, id, 6, &handle) and ids C and D, only
//      when the sound device word is non-zero and the id is non-zero, with type 14; a handle
//      stays -1 when its id was zero (or, for C and D, when the sound device word is zero), and a
//      zero lookup result returns -1 (no ReadEnd);
//   4. the record is stored at sprite + 52 * slot (the slot is not range checked):
//      +424 handle A, +428 handle B, +432 F, +436 V, +448 handle C, +452 handle D, +456 G, +460 I,
//      +464 H, +468 J (unresolved handles are -1);
//   5. returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: this fills a table of 52-byte per-slot play entries (two animation-type resources of
// type 6, two sound resources of type 14, and float parameters; version 2 added the second sound
// and its float, version 3 added two more floats). Unknown: the names of the table fields and of
// resource types 6 and 14 (14 is used for sounds by other parsers), and the table size.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* handle);

int ParseCSpritePlayList__10VIESFParseR9VICSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    unsigned short tag;
    unsigned short version;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &version) < 0) return -1;
    if (tag != 0x2910) return -1;
    Read__9VIObjFileRi(file, &count);
    for (int i = 0; i < count; ++i) {
        unsigned id_a = 0, id_b = 0, id_c = 0, id_d = 0;
        float g = 1.0f, i_value = 1.0f, h = 0.0f, j = 0.0f;
        float f;
        int slot;
        int value;
        int handle_a = -1, handle_b = -1, handle_c = -1, handle_d = -1;
        Read__9VIObjFileRUi(file, &id_a);
        if (version != 0) Read__9VIObjFileRUi(file, &id_b);
        Read__9VIObjFileRi(file, &slot);
        Read__9VIObjFileRf(file, &f);
        Read__9VIObjFileRi(file, &value);
        if (version >= 2) {
            Read__9VIObjFileRUi(file, &id_c);
            Read__9VIObjFileRf(file, &g);
            if (version >= 3) Read__9VIObjFileRf(file, &h);
            Read__9VIObjFileRUi(file, &id_d);
            Read__9VIObjFileRf(file, &i_value);
            if (version >= 3) Read__9VIObjFileRf(file, &j);
        }
        if (id_a != 0) {
            if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id_a, 6, &handle_a) == 0) return -1;
        }
        if (id_b != 0) {
            if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id_b, 6, &handle_b) == 0) return -1;
        }
        if (words[4] != 0) {
            if (id_c != 0) {
                if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id_c, 14, &handle_c) == 0) return -1;
            }
            if (id_d != 0) {
                if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id_d, 14, &handle_d) == 0) return -1;
            }
        }
        unsigned char* entry = sprite + 52 * slot;
        *reinterpret_cast<float*>(entry + 432) = f;
        *reinterpret_cast<int*>(entry + 424) = handle_a;
        *reinterpret_cast<float*>(entry + 456) = g;
        *reinterpret_cast<int*>(entry + 428) = handle_b;
        *reinterpret_cast<float*>(entry + 460) = i_value;
        *reinterpret_cast<int*>(entry + 448) = handle_c;
        *reinterpret_cast<float*>(entry + 464) = h;
        *reinterpret_cast<int*>(entry + 436) = value;
        *reinterpret_cast<int*>(entry + 452) = handle_d;
        *reinterpret_cast<float*>(entry + 468) = j;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
