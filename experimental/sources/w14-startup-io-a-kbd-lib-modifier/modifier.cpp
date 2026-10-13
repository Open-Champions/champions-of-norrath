// Observed: CheckforModifierKeyEvents at 0x002e8300 is 400 bytes (a local symbol in the
// image). Arguments are the keyboard-library state block and a raw report (layouts as
// in generate_key_release_events). It compares state byte 33 with report byte 11 and,
// for each of the 8 bits whose value differs, takes a key code from
// ModifierCodes[bit], sets (bit now 1, direction 1) or clears (direction 2) that
// code's bit in the 32-byte map at 224, and, if fewer than 10 events are queued
// (halfword at 38), appends a 16-byte event at 40 + 16 * queued: word 0 = report
// word 0, halfword 4 = code, halfword 8 = report byte 10, halfword 10 = report
// byte 11, halfword 12 = direction. A full queue silently drops the event; the map
// update still happens. State byte 33 is not written here. No result is used.
// The original applies the map update twice (before and after queueing); the second
// application is identical, so it is written once here.
// Inferred: report byte 11 is a modifier bit set, ModifierCodes lists the key code
// for each bit; the layout matches the other generators.
// Unknown: the contents of ModifierCodes (eight bytes of local data at 0x00447f60).
extern "C" {
extern unsigned char ModifierCodes[];

void CheckforModifierKeyEvents(unsigned char* state, unsigned char* report) {
    for (int bit = 0; bit < 8; ++bit) {
        unsigned mask = (1u << bit) & 0xff;
        if (((state[33] ^ report[11]) & mask) == 0) continue;
        unsigned char code = ModifierCodes[bit];
        unsigned direction = (report[11] & mask) != 0 ? 1 : 2;
        unsigned char* map = state + 224 + (code >> 3);
        unsigned code_mask = (1u << (code & 7)) & 0xff;
        unsigned short* queued = reinterpret_cast<unsigned short*>(state + 38);
        if (direction == 2) *map = static_cast<unsigned char>(*map & ~code_mask);
        else *map = static_cast<unsigned char>(*map | code_mask);
        if (*queued < 10) {
            unsigned char* event = state + 40 + 16 * *queued;
            *reinterpret_cast<unsigned*>(event) = *reinterpret_cast<unsigned*>(report);
            *reinterpret_cast<unsigned short*>(event + 4) = code;
            *reinterpret_cast<unsigned short*>(event + 8) = report[10];
            *reinterpret_cast<unsigned short*>(event + 10) = report[11];
            *reinterpret_cast<unsigned short*>(event + 12) = static_cast<unsigned short>(direction);
            *queued = static_cast<unsigned short>(*queued + 1);
        }
    }
}
}
