// Observed: ProcessRawFile__13VIStringTableRCt7VIArray1Zc at 0x0101b920 is 600 bytes. Arguments:
// the table object (a0) and a reference to a byte array (a1) whose word at +0 is the length and
// word at +4 the data pointer. The object's word at +8 receives a freshly allocated 16-bit text
// buffer. Three file-local items are used: a 3-byte header (EF BB BF), a 2-byte header (FF FE) and
// a 16-bit item that is set to 0xFFFE on the first call (guarded by a one-word flag) and compared
// as its two bytes (FE FF) in memory order.
// Branches, in order: (1) length >= 3 and data begins EF BB BF: n = UTF8_strlen(data + 3,
// length - 3); allocate n * 2 + 2 bytes with __builtin_vec_new; a null result is returned as -1;
// UTF8_convertToUTF16(data + 3, buffer, n); the function returns 0 when that call returns n and
// 1 otherwise. (2) length >= 2 and data begins FF FE: units = length / 2 - 1; allocate units * 2
// + 2 bytes (null: return -1); memcpy(buffer, data + 2, units * 2); write a zero unit after them;
// return 0. (3) length >= 2 and data begins FE FF: same sizing; copy the units with their two
// bytes swapped (written only when units > 0), then a zero unit; return 0. (4) otherwise: allocate
// length * 2 + 2 bytes (null: return -1) and call str8to16(buffer, length * 2 + 2, data); return
// 0. The branch-1 failure code 1 is an observed value; the allocation failure code is -1.
// Inferred: loads a text blob that may carry a UTF-8 or UTF-16 byte-order mark into the table's
// 16-bit text buffer; the byte-swapped branch handles big-endian UTF-16.
// Unknown: the buffer is not zero-terminated explicitly in branches (1) and (4), and whether the
// callees terminate it is outside this unit.
extern "C" {
int memcmp(const void* a, const void* b, unsigned int n);
void* memcpy(void* destination, const void* source, unsigned int n);
void* __builtin_vec_new(unsigned int size);
int UTF8_strlen__FPCci(const char* text, int length);
int UTF8_convertToUTF16__FPCcPUsi(const char* text, unsigned short* destination, int count);
unsigned short* str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned int limit, const char* source);
}

static const unsigned char utf8header[3] = {0xef, 0xbb, 0xbf};
static const unsigned char ucs2header[2] = {0xff, 0xfe};
static unsigned short ucs2header_reverse_endian;
static int ucs2header_reverse_endian_ready;

extern "C" {
int ProcessRawFile__13VIStringTableRCt7VIArray1Zc(unsigned char* self, const unsigned char* raw) {
    if (ucs2header_reverse_endian_ready == 0) {
        ucs2header_reverse_endian = 0xfffe;
        ucs2header_reverse_endian_ready = 1;
    }
    int length = *reinterpret_cast<const int*>(raw);
    const unsigned char* data = *reinterpret_cast<const unsigned char* const*>(raw + 4);
    unsigned short** text = reinterpret_cast<unsigned short**>(self + 8);

    if (length >= 3 && memcmp(utf8header, data, 3) == 0) {
        const char* body = reinterpret_cast<const char*>(data + 3);
        int count = UTF8_strlen__FPCci(body, length - 3);
        *text = static_cast<unsigned short*>(__builtin_vec_new(static_cast<unsigned int>(count) * 2 + 2));
        if (*text == 0) return -1;
        return count != UTF8_convertToUTF16__FPCcPUsi(body, *text, count);
    }
    if (length >= 2 && memcmp(ucs2header, data, 2) == 0) {
        const unsigned char* body = data + 2;
        int bytes = (length / 2 - 1) * 2;
        *text = static_cast<unsigned short*>(__builtin_vec_new(static_cast<unsigned int>(bytes) + 2));
        if (*text == 0) return -1;
        memcpy(*text, body, static_cast<unsigned int>(bytes));
        *reinterpret_cast<unsigned short*>(reinterpret_cast<unsigned char*>(*text) + bytes) = 0;
        return 0;
    }
    if (length >= 2 && memcmp(&ucs2header_reverse_endian, data, 2) == 0) {
        const unsigned short* body = reinterpret_cast<const unsigned short*>(data + 2);
        int units = length / 2 - 1;
        int bytes = units * 2;
        *text = static_cast<unsigned short*>(__builtin_vec_new(static_cast<unsigned int>(bytes) + 2));
        if (*text == 0) return -1;
        for (int i = 0; i < units; ++i) {
            unsigned short v = body[i];
            (*text)[i] = static_cast<unsigned short>((v >> 8) | (v << 8));
        }
        *reinterpret_cast<unsigned short*>(reinterpret_cast<unsigned char*>(*text) + bytes) = 0;
        return 0;
    }
    unsigned int size = static_cast<unsigned int>(length) * 2 + 2;
    *text = static_cast<unsigned short*>(__builtin_vec_new(size));
    if (*text == 0) return -1;
    str8to16__8VIStringPUsUiPCc(*text, size, reinterpret_cast<const char*>(data));
    return 0;
}
}
