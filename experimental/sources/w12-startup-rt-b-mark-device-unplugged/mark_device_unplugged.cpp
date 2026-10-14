// Observed mark_device_unplugged at 0x002e7f58, 380 bytes (a file-local routine in the original),
// called by the keyboard/mouse reader thread with the halfword at +4 of a record of kind 0.
// Searches two tables in GLibData for the first record whose signed halfword at +12 equals the device
// argument (sign-extended compare, so -1 matches an already freed slot):
//   table one: count in the word at GLibData + 64, records of 272 bytes starting at GLibData + 128;
//   table two: count in the word at GLibData + 68, records of 104 bytes starting at GLibData + 1216.
// Table one is searched completely before table two. At the first match, if the word at record + 36 is
// already 6 nothing happens. Otherwise, holding the semaphore whose id is the word at GLibData + 44
// (WaitSema, then SignalSema as the last action), it stores 0x80000006 at record + 24, -1 in the
// halfword at record + 12, 6 at record + 36, and sets the flag word at GLibData + 100 (table one) or
// GLibData + 104 (table two) to 1. No match in either table: nothing at all.
// Inferred only: the tables are the keyboard and mouse device lists (the reader thread initialises
// the +12 halfwords of both to -1 the same way) and the value 6 is an "unplugged" state. The meaning
// of the +24 word and the flag words is not established. Nothing was executed on the EE.
extern "C" {
typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;

extern unsigned char GLibData[] __attribute__((aligned(4)));
int WaitSema(int semaphore);
int SignalSema(int semaphore);

static inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
static inline short& half(unsigned char* base, int offset) { return *reinterpret_cast<AliasedShort*>(base + offset); }

void mark_device_unplugged(int device) {
    unsigned char* const g = GLibData;
    for (int i = 0; i < word(g, 64); ++i) {
        unsigned char* const record = g + 128 + 272 * i;
        if (half(record, 12) != device) continue;
        if (word(record, 36) == 6) return;
        WaitSema(word(g, 44));
        word(record, 24) = static_cast<int>(0x80000006u);
        half(record, 12) = -1;
        word(record, 36) = 6;
        word(g, 100) = 1;
        SignalSema(word(g, 44));
        return;
    }
    for (int i = 0; i < word(g, 68); ++i) {
        unsigned char* const record = g + 1216 + 104 * i;
        if (half(record, 12) != device) continue;
        if (word(record, 36) == 6) return;
        WaitSema(word(g, 44));
        word(record, 24) = static_cast<int>(0x80000006u);
        half(record, 12) = -1;
        word(record, 36) = 6;
        word(g, 104) = 1;
        SignalSema(word(g, 44));
        return;
    }
}
}
