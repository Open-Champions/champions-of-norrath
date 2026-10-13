// Observed generate_key_press_events at 0x002e81f0, 272 bytes (a local routine of the keyboard reader),
// pinned SLUS_205.65. The twin of generate_key_release_events, called by process_kbd_data as fn(slot, record).
// Fields are the same as in the release routine: the current report's key codes at record +20 (count: halfword
// at record +16), the previous report's key codes at slot +260 (count: int at slot +256), the held-key bitmap at
// slot +224, the event count (unsigned halfword) at slot +38 and the ten 16-byte queue entries from slot +40.
// For each key of the current report, in order, that is absent from the previous report: its bitmap bit is set
// and, when fewer than ten events are queued, an entry is appended exactly as the release routine does, with
// halfword +12 = 1 instead of 2. With a full queue the bit is still set and nothing is appended.
// Returns 0x8000000d if the latest new key found the queue full, 0 if the latest new key was queued, and 0 if
// no key was new.
// Inferred: 1 marks a press; 0x8000000d is a queue-full code.
// Unknown: as in the release routine, the contents carried in record word 0 and bytes 10 and 11.
extern "C" {
typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;

int generate_key_press_events(unsigned char* slot, unsigned char* record) {
    const int current = *reinterpret_cast<AliasedShort*>(record + 16);
    int status = 0;
    for (int i = 0; i < current; ++i) {
        const unsigned char key = record[20 + i];
        bool present = false;
        for (int j = 0; j < *reinterpret_cast<AliasedInt*>(slot + 256); ++j) {
            if (slot[260 + j] == key) {
                present = true;
                break;
            }
        }
        if (present) continue;
        slot[224 + (key >> 3)] |= static_cast<unsigned char>(1 << (key & 7));
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
        *reinterpret_cast<AliasedShort*>(entry + 12) = 1;
        *reinterpret_cast<AliasedUShort*>(slot + 38) = static_cast<unsigned short>(count + 1);
        status = 0;
    }
    return status;
}
}
