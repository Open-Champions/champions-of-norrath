// Observed setAnimEventFlags__8Creatureififififi at 0x1128a8, extent 88.
// Writes four (float, byte) pairs into one animation record of a creature. The
// creature's word at +36 is a table of 8-byte entries; entry `index` has a record
// pointer in its second word (+4). The record receives, in this order, with the
// table pointer re-read from the creature before every pair:
//   float at +20, byte at +24   (first float, first integer)
//   float at +28, byte at +32
//   float at +36, byte at +40
//   float at +44, byte at +48
// Arguments, from the mangled name: the table index, then four (float, integer)
// pairs, each float stored in the record with its integer's low eight bits as
// the byte beside it. The compiled argument registers match the original
// (integers in a2, a3, t0, t1; floats in f12 to f15).
// norrath_creature_set_anim_event_flags is the name the constructor sources import;
// it is defined as a second symbol for the same code.
// Unknown: what the record is (other than holding these fields) and what the
// byte/float pairs mean to the animation player.
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
struct EventCreature {
    unsigned char before_table[36];
    AnimEventEntry* table;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(EventCreature, table) == 36 && sizeof(AnimEventEntry) == 8 &&
                  __builtin_offsetof(AnimEventRecord, first_float) == 20 && __builtin_offsetof(AnimEventRecord, first_byte) == 24 &&
                  __builtin_offsetof(AnimEventRecord, second_float) == 28 && __builtin_offsetof(AnimEventRecord, second_byte) == 32 &&
                  __builtin_offsetof(AnimEventRecord, third_float) == 36 && __builtin_offsetof(AnimEventRecord, third_byte) == 40 &&
                  __builtin_offsetof(AnimEventRecord, fourth_float) == 44 && __builtin_offsetof(AnimEventRecord, fourth_byte) == 48,
              "observed offsets");
#endif

extern "C" void setAnimEventFlags__8Creatureififififi(EventCreature* creature, int index, float first_float, int first_byte,
                                                      float second_float, int second_byte, float third_float, int third_byte,
                                                      float fourth_float, int fourth_byte) {
    creature->table[index].record->first_byte = static_cast<signed char>(first_byte);
    creature->table[index].record->first_float = first_float;
    creature->table[index].record->second_byte = static_cast<signed char>(second_byte);
    creature->table[index].record->second_float = second_float;
    creature->table[index].record->third_byte = static_cast<signed char>(third_byte);
    creature->table[index].record->third_float = third_float;
    creature->table[index].record->fourth_byte = static_cast<signed char>(fourth_byte);
    creature->table[index].record->fourth_float = fourth_float;
}

#if defined(__mips__)
extern "C" void norrath_creature_set_anim_event_flags(void* object, int a, float b, int c, float d, int e, float f, int g, float h, int i)
    __attribute__((alias("setAnimEventFlags__8Creatureififififi")));
#endif
