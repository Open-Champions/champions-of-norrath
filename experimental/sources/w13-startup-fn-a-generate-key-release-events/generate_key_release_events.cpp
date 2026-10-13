// Observed generate_key_release_events at 0x002e80d8, 280 bytes (a local routine of the keyboard reader),
// pinned SLUS_205.65. Called by process_kbd_data as fn(slot, record) after CheckforModifierKeyEvents.
// Fields read from the code (offsets in bytes):
//   slot +224  32-byte bitmap indexed by key code (byte key >> 3, bit key & 7)
//   slot +256  int, how many key codes were down at the previous report; slot +260 those key codes, one byte each
//   slot +38   unsigned halfword, how many events are queued; the queue entries start at slot +40, 16 bytes each,
//              ten of them at most
//   record +16 halfword, how many key codes the current report holds; record +20 those codes, one byte each
//   record +0 (word), +10 and +11 (bytes) are copied into each queued event
// For each previously-down key, in order, that is absent from the current report: its bitmap bit is cleared and,
// when fewer than ten events are queued, an entry is appended: word at +0 = record word 0, halfword at +4 = the key,
// halfword at +8 = record byte 10, halfword at +10 = record byte 11, halfword at +12 = 2; the count is then
// incremented. When the queue already holds ten, nothing is appended (the bit is still cleared).
// Returns 0x8000000d if the latest absent key found the queue full, 0 if the latest absent key was queued, and 0
// if no key was absent; the value of the latest absent key replaces any earlier one.
// Inferred: 2 marks a release (generate_key_press_events appends the same record with 1); 0x8000000d is
// a queue-full code.
// Unknown: the meaning of record word 0 and of record bytes 10 and 11 (possibly modifier state), and what the
// halfword at slot +36 or the last two bytes of each entry hold.
extern "C" {
typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;

int generate_key_release_events(unsigned char* slot, unsigned char* record) {
    const int current = *reinterpret_cast<AliasedShort*>(record + 16);
    int status = 0;
    for (int i = 0; i < *reinterpret_cast<AliasedInt*>(slot + 256); ++i) {
        const unsigned char key = slot[260 + i];
        bool present = false;
        for (int j = 0; j < current; ++j) {
            if (record[20 + j] == key) {
                present = true;
                break;
            }
        }
        if (present) continue;
        slot[224 + (key >> 3)] &= static_cast<unsigned char>(~(1 << (key & 7)));
        const unsigned count = *reinterpret_cast<AliasedUShort*>(slot + 38);
        if (count >= 10) {
            status = static_cast<int>(0x8000000du);
            continue;
        }
        unsigned char* const entry = slot + 40 + 16 * count;
        *reinterpret_cast<AliasedInt*>(entry) = *reinterpret_cast<AliasedInt*>(record);
        *reinterpret_cast<AliasedShort*>(entry + 4) = key;
        *reinterpret_cast<AliasedShort*>(entry + 8) = record[10];
        *reinterpret_cast<AliasedShort*>(entry + 10) = record[11];
        *reinterpret_cast<AliasedShort*>(entry + 12) = 2;
        *reinterpret_cast<AliasedUShort*>(slot + 38) = static_cast<unsigned short>(count + 1);
        status = 0;
    }
    return status;
}
}
