// Observed: ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem at 0x01176d48 is 252 bytes.
// Arguments: parse (a0, the ESF parser record: object file word at +36) and table (a1, an array
// object whose element pointer sits at +4 and whose elements are 16 bytes). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative, or a tag other than 0x9000,
//      returns -1;
//   2. reads a signed count (Read__9VIObjFileRi) and calls Init__t7VIArray1Z14VIResourceElemi(table,
//      count); a negative result returns -1;
//   3. for each index below the count (the count local and the element pointer at table+4 are
//      read again every iteration): Read__9VIObjFileRUl into the element at +0, then
//      Read__9VIObjFileRUi into +8 and into +12 (results ignored);
//   4. returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: 0x9000 is the resource table tag and each element holds three file-supplied values,
// presumably an identifier and two more fields; the +4 word is the array's element pointer. The first
// value is read through the Ul overload; other code in the image passes an Ul argument loaded as a
// doubleword (ParseWorldZones), so the EE long is probably 8 bytes and that value fills element
// bytes +0..+7, which explains the 16-byte element (the build flags here use a 4-byte long, only
// the pointer is passed, so the compiled call is the same).
// Unknown: the element field names, and the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRUl(void* file, unsigned long* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Init__t7VIArray1Z14VIResourceElemi(void* table, int count);

int ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(unsigned char* parse, unsigned char* table) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x9000) return -1;
    Read__9VIObjFileRi(file, &count);
    if (Init__t7VIArray1Z14VIResourceElemi(table, count) < 0) return -1;
    for (int i = 0; i < count; ++i) {
        unsigned* data = reinterpret_cast<unsigned*>(table + 4);
        Read__9VIObjFileRUl(file, static_cast<unsigned long*>(NORRATH_WORD_ADDRESS(*data + 16 * i)));
        Read__9VIObjFileRUi(file, static_cast<unsigned*>(NORRATH_WORD_ADDRESS(*data + 16 * i + 8)));
        Read__9VIObjFileRUi(file, static_cast<unsigned*>(NORRATH_WORD_ADDRESS(*data + 16 * i + 12)));
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
