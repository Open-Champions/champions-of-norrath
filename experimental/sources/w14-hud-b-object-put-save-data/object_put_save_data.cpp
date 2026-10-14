// Observed objectPutSaveData__FPci at 0x00142ee8, 632 bytes, 144-byte frame. Arguments: a buffer in
// the layout objectGetSaveData__FPci writes, and an int "filter" flag. Returns the number of bytes
// from the buffer start to the end of the last record read (an int).
//  1. Stores the defaults 32, 32, 32, 68, 68, 68, 0, -1.0f, 1.0f into the nine words of lightStart.
//  2. Header: short count at +0, unsigned short flags at +2, int pool offset at +4 (from the buffer
//     start). The flags are stored back with bit 2 set; the original bits 1 (create) and 2
//     (already relocated) are kept. poolOffset is set to the buffer plus the pool offset.
//  3. For each of count records, starting at buffer + 8 (the record size, a short at +4, moves to
//     the next): the word at +0 is a name offset, negative meaning none; the name is poolOffset plus
//     the offset or a null pointer. Record layout: +6 short type, +8/+12/+16 floats (a position),
//     from +20 a list of option words ended by a zero word, then the object data at the next
//     8-aligned address after the list's terminator (list start + 4 * (n + 1) + 7, rounded down).
//     When the filter flag is set the record is skipped unless userFilterFunction is nonzero and
//     returns nonzero for the name. Otherwise: unless flag bit 2 was already set, each option word
//     is replaced by 0 (negative) or poolOffset plus the word; then, when userCreateByNameFunction
//     is nonzero, it is called with the name, a copy of the position, the type and the option array
//     address, and a nonzero object gets the name pointer stored at object + 52. When flag bit 1
//     was set and an object was made, vtable slot 8 (the 8-byte entry at *(object + 72) + 64: a
//     short this-adjustment, then the function) is called with the adjusted object and the data
//     address.
//  4. For each object on deleteList, walked with a list safe iterator masked by objectIgnoreFlags:
//     vtable slot 1 (the entry at *(object + 72) + 8) is called with the adjusted object and 3.
//     The node's object word is at node + 12; a null object is skipped.
// Inferred: this loads the objects saved by objectGetSaveData and then tells every object on
// deleteList to run its message 3. Which messages slot 8 and slot 1 are is not established here.
// Unknown: what the lightStart defaults are for, the record's total size rules, and whether the
// type short and the option array are what the constructors take.
extern "C" {
struct Point3 { float x, y, z; };
typedef void* (*CreateByNameFunction)(char* name, Point3 position, short type, unsigned* options);
typedef int (*FilterFunction)(char* name);

extern int lightStart[];
extern char* poolOffset;
extern int objectIgnoreFlags;
extern unsigned char deleteList[];
extern unsigned norrath_create_by_name_function_word;
extern unsigned userFilterFunction;

void __16ListSafeIteratorP8ListHeadi(unsigned* iterator, unsigned char* head, int mask);
void next__16ListSafeIterator(unsigned* iterator);
void _$_16ListSafeIterator(unsigned* iterator, int flags);
}

namespace {
// Every pointer stored in the data is a 32-bit word.
template <typename T>
inline T fromWord(unsigned word) {
    return reinterpret_cast<T>(static_cast<__UINTPTR_TYPE__>(word));
}

// A vtable entry: a short this-adjustment at +0 and the function address word at +4.
inline void callEntry(unsigned char* object, int offset, void* argument) {
    unsigned char* entry = fromWord<unsigned char*>(*reinterpret_cast<unsigned*>(object + 72)) + offset;
    fromWord<void (*)(void*, void*)>(*reinterpret_cast<unsigned*>(entry + 4))(object + *reinterpret_cast<short*>(entry),
                                                                              argument);
}
}  // namespace

extern "C" int objectPutSaveData__FPci(char* buffer, int filter) {
    lightStart[2] = 32;
    lightStart[5] = 68;
    lightStart[0] = 32;
    lightStart[1] = 32;
    lightStart[3] = 68;
    lightStart[4] = 68;
    lightStart[6] = 0;
    *reinterpret_cast<float*>(lightStart + 7) = -1.0f;
    *reinterpret_cast<float*>(lightStart + 8) = 1.0f;

    unsigned short flags = *reinterpret_cast<unsigned short*>(buffer + 2);
    poolOffset = buffer + *reinterpret_cast<int*>(buffer + 4);
    const __UINTPTR_TYPE__ pool = reinterpret_cast<__UINTPTR_TYPE__>(poolOffset);
    const int count = *reinterpret_cast<short*>(buffer);
    const bool create = (flags & 1) != 0;
    const bool relocated = (flags & 2) != 0;
    *reinterpret_cast<unsigned short*>(buffer + 2) = static_cast<unsigned short>(flags | 2);

    char* next = buffer + 8;
    if (count > 0) {
        int remaining = count;
        char* record = next;
        do {
            const int nameOffset = *reinterpret_cast<int*>(record);
            const int size = *reinterpret_cast<short*>(next + 4);
            char* name = nullptr;
            next += size;
            if (nameOffset >= 0) name = poolOffset + nameOffset;
            bool skip = false;
            if (filter != 0) {
                if (userFilterFunction == 0 || fromWord<FilterFunction>(userFilterFunction)(name) == 0) skip = true;
            }
            if (!skip) {
                Point3 position;
                position.x = *reinterpret_cast<float*>(record + 8);
                position.y = *reinterpret_cast<float*>(record + 12);
                position.z = *reinterpret_cast<float*>(record + 16);
                unsigned* options = reinterpret_cast<unsigned*>(record + 20);
                int optionCount = 0;
                if (options[0] != 0) {
                    unsigned* read = options;
                    unsigned* write = options;
                    do {
                        if (!relocated) {
                            int word = static_cast<int>(*write);
                            *write = word < 0 ? 0u : static_cast<unsigned>(pool + word);
                        }
                        ++read;
                        ++write;
                        ++optionCount;
                    } while (*read != 0);
                }
                unsigned char* object = nullptr;
                if (norrath_create_by_name_function_word != 0) {
                    Point3 copy = position;
                    object = static_cast<unsigned char*>(fromWord<CreateByNameFunction>(norrath_create_by_name_function_word)(
                        name, copy, *reinterpret_cast<short*>(record + 6), options));
                    if (object != nullptr) *reinterpret_cast<unsigned*>(object + 52) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(name));
                }
                if (create && object != nullptr)
                    callEntry(object, 64,
                              reinterpret_cast<void*>((reinterpret_cast<__UINTPTR_TYPE__>(record + optionCount * 4) + 31) & ~7u));
            }
            --remaining;
            record = next;
        } while (remaining != 0);
    }

    unsigned iterator[8];
    __16ListSafeIteratorP8ListHeadi(iterator, deleteList, objectIgnoreFlags);
    while (iterator[1] != 0) {
        unsigned char* object = fromWord<unsigned char*>(*reinterpret_cast<unsigned*>(fromWord<unsigned char*>(iterator[1]) + 12));
        if (object != nullptr) callEntry(object, 8, reinterpret_cast<void*>(3));
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);
    return static_cast<int>(next - buffer);
}
