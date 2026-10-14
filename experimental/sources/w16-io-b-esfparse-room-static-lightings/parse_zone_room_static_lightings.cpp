// Observed: ParseZoneRoomStaticLightingsObj__10VIESFParse at 0x01175b68 is 332 bytes and
// returns an int. Same shape as ParseZoneRoomActorsObj__10VIESFParse. Argument: parse
// (object file word at +36, current-zone word at +28). Any negative result returns -1:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second); the tag must be 0x32a0.
//   2. Read__9VIObjFileRi(file, &index) (result ignored), then ReadEnd__9VIObjFile(file).
//   3. room = (word at zone + 96) + index * 120. ReadBegin again; the tag must be 0x6030.
//   4. The array at room + 68 is sized with Init__t7VIArray1Zii(room + 68, n) where n =
//      NumSubObjects__9VIObjFile(file).
//   5. For i from 0 while i < NumSubObjects(file) (called before every test):
//      value = ParseStaticLighting__10VIESFParseRt7VIArray1Zi(parse, room + 60); it is stored
//      to the word at (data word at room + 72) + i * 4 before the sign is tested, and a
//      negative value returns -1.
//   6. ReadEnd (negative returns -1), then ReadEnd again; its sign gives the result.
// Inferred: tag 0x32a0 holds the room index and 0x6030 the room's static lighting list;
// the array at room + 60 is shared between all calls and collects what each lighting
// parse produces, while the array at +68 gets one result word per lighting.
// Unknown: field names of the room record, what the lighting parse stores into the array
// at +60, and the array layout beyond its data word at +4.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int Read__9VIObjFileRi(void* file, int* value);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
int Init__t7VIArray1Zii(void* array, int count);
int ParseStaticLighting__10VIESFParseRt7VIArray1Zi(void* parse, void* array);

int ParseZoneRoomStaticLightingsObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int index;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x32a0) return -1;
    Read__9VIObjFileRi(file, &index);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    unsigned char* zone = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7]));
    unsigned char* room = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(zone + 96) + index * 120));
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x6030) return -1;
    unsigned char* array = room + 68;
    if (Init__t7VIArray1Zii(array, NumSubObjects__9VIObjFile(file)) < 0) return -1;
    for (int i = 0; i < NumSubObjects__9VIObjFile(file); ++i) {
        int value = ParseStaticLighting__10VIESFParseRt7VIArray1Zi(parse, room + 60);
        *reinterpret_cast<int*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(array + 4))) + i * 4) = value;
        if (value < 0) return -1;
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
