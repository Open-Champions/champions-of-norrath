// Observed setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi
// at 0x112900, extent 60.
// The same field writes as setAnimEventFlags, but the table of 8-byte entries is
// the second argument (a resource map) instead of the creature's word at +36; the
// creature itself is not read. Entry `index` has a record pointer in its second
// word (+4). The record receives four (float, byte) pairs at +20/+24, +28/+32,
// +36/+40 and +44/+48. The record pointer is re-read from the entry before every
// pair. Arguments from the mangled name: creature, map, index, then four
// (integer, float) pairs in the order integer, float; compiled registers match
// the original (integers a3, t0, t1, t2; floats f12 to f15).
// norrath_creature_set_anim_event_flags_res_map is the name the constructor sources
// import; it is defined as a second symbol for the same code.
// Unknown: what the map is beyond a table of such entries, and the field meanings.
struct AnimEventRecord {
    unsigned char before_first[20];
    float first_float;
    signed char first_byte;
    unsigned char pad25[3];
    float second_float;
    signed char second_byte;
    unsigned char pad33[3];
    float third_float;
    signed char third_byte;
    unsigned char pad41[3];
    float fourth_float;
    signed char fourth_byte;
};
struct AnimEventEntry {
    unsigned word0;
    AnimEventRecord* record;
};
#if defined(__mips__)
static_assert(sizeof(AnimEventEntry) == 8 && __builtin_offsetof(AnimEventRecord, first_byte) == 24 &&
                  __builtin_offsetof(AnimEventRecord, second_byte) == 32 && __builtin_offsetof(AnimEventRecord, third_byte) == 40 &&
                  __builtin_offsetof(AnimEventRecord, fourth_byte) == 48 && __builtin_offsetof(AnimEventRecord, fourth_float) == 44,
              "observed offsets");
#endif

extern "C" void setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(
    void* creature, AnimEventEntry* map, int index, int first_byte, float first_float, int second_byte, float second_float,
    int third_byte, float third_float, int fourth_byte, float fourth_float) {
    (void)creature;
    map[index].record->first_byte = static_cast<signed char>(first_byte);
    map[index].record->first_float = first_float;
    map[index].record->second_byte = static_cast<signed char>(second_byte);
    map[index].record->second_float = second_float;
    map[index].record->third_byte = static_cast<signed char>(third_byte);
    map[index].record->third_float = third_float;
    map[index].record->fourth_byte = static_cast<signed char>(fourth_byte);
    map[index].record->fourth_float = fourth_float;
}

#if defined(__mips__)
// The importer declares the five integers first; the register assignment is the same.
#pragma GCC diagnostic ignored "-Wattribute-alias"
extern "C" void norrath_creature_set_anim_event_flags_res_map(void* object, void* map, int a, int b, int c, int d, int e, float f12,
                                                              float f13, float f14, float f15)
    __attribute__((alias("setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi")));
#endif
