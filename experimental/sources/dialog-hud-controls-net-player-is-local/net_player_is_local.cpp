// Observed netPlayerIsLocal__Fi at 0x001353a8, 44 bytes.
// The compare against nmPartners is signed. An index that is not strictly
// below that count returns 1 and does not read a partner record.
// Otherwise the low 32 bits of index * 17632 select a partner, and the
// return value is the sign-extended byte at partners + 0x44d4 of that
// record. partners and nmPartners are not defined here.
extern "C" int nmPartners;
extern "C" unsigned char partners[];

extern "C" int netPlayerIsLocal__Fi(int index) {
    if (!(index < nmPartners)) return 1;
    const unsigned slot = static_cast<unsigned>(index) * 17632u;
    return static_cast<signed char>(partners[slot + 0x44d4u]);
}
