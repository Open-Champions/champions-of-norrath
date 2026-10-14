// Observed updateTrails__Fv at 0x001840c0, 388 bytes. It walks the 64 records of
// trail (0x00780b00, 1040 bytes each) in order. Offsets inside a record:
//   +6 + 8 * n   halfword (signed) stamp of entry n, n below 128
//   +1024        halfword; zero marks the record unused
//   +1026        halfword age
//   +1028        byte, first live entry index ("start")
//   +1029        byte, entry index at which the scan stops ("end")
//   +1031        byte of flags: bit 0 skips the record, bit 4 stops the age from
//                advancing, bit 1 selects a step of 2 instead of 1
//   +1032        halfword (signed) lifetime
// For each used record without bit 0: unless bit 4 is set the age is incremented
// (16-bit wrap). If start equals end the +1024 halfword is cleared. Otherwise,
// with age and lifetime read as signed halfwords, entries are retired from the
// start while age minus the stamp at start is not below the lifetime: start
// becomes (start + step) & 0x7f and is stored to +1028 each time, and reaching
// end clears the +1024 halfword and stops the scan. The scan also stops, without
// clearing, at the first entry whose age difference is below the lifetime. The
// meaning of the fields beyond these offsets is not established here. Fresh
// compilation is not an original-byte match.
extern "C" {
extern unsigned char trail[];
}

namespace {
unsigned load_u16(const unsigned char* p) {
    return static_cast<unsigned>(p[0]) | (static_cast<unsigned>(p[1]) << 8);
}
int load_s16(const unsigned char* p) {
    const unsigned v = load_u16(p);
    return static_cast<int>(v ^ 0x8000u) - 0x8000;
}
void store_u16(unsigned char* p, unsigned v) {
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
}
}

extern "C" void updateTrails__Fv() {
    for (unsigned n = 0u; n < 64u; ++n) {
        unsigned char* record = trail + n * 1040u;
        if (load_s16(record + 1024) == 0) continue;
        const unsigned flags = record[1031];
        if ((flags & 1u) != 0u) continue;
        if ((flags & 0x10u) == 0u) store_u16(record + 1026, load_u16(record + 1026) + 1u);
        const unsigned end = record[1029];
        unsigned index = record[1028];
        if (index == end) {
            store_u16(record + 1024, 0u);
            continue;
        }
        const unsigned step = (flags & 2u) != 0u ? 2u : 1u;
        const int age = load_s16(record + 1026);
        const int lifetime = load_s16(record + 1032);
        bool cleared = false;
        for (;;) {
            const int stamp = load_s16(record + 6u + (index & 0xffu) * 8u);
            if (age - stamp < lifetime) break;
            index = (index + step) & 0x7fu;
            record[1028] = static_cast<unsigned char>(index);
            if (index == end) {
                store_u16(record + 1024, 0u);
                cleared = true;
                break;
            }
        }
        if (!cleared && (index & 0xffu) == end) store_u16(record + 1024, 0u);
    }
}
