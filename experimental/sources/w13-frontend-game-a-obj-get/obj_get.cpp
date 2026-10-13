// Observed objGet__FP10GameObjectPc at 0x00142498, 996 bytes, 592-byte frame (a file-local function
// in the image). Serialises one game object into the record at the second argument `out` and
// returns nothing. Strings are interned in a byte pool between poolStart and poolEnd (the globals at
// 0x0048689c and 0x004868a0): an entry is a length byte, the characters and a terminating zero; an
// interned string is identified by its offset from poolStart, the offset of its first character.
// Intern(s): with poolStart zero the result is zero and nothing is added. Otherwise entries are
// walked from poolStart to poolEnd (next entry = this one + length byte + 1); the first entry whose
// first character matches (as signed bytes) and that is strcmp-equal to s gives its offset. When
// none matches, the entry is appended at poolEnd (length = strlen(s)), poolEnd advances by
// strlen + 2, and the offset of the new string is the result.
//  1. The object's name text is the word at object + 52 when that is nonzero: the characters up to
//     the first ';', ',' or zero byte are copied to a local buffer (no bound is applied), and the
//     buffer is interned. When it is zero, the object's dynamic type is read by calling the
//     function word at (word at object + 72) + 4 with the object (a null object calls
//     __throw_bad_typeid), the first word of the returned type description is a name text,
//     leading decimal digits are skipped and the rest is interned. The offset becomes the int at out + 0.
//  2. out + 6 gets the halfword at object + 56; out + 8, +12 and +16 get the three words at object
//     + 16, +20 and +24 (copied as words).
//  3. The word at object + 44 points at a zero-terminated array of text words. Each text is interned
//     (a null text and an empty pool both give offset zero) and its offset is stored from out + 20 on,
//     one int each; a zero int ends the list. When that word is zero or the first text is null
//     nothing is listed, and the end marker sits at out + 20.
//  4. B = (out + 4 * count + 31) rounded down to a multiple of 8. The function word at
//     ((word at object + 72) + 72) + 4 is called with (object + the halfword at (vtable + 72), B)
//     and returns a byte count; the end of the record is (B + count + 3) rounded down to a multiple
//     of 4, and the halfword at out + 4 is that end minus out.
// Inferred: this builds one entry of a saved object table (name offset, size, flags halfword,
// position, property name offsets, then the object's own save data written by its virtual function).
// Unknown: the meaning of the halfword at +56, the words at +44 and +52, and the field names.
extern "C" {
extern unsigned char* poolStart;
extern unsigned char* poolEnd;
void __throw_bad_typeid();
int strcmp(const char* left, const char* right);
unsigned long strlen(const char* text);
char* strcpy(char* to, const char* from);
}

#ifdef W13_FRONT_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
template <class T>
inline void store(unsigned char* base, int offset, T value) {
    __builtin_memcpy(base + offset, &value, sizeof value);
}
// 32-bit pointer words of the image become native pointers through this on the host check.
inline __UINTPTR_TYPE__ widen(unsigned word) {
#ifdef W13_FRONT_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
typedef void* (*TypeFunction)(void* object);
typedef int (*SaveFunction)(void* object, void* buffer);

__attribute__((always_inline)) inline int intern(const char* text) {
    unsigned char* start = poolStart;
    if (start == nullptr) return 0;
    unsigned char* at = start;
    if (at != poolEnd) {
        do {
            const int length = static_cast<signed char>(*at);
            ++at;
            if (static_cast<signed char>(*at) == static_cast<signed char>(*text) &&
                strcmp(reinterpret_cast<char*>(at), text) == 0) {
                return static_cast<int>(at - poolStart);
            }
            at += length + 1;
        } while (at != poolEnd);
    }
    const int length = static_cast<int>(strlen(text));
    *at = static_cast<unsigned char>(length);
    ++at;
    strcpy(reinterpret_cast<char*>(at), text);
    poolEnd = poolEnd + length + 2;
    return static_cast<int>(at - poolStart);
}
}

extern "C" void objGet__FP10GameObjectPc(void* self, char* out) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* record = reinterpret_cast<unsigned char*>(out);
    const unsigned nameWord = load<unsigned>(object, 52);
    if (nameWord == 0) {
        const char* text = nullptr;
        if (object == nullptr) {
            __throw_bad_typeid();
        } else {
            const unsigned char* table = reinterpret_cast<const unsigned char*>(widen(load<unsigned>(object, 72)));
            const unsigned char* type =
                static_cast<const unsigned char*>(reinterpret_cast<TypeFunction>(widen(load<unsigned>(table, 4)))(object));
            text = reinterpret_cast<const char*>(widen(load<unsigned>(type, 0)));
        }
        while (static_cast<unsigned char>(*text - '0') < 10) ++text;
        store<int>(record, 0, intern(text));
    } else {
        const char* source = reinterpret_cast<const char*>(widen(nameWord));
        char name[512];
        int length = 0;
        while (source[length] != 0 && source[length] != ';' && source[length] != ',') {
            name[length] = source[length];
            ++length;
        }
        name[length] = 0;
        store<int>(record, 0, intern(name));
    }
    store<unsigned>(record, 8, load<unsigned>(object, 16));
    store<unsigned short>(record, 6, load<unsigned short>(object, 56));
    store<unsigned>(record, 12, load<unsigned>(object, 20));
    store<unsigned>(record, 16, load<unsigned>(object, 24));
    int count = 0;
    const unsigned char* list = reinterpret_cast<const unsigned char*>(widen(load<unsigned>(object, 44)));
    if (list != nullptr && load<unsigned>(list, 0) != 0) {
        for (;;) {
            const unsigned textWord = load<unsigned>(list, 4 * count);
            int offset = 0;
            if (poolStart != nullptr && textWord != 0) offset = intern(reinterpret_cast<const char*>(widen(textWord)));
            store<int>(record, 20 + 4 * count, offset);
            ++count;
            if (load<unsigned>(list, 4 * count) == 0) break;
        }
    }
    store<int>(record, 20 + 4 * count, 0);
    unsigned char* buffer = reinterpret_cast<unsigned char*>((reinterpret_cast<__UINTPTR_TYPE__>(record) + 4 * count + 31) & ~static_cast<__UINTPTR_TYPE__>(7));
    const unsigned char* entry = reinterpret_cast<const unsigned char*>(widen(load<unsigned>(object, 72))) + 72;
    const int written = reinterpret_cast<SaveFunction>(widen(load<unsigned>(entry, 4)))(object + load<short>(entry, 0), buffer);
    unsigned char* end = reinterpret_cast<unsigned char*>((reinterpret_cast<__UINTPTR_TYPE__>(buffer + written) + 3) & ~static_cast<__UINTPTR_TYPE__>(3));
    store<unsigned short>(record, 4, static_cast<unsigned short>(end - record));
}
