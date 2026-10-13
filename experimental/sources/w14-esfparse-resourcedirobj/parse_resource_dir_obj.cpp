// Observed: ParseResourceDirObj__10VIESFParseiR13VIResourceDir at 0x01176c60 is 228 bytes.
// Arguments: parse (a0, the ESF parser record, object file word at +36), a flag (a1) and the
// resource directory array to fill (a2, passed through by reference). In order it:
//   1. calls ReadBegin__9VIObjFileRUsT1(file, &tag, &second) and returns -1 when that is negative
//      or when the 16-bit tag is not 0xa010;
//   2. when the flag is zero it takes n = NumSubObjects__9VIObjFile(file) once and calls
//      ParseObject__10VIESFParse(parse) n times (never when n is not positive), returning -1 at
//      once when a call returns a negative value; a non-zero flag skips these nested objects;
//   3. calls ReadEnd__9VIObjFile(file) and returns -1 when negative;
//   4. calls ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(parse, array) and
//      returns -1 when negative; then a second ReadEnd, returning -1 when negative, else 0.
// Inferred: 0xa010 is the resource directory body tag inside the 0xa000 container; the nested
// objects are the resources the directory lists, and the table maps names to them. The flag name
// is unknown. Unknown: the meaning of the second ReadBegin output (never read).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
int ParseObject__10VIESFParse(void* parse);
int ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(void* parse, void* array);

int ParseResourceDirObj__10VIESFParseiR13VIResourceDir(unsigned char* parse, int flag, void* array) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0xa010) return -1;
    if (flag == 0) {
        int count = NumSubObjects__9VIObjFile(file);
        for (int i = 0; i < count; ++i) {
            if (ParseObject__10VIESFParse(parse) < 0) return -1;
        }
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem(parse, array) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
