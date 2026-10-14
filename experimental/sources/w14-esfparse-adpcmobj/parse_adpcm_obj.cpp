// Observed: ParseAdpcmObj__10VIESFParse at 0x011736a0 is 420 bytes. Argument: parse (a0, the ESF
// parser record: progress-callback word at +4 and its argument word at +8, dictionary word at +32,
// object file word at +36). In order it:
//   1. calls the callback word with the argument word when it is non-zero;
//   2. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1; a tag other than
//      0xb010 calls ReadEnd__9VIObjFile and returns -1;
//   3. reads an unsigned id, three signed ints (a, b, c), two floats and one more signed int into
//      locals, then ReadEnd (negative returns -1);
//   4. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type,
//      &handle); a hit with a type other than 14 returns -1, and type 14 calls ReadEnd (ignored)
//      and returns the found handle;
//   5. otherwise it reads a second header with ReadBegin (negative returns -1); a tag other than
//      0xb020 calls ReadEnd and returns -1. It allocates ((b * a) << 4) bytes with
//      __builtin_vec_new, reads that many bytes from the file into it with
//      Read__9VIObjFilePUci, frees it with __builtin_vec_delete when the allocation is non-null,
//      then ReadEnd twice (the first negative returns -1; the second one decides between -1 and
//      the handle local).
// Inferred: this is the ADPCM sound resource record (0xb010 header, 0xb020 sample data of 16-byte
// blocks); the parser only skips its payload and does not hand the data to anything. The handle
// local is only written by Find, so for id == 0 the original returns whatever its stack slot held;
// this version returns 0 in that case. Unknown: the meaning of a, b, c and the floats.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
typedef void (*ProgressCallback)(void* argument);

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int Read__9VIObjFilePUci(void* file, unsigned char* buffer, int size);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);

int ParseAdpcmObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned callback = words[1];
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int a;
    int b;
    int c;
    float f0;
    float f1;
    int d;
    int type;
    int handle = 0;
    if (callback != 0) reinterpret_cast<ProgressCallback>(NORRATH_WORD_ADDRESS(callback))(NORRATH_WORD_ADDRESS(words[2]));
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0xb010) {
        ReadEnd__9VIObjFile(file);
        return -1;
    }
    Read__9VIObjFileRUi(file, &id);
    Read__9VIObjFileRi(file, &a);
    Read__9VIObjFileRi(file, &b);
    Read__9VIObjFileRi(file, &c);
    Read__9VIObjFileRf(file, &f0);
    Read__9VIObjFileRf(file, &f1);
    Read__9VIObjFileRi(file, &d);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(NORRATH_WORD_ADDRESS(words[8]), id, &type, &handle) != 0) {
        if (type != 14) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0xb020) {
        ReadEnd__9VIObjFile(file);
        return -1;
    }
    int size = (b * a) << 4;
    unsigned char* buffer = static_cast<unsigned char*>(__builtin_vec_new(static_cast<unsigned>(size)));
    Read__9VIObjFilePUci(file, buffer, size);
    if (buffer != 0) __builtin_vec_delete(buffer);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
