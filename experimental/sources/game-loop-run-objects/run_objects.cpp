// Observed runObjectsTask at 0x00140070, 924 bytes.
// It always walks deleteList with ListSafeIterator and objectIgnoreFlags.
// A null object word at node+12 is skipped. Any other object is called
// through the 8-byte slot at vtable+8 with the sign-extended halfword
// adjustment and the integer 3. When (frameNm & 63) == 0, lastNmBlocksFree
// is replaced with blockGetNmFree and then read again. A signed value below
// 50 walks 16 ghostObjectGrid cells. The start cell comes from
// ((frameNm << 4) & 0x0fffffff): column (value / 120) % 120 and row
// (value % 120) + 1, wrapping at 120. Each later cell advances the row.
// A cell list uses the word at node+0 as the next node. An object whose
// flags at byte 12 include 0x1000 is called through vtable+8 with 3 when
// netNetworkMode is nonzero, or when the squared distance from object+16
// to worldViewCenter is greater than the 0x48610000 constant. The first
// such object ends that cell. gameIsPaused then runs. A nonzero result
// returns. Otherwise runList, mediumRunList[frameNm % 15], and
// slowRunList[frameNm % 60] are walked. An object whose flags include
// 0x8000 is skipped. The others are called through vtable+32 with only
// the adjusted object.
extern "C" void __16ListSafeIteratorP8ListHeadi(unsigned char* iterator, unsigned char* head, int mask);
extern "C" void next__16ListSafeIterator(unsigned char* iterator);
extern "C" void _$_16ListSafeIterator(unsigned char* iterator, int flags);
extern "C" int blockGetNmFree__Fv();
extern "C" int gameIsPaused__Fv();
extern "C" unsigned char deleteList[];
extern "C" unsigned char runList[];
extern "C" unsigned char mediumRunList[];
extern "C" unsigned char slowRunList[];
extern "C" unsigned char ghostObjectGrid[];
extern "C" unsigned char worldViewCenter[];
extern "C" int objectIgnoreFlags;
extern "C" int lastNmBlocksFree;
extern "C" int frameNm;
extern "C" signed char netNetworkMode;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_run_objects_base;
extern "C" void (*norrath_run_objects_with_arg[])(unsigned char*, int);
extern "C" void (*norrath_run_objects_only[])(unsigned char*);
#endif

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

unsigned load_u16(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8);
}

float load_f32(const unsigned char* address) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = load_u32(address);
    return value.as_float;
}

unsigned char* pointer_at(const unsigned char* address) {
    unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_run_objects_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

unsigned char* from_word(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_run_objects_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

void call_slot(unsigned char* object, unsigned slot, int argument, bool with_argument) {
    unsigned char* table = pointer_at(object + 72);
    int adjustment = static_cast<short>(load_u16(table + slot));
    unsigned code = load_u32(table + slot + 4);
    unsigned char* self = object + adjustment;
#if defined(NORRATH_HOST_CONTRACT)
    if (with_argument) norrath_run_objects_with_arg[code](self, argument);
    else norrath_run_objects_only[code](self);
#else
    if (with_argument) {
        auto function = reinterpret_cast<void (*)(unsigned char*, int)>(static_cast<__UINTPTR_TYPE__>(code));
        function(self, argument);
    } else {
        auto function = reinterpret_cast<void (*)(unsigned char*)>(static_cast<__UINTPTR_TYPE__>(code));
        function(self);
    }
#endif
}

void walk_delete(unsigned char* iterator) {
    __16ListSafeIteratorP8ListHeadi(iterator, deleteList, objectIgnoreFlags);
    unsigned char* node = pointer_at(iterator + 4);
    while (node !=
#if defined(NORRATH_HOST_CONTRACT)
           norrath_run_objects_base
#else
           nullptr
#endif
    ) {
        unsigned object_word = load_u32(node + 12);
        if (object_word != 0u) call_slot(from_word(object_word), 8u, 3, true);
        next__16ListSafeIterator(iterator);
        node = pointer_at(iterator + 4);
    }
    _$_16ListSafeIterator(iterator, 2);
}

bool farther_than_limit(unsigned char* object) {
    float dx = load_f32(object + 16) - load_f32(worldViewCenter);
    float dy = load_f32(object + 20) - load_f32(worldViewCenter + 4);
    float dz = load_f32(object + 24) - load_f32(worldViewCenter + 8);
    union {
        unsigned as_bits;
        float as_float;
    } limit;
    limit.as_bits = 0x48610000u;
    return limit.as_float < dx * dx + dy * dy + dz * dz;
}

void walk_grid_cell(unsigned cell) {
    while (cell != 0u) {
        unsigned char* node = from_word(cell);
        unsigned char* object = pointer_at(node + 12);
        unsigned flags = load_u32(object + 12);
        if ((flags & 0x1000u) == 0u || (netNetworkMode == 0 && !farther_than_limit(object))) {
            cell = load_u32(node);
            continue;
        }
        call_slot(object, 8u, 3, true);
        break;
    }
}

void walk_run_list(unsigned char* iterator, unsigned char* head) {
    __16ListSafeIteratorP8ListHeadi(iterator, head, objectIgnoreFlags);
    unsigned char* node = pointer_at(iterator + 4);
    while (node !=
#if defined(NORRATH_HOST_CONTRACT)
           norrath_run_objects_base
#else
           nullptr
#endif
    ) {
        unsigned char* object = pointer_at(node + 12);
        if ((load_u32(object + 12) & 0x8000u) == 0u) call_slot(object, 32u, 0, false);
        next__16ListSafeIterator(iterator);
        node = pointer_at(iterator + 4);
    }
    _$_16ListSafeIterator(iterator, 2);
}
}

extern "C" void runObjectsTask__Fv() {
    unsigned char iterator[32];
    walk_delete(iterator);
    if ((frameNm & 63) == 0) lastNmBlocksFree = blockGetNmFree__Fv();
    if (lastNmBlocksFree < 50) {
        unsigned mixed = (static_cast<unsigned>(frameNm) << 4) & 0x0fffffffu;
        int row = static_cast<int>(mixed % 120u) + 1;
        int column = static_cast<int>((mixed / 120u) % 120u);
        if (row >= 120) {
            column += 1;
            row = 0;
            if (column >= 120) column = 0;
        }
        for (int step = 0; step < 16; ++step) {
            unsigned cell = load_u32(ghostObjectGrid + static_cast<unsigned>(column * 120 + row) * 4u);
            if (cell != 0u) walk_grid_cell(cell);
            row += 1;
            if (row >= 120) {
                column += 1;
                row = 0;
                if (column >= 120) column = 0;
            }
        }
    }
    if (gameIsPaused__Fv() != 0) return;
    walk_run_list(iterator, runList);
    walk_run_list(iterator, mediumRunList + static_cast<unsigned>(frameNm % 15) * 4u);
    walk_run_list(iterator, slowRunList + static_cast<unsigned>(frameNm % 60) * 4u);
}
