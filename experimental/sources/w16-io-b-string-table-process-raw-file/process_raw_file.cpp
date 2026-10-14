// Observed: ProcessRawFile__13VIStringTableRCt7VIArray1Zc at 0x0101b920 is 600 bytes and
// returns an int. Arguments: the string table (this; its word at +8 receives the text
// buffer) and a byte array (a count word at +0 and a data pointer word at +4). It first
// runs a once-only setup: when the guard word (a local at 0x0118e6a8) is zero it stores
// 0xfffe in the halfword at 0x011e31d0 and sets the guard. The count n and data then pick
// one of four paths:
//   - n >= 3 and the first three bytes are EF BB BF (the local table at 0x011c4360): the
//     rest is UTF-8. len = UTF8_strlen__FPCci(data + 3, n - 3); the buffer is
//     __builtin_vec_new(len * 2 + 2) stored at this + 8 (before the null test); null
//     returns -1; UTF8_convertToUTF16__FPCcPUsi(data + 3, buffer, len) must return len,
//     otherwise 1 is returned; success returns 0.
//   - n >= 2 and the first two bytes are FF FE (local table at 0x011c4364): little-endian
//     UTF-16. count = n / 2 - 1 halfwords; the buffer is __builtin_vec_new(count * 2 + 2);
//     memcpy(buffer, data + 2, count * 2); a zero halfword ends it; result 0, or -1 on a
//     null buffer.
//   - n >= 2 and the first two bytes are FE FF (the halfword stored by the setup): the
//     same size, but each halfword is byte-swapped while copied (a copy loop that does
//     not run when count <= 0), then the zero halfword; result 0, or -1 on null.
//   - otherwise (including n < 2): plain 8-bit text. size = n * 2 + 2; buffer =
//     __builtin_vec_new(size), stored at this + 8, null returns -1;
//     str8to16__8VIStringPUsUiPCc(buffer, size, data); result 0.
// Inferred: this turns a raw localisation file into a UTF-16 buffer, detecting a byte order
// mark. The 1252 table in str8to16 handles the plain case.
// Unknown: the names of the table fields, why a UTF-8 length mismatch returns 1 rather than
// -1, and whether n is always a byte count (an odd n in the UTF-16 paths drops the last
// byte).

// Words that hold 32-bit target addresses: a host check redefines these two macros.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
#ifndef NORRATH_ADDRESS_WORD
#define NORRATH_ADDRESS_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

extern "C" {
void* __builtin_vec_new(unsigned size);
int memcmp(const void* a, const void* b, __SIZE_TYPE__ count);
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);
int UTF8_strlen__FPCci(const char* text, int count);
int UTF8_convertToUTF16__FPCcPUsi(const char* text, unsigned short* destination, int count);
unsigned short* str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* text);

int ProcessRawFile__13VIStringTableRCt7VIArray1Zc(unsigned char* table, unsigned char* raw) {
    static const unsigned char utf8Header[3] = {0xef, 0xbb, 0xbf};
    static const unsigned char ucs2Header[2] = {0xff, 0xfe};
    static unsigned short ucs2HeaderReverse;
    static int reverseReady;
    if (reverseReady == 0) {
        ucs2HeaderReverse = 0xfffe;
        reverseReady = 1;
    }
    unsigned* words = reinterpret_cast<unsigned*>(table);
    int count = *reinterpret_cast<int*>(raw);
    const char* data = static_cast<const char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(raw + 4)));
    if (count >= 3 && memcmp(utf8Header, data, 3) == 0) {
        const char* text = data + 3;
        int length = UTF8_strlen__FPCci(text, count - 3);
        unsigned short* buffer = static_cast<unsigned short*>(__builtin_vec_new(length * 2 + 2));
        words[2] = NORRATH_ADDRESS_WORD(buffer);
        if (buffer == 0) return -1;
        return UTF8_convertToUTF16__FPCcPUsi(text, buffer, length) != length ? 1 : 0;
    }
    count = *reinterpret_cast<int*>(raw);
    if (count >= 2 && memcmp(ucs2Header, data, 2) == 0) {
        const char* text = data + 2;
        int bytes = (count / 2 - 1) * 2;
        unsigned short* buffer = static_cast<unsigned short*>(__builtin_vec_new(bytes + 2));
        words[2] = NORRATH_ADDRESS_WORD(buffer);
        if (buffer == 0) return -1;
        memcpy(buffer, text, bytes);
        *reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[2])) + bytes) = 0;
        return 0;
    }
    count = *reinterpret_cast<int*>(raw);
    if (count >= 2 && memcmp(&ucs2HeaderReverse, data, 2) == 0) {
        const unsigned short* text = reinterpret_cast<const unsigned short*>(data + 2);
        int halfwords = count / 2 - 1;
        int bytes = halfwords * 2;
        unsigned short* buffer = static_cast<unsigned short*>(__builtin_vec_new(bytes + 2));
        words[2] = NORRATH_ADDRESS_WORD(buffer);
        if (buffer == 0) return -1;
        for (int i = 0; i < halfwords; ++i) buffer[i] = static_cast<unsigned short>((text[i] << 8) | (text[i] >> 8));
        *reinterpret_cast<unsigned short*>(reinterpret_cast<unsigned char*>(buffer) + bytes) = 0;
        return 0;
    }
    count = *reinterpret_cast<int*>(raw);
    unsigned size = count * 2 + 2;
    unsigned short* buffer = static_cast<unsigned short*>(__builtin_vec_new(size));
    words[2] = NORRATH_ADDRESS_WORD(buffer);
    if (buffer == 0) return -1;
    str8to16__8VIStringPUsUiPCc(buffer, size, data);
    return 0;
}
}
