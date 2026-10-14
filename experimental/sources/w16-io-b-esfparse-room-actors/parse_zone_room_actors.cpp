// Observed: ParseZoneRoomActorsObj__10VIESFParse at 0x011758a0 is 324 bytes and returns an
// int. Argument: parse (object file word at +36, current-zone word at +28). Steps, any
// negative result returning -1:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second); the tag must be 0x3280.
//   2. Read__9VIObjFileRi(file, &index) (result ignored), then ReadEnd__9VIObjFile(file).
//   3. room = (word at zone + 96) + index * 120, where zone is the word at parse + 28.
//      ReadBegin again; the tag must be 0x6040.
//   4. The array at room + 60 is sized with Init__t7VIArray1Zii(room + 60, n) where n =
//      NumSubObjects__9VIObjFile(file); negative returns -1.
//   5. For i from 0 while i < NumSubObjects(file) (called before every test):
//      value = ParseZoneActor__10VIESFParse(parse); it is stored to the word at
//      (data word at array + 4) + i * 4 before the sign is tested, and a negative value
//      returns -1.
//   6. ReadEnd (negative returns -1), then ReadEnd again; its sign gives the result
//      (-1 for negative, 0 otherwise).
// Inferred: tag 0x3280 holds an index of the room within the zone's room table (records are
// 120 bytes), tag 0x6040 the room's actor list; the array elements are actor handles.
// Unknown: the field names of the room record, what ParseZoneActor returns on success, and
// the layout of the array beyond the data word at +4.

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
int ParseZoneActor__10VIESFParse(void* parse);

int ParseZoneRoomActorsObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int index;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x3280) return -1;
    Read__9VIObjFileRi(file, &index);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    unsigned char* zone = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7]));
    unsigned char* room = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(zone + 96) + index * 120));
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x6040) return -1;
    unsigned char* array = room + 60;
    if (Init__t7VIArray1Zii(array, NumSubObjects__9VIObjFile(file)) < 0) return -1;
    for (int i = 0; i < NumSubObjects__9VIObjFile(file); ++i) {
        int value = ParseZoneActor__10VIESFParse(parse);
        *reinterpret_cast<int*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(array + 4))) + i * 4) = value;
        if (value < 0) return -1;
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
