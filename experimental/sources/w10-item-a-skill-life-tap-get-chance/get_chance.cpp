// Observed getChance__12SkillLifeTapi at 0x002be340, 108 bytes (this, int rank),
// returning a float. `this` holds a table pointer at +96. The table is read as
// 8-byte entries {signed halfword adjustment, unused halfword, function
// address}; this is how the compiler reaches virtual calls here.
//   1. entry at table+120: called with (this + adjustment, rank, 1);
//   2. entry at table+88: called with (this + adjustment, 0, result of step 1).
// The result of step 2 is converted to float and scaled by 0.01f (the single-
// precision word 0x3c23d70a at gp-19020), then returned.
// Unknown: what the two called entries compute, which class slots they are, and
// what the integer result of step 2 means before scaling.
extern "C" float getChance__12SkillLifeTapi(void* self, int rank) {
    struct Entry {
        short adjust;
        short unused;
        int (*function)(void*, int, int);
    };
    char* object = static_cast<char*>(self);
    const char* table = *reinterpret_cast<char**>(object + 96);
    const Entry& first = *reinterpret_cast<const Entry*>(table + 120);
    const int a = first.function(object + first.adjust, rank, 1);
    const Entry& second = *reinterpret_cast<const Entry*>(*reinterpret_cast<char**>(object + 96) + 88);
    const int b = second.function(object + second.adjust, 0, a);
    return static_cast<float>(b) * 0.01f;
}
