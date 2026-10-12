// Observed initLump__FPv at 0x00164338, 960 bytes (a local symbol). The
// argument is the address of a loaded lump: a 16-bit row count at byte 0 and,
// from byte 4, rows of three words {name, data, size} where name and data are
// offsets from the lump start. memCheckHeapReal__FPCci is called with
// "lump.cc" and 298 first and with 400 last. The function returns the word
// `shift`, described below.
// For each row (index i, count n re-read as rows are dropped):
//   * name and data are turned into addresses by adding the lump address.
//   * The row is processed only when `previous` (the last processed data
//     address, initially 0) is below this row's data address; other rows are
//     skipped after the conversion.
//   * A zero size prints the name through scePrintf with the "...is fucked"
//     message stored in the image and then stores zero through the zero size
//     (a store to address 0); that is reproduced with a volatile store.
//   * previous = data. When shift > 0 every later row whose offset plus the
//     lump address equals this row's data address has shift subtracted from
//     its offset, then size bytes at the old data address are copied down to
//     data - shift and data becomes data - shift.
//   * A leading "ntsc_" (5 chars) or "pal_" (4 chars) is stripped in place with
//     memmove of strlen-4 / strlen-3 bytes; "pal_" sets a flag.
//   * dupes = number of earlier rows with data equal to this row's data plus
//     later rows whose offset plus lump address equals it. If the row had
//     "pal_" and dupes is zero it is dropped: shift = (shift + size) & -128,
//     rows i+1.. move down one slot (count - i row copies, so the copy reads
//     one row past the old table end, as the image does), the stored count is
//     decremented and i is not advanced. Otherwise shift = (shift +
//     initFile__FPcPvi(name, data, size)) & -128.
// After the loop each remaining row whose name has an extension (text after the
// last '.') equal to "anm" gets animMapAnimationSounds__FP15AnimationHeader(data).
// Unknown: what initFile's result counts and why only pal_ rows are dropped.
extern "C" {
int scePrintf(const char*, ...);
void* memcpy(void*, const void*, __SIZE_TYPE__);
void* memmove(void*, const void*, __SIZE_TYPE__);
__SIZE_TYPE__ strlen(const char*);
int strncmp(const char*, const char*, __SIZE_TYPE__);
int strcmp(const char*, const char*);
char* strrchr(const char*, int);
void memCheckHeapReal__FPCci(const char*, int);
int initFile__FPcPvi(char*, void*, int);
void animMapAnimationSounds__FP15AnimationHeader(void*);
}
struct LumpRow { unsigned name, data, size; };
static_assert(sizeof(LumpRow) == 12, "row stride");

// Host contracts define NORRATH_HOST_CONTRACT: a host pointer does not fit in the 32-bit row words, so
// they supply the address <-> pointer mapping. On the target an address is the pointer itself.
#if defined(NORRATH_HOST_CONTRACT)
unsigned norrath_host_address(const void*);
void* norrath_host_pointer(unsigned);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
static inline unsigned address_of(const void* pointer) { return norrath_host_address(pointer); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(address)); }
static inline unsigned address_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
#endif

extern "C" int initLump__FPv(void* lump) {
    const unsigned base = address_of(lump);
    unsigned short* countPointer = static_cast<unsigned short*>(lump);
    LumpRow* rows = reinterpret_cast<LumpRow*>(static_cast<unsigned char*>(lump) + 4);
    memCheckHeapReal__FPCci("lump.cc", 298);
    int count = *countPointer;
    int shift = 0;
    unsigned previous = 0;
    int i = 0;
    if (count != 0) {
        do {
            LumpRow* row = rows + i;
            int next = i + 1;
            row->data = row->data + base;
            row->name = row->name + base;
            if (previous < row->data) {
                if (row->size == 0) {
                    scePrintf("!!!!!!!!!! %s is fucked\n", at(row->name));
                    *static_cast<volatile int*>(at(row->size)) = 0;
                }
                previous = row->data;
                if (shift > 0) {
                    for (int j = next; j < count; ++j) {
                        unsigned offset = rows[j].data;
                        if (offset + base == row->data) rows[j].data = offset - shift;
                    }
                    unsigned destination = row->data - shift;
                    row->data = destination;
                    memcpy(at(destination), at(previous), row->size);
                }
                char* name = static_cast<char*>(at(row->name));
                if (strncmp(name, "ntsc_", 5) == 0) memmove(name, name + 5, strlen(name) - 4);
                name = static_cast<char*>(at(row->name));
                int pal;
                if (strncmp(name, "pal_", 4) == 0) {
                    memmove(name, name + 4, strlen(name) - 3);
                    pal = 1;
                } else {
                    pal = 0;
                }
                int same = 0;
                for (int k = 0; k < i; ++k)
                    if (rows[k].data == row->data) ++same;
                for (int k = next; k < count; ++k)
                    if (rows[k].data + base == row->data) ++same;
                if (same > 0) pal = 0;
                if (pal != 0) {
                    shift = (shift + static_cast<int>(row->size)) & -128;
                    if (i < count) {
                        LumpRow* move = row;
                        for (int m = count - i; m != 0; --m, ++move) *move = move[1];
                    }
                    next = i;
                    --count;
                    *countPointer = static_cast<unsigned short>(*countPointer - 1);
                } else {
                    int used = initFile__FPcPvi(static_cast<char*>(at(row->name)), at(row->data), static_cast<int>(row->size));
                    shift = (shift + used) & -128;
                }
            }
            i = next;
        } while (i < count);
    }
    if (count > 0) {
        LumpRow* row = rows;
        for (int left = count; left != 0; --left, ++row) {
            char* dot = strrchr(static_cast<char*>(at(row->name)), '.');
            if (dot != 0 && strcmp(dot + 1, "anm") == 0) animMapAnimationSounds__FP15AnimationHeader(at(row->data));
        }
    }
    memCheckHeapReal__FPCci("lump.cc", 400);
    return shift;
}
