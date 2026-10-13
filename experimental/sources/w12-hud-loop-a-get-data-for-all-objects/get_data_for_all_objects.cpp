// Observed getDataForAllObjects__FPciPiiT2T2 at 0x00142880 (a local function of the object save code), 1124
// bytes, 208-byte frame. Arguments: data, size, count (int*), pass, eligible (int*), omitted (int*).
// The caller objectGetSaveData tries passes 0, 1, 2, ... and a result of -1 means "did not fit".
// Setup: *count = 0; the running eligible total, the omitted total and the output cursor (data) start at
// zero, zero and data. Three groups of lists are walked with a list safe iterator built with mask 0:
// the 120 by 120 objectGrid (rows of 480 bytes, one 4-byte head per column, rows in order, columns in
// order), the same shape at ghostObjectGrid, then the single head offTheGrid. For each node the object is the
// word at node + 12 and flags is the word at object + 12:
//  - flags & 0x4000 skips the object.
//  - When userFilterFunction (a word holding a function address) is nonzero it is called with the word at
//    object + 52, a nonzero result skips the object, and flags is read again.
//  - eligible total increases by one when flags & 0x3000 is nonzero.
//  - The object is taken when pass is zero, or flags & 0x3000 is zero, or (total modulo (pass + 1)) is
//    zero, or (flags & 0x2000 is nonzero and pass is 1). The total is compared after the increase.
//  - A taken object increases *count, then objGet(object, cursor) returns a byte count added to the
//    cursor. When cursor - data reaches size - 512 or more the iterator is destroyed and -1 is returned
//    at once (the outputs *eligible and *omitted are not written in that case).
//  - A rejected object increases the omitted total.
// Each iterator is destroyed with delete flag 2 after its cell. At the end *eligible = eligible total,
// *omitted = omitted total and the result is cursor - data.
// Inferred: this packs the objects of the world for saving; "eligible" objects are thinned out by later
// passes. Unknown: the meaning of the flag bits and of the filter argument.
extern "C" {
extern unsigned char objectGrid[];
extern unsigned char ghostObjectGrid[];
extern unsigned char offTheGrid[];
extern unsigned userFilterFunction;
void __16ListSafeIteratorP8ListHeadi(unsigned* iterator, unsigned char* head, int mask);
void next__16ListSafeIterator(unsigned* iterator);
void _$_16ListSafeIterator(unsigned* iterator, int flags);
int objGet__FP10GameObjectPc(unsigned char* object, char* destination);
#if defined(NORRATH_HOST_CONTRACT)
unsigned char* norrath_gdao_pointer(unsigned word);
int norrath_gdao_call_filter(unsigned function, unsigned argument);
#endif
}

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

unsigned char* pointer_at(const unsigned char* address) {
    const unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    return word == 0 ? nullptr : norrath_gdao_pointer(word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

int call_filter(unsigned function, unsigned argument) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_gdao_call_filter(function, argument);
#else
    return reinterpret_cast<int (*)(unsigned)>(static_cast<__UINTPTR_TYPE__>(function))(argument);
#endif
}

struct Walk {
    char* data;
    char* cursor;
    int size;
    int* count;
    int pass;
    int eligible;
    int omitted;
};

// Returns true when the output is full and the whole routine must give up.
bool walk_list(Walk& state, unsigned char* head) {
    unsigned iterator[8];
    __16ListSafeIteratorP8ListHeadi(iterator, head, 0);
    while (unsigned char* node = pointer_at(reinterpret_cast<unsigned char*>(iterator) + 4)) {
        unsigned char* object = pointer_at(node + 12);
        unsigned flags = load_u32(object + 12);
        bool skip = (flags & 0x4000u) != 0;
        if (!skip && userFilterFunction != 0) {
            if (call_filter(userFilterFunction, load_u32(object + 52)) != 0) skip = true;
            else flags = load_u32(object + 12);
        }
        if (!skip) {
            const unsigned masked = flags & 0x3000u;
            if (masked != 0) ++state.eligible;
            bool take;
            if (state.pass == 0) take = true;
            else if (masked == 0) take = true;
            else if (state.eligible % (state.pass + 1) == 0) take = true;
            else take = (flags & 0x2000u) != 0 && state.pass == 1;
            if (take) {
                ++*state.count;
                state.cursor += objGet__FP10GameObjectPc(object, state.cursor);
                if (!(state.cursor - state.data < state.size - 512)) {
                    _$_16ListSafeIterator(iterator, 2);
                    return true;
                }
            } else {
                ++state.omitted;
            }
        }
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);
    return false;
}
}

extern "C" int getDataForAllObjects__FPciPiiT2T2(char* data, int size, int* count, int pass, int* eligible, int* omitted) {
    Walk state = {data, data, size, count, pass, 0, 0};
    *count = 0;
    for (int row = 0; row < 120; ++row)
        for (int column = 0; column < 120; ++column)
            if (walk_list(state, objectGrid + row * 480 + column * 4)) return -1;
    for (int row = 0; row < 120; ++row)
        for (int column = 0; column < 120; ++column)
            if (walk_list(state, ghostObjectGrid + row * 480 + column * 4)) return -1;
    if (walk_list(state, offTheGrid)) return -1;
    *eligible = state.eligible;
    *omitted = state.omitted;
    return static_cast<int>(state.cursor - data);
}
