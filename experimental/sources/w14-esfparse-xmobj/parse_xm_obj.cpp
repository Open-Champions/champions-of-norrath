// Observed: ParseXmObj__10VIESFParse at 0x01173848 is 472 bytes. Argument: parse (a0, the ESF
// parser record: progress-callback word at +4 and its argument word at +8, dictionary word at +32,
// object file word at +36). In order it:
//   1. calls the callback word with the argument word when it is non-zero;
//   2. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1; a tag other than
//      0xb040 calls ReadEnd__9VIObjFile and returns -1;
//   3. reads an unsigned id; when the 16-bit second output is non-zero it also reads two floats
//      into locals that start as 1.0 and 0.0 (they are not used again);
//   4. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type,
//      &handle); a hit with a type other than 14 returns -1, and type 14 calls ReadEnd twice
//      (results ignored) and returns the found handle;
//   5. otherwise it allocates ObjectSize__9VIObjFile(file) - 12 bytes with __builtin_vec_new, reads
//      that many bytes into it with Read__9VIObjFilePUci, calls ReadEnd (negative returns -1,
//      leaving the buffer allocated), and checks the first word of the buffer: a non-zero word
//      frees the buffer and returns -1; a zero word keeps it (it is not freed or stored);
//   6. it then reads a second header with ReadBegin (negative returns -1; a tag other than 0xb060
//      returns -1 without ReadEnd), allocates ObjectSize(file) bytes, reads them, calls ReadEnd
//      (negative returns -1), then ReadEnd again, returning -1 when that is negative, else the
//      handle local.
// Inferred: this is the XM module resource record (0xb040 header, then two data blocks); the data
// is read and dropped, nothing is registered, and the first word of the first block acts as an
// error flag. The handle local is only written by Find, so for id == 0 the original returns
// whatever its stack slot held; this version returns 0 in that case. Unknown: what the 12-byte
// header and the first-word flag mean, and why the buffers are not released or kept.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
typedef void (*ProgressCallback)(void* argument);

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int Read__9VIObjFilePUci(void* file, unsigned char* buffer, int size);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int ObjectSize__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);

int ParseXmObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned callback = words[1];
    unsigned short tag;
    unsigned short second;
    unsigned id;
    float f0 = 1.0f;
    float f1 = 0.0f;
    int type;
    int handle = 0;
    if (callback != 0) reinterpret_cast<ProgressCallback>(NORRATH_WORD_ADDRESS(callback))(NORRATH_WORD_ADDRESS(words[2]));
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0xb040) {
        ReadEnd__9VIObjFile(file);
        return -1;
    }
    Read__9VIObjFileRUi(file, &id);
    if (second != 0) {
        Read__9VIObjFileRf(file, &f0);
        Read__9VIObjFileRf(file, &f1);
    }
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(NORRATH_WORD_ADDRESS(words[8]), id, &type, &handle) != 0) {
        if (type != 14) return -1;
        ReadEnd__9VIObjFile(file);
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    int size = ObjectSize__9VIObjFile(file) - 12;
    unsigned* buffer = static_cast<unsigned*>(__builtin_vec_new(static_cast<unsigned>(size)));
    Read__9VIObjFilePUci(file, reinterpret_cast<unsigned char*>(buffer), size);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (buffer[0] != 0) {
        __builtin_vec_delete(buffer);
        return -1;
    }
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0xb060) return -1;
    size = ObjectSize__9VIObjFile(file);
    buffer = static_cast<unsigned*>(__builtin_vec_new(static_cast<unsigned>(size)));
    Read__9VIObjFilePUci(file, reinterpret_cast<unsigned char*>(buffer), size);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
