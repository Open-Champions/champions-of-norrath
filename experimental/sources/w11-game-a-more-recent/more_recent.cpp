// Observed moreRecent__Fii at 0x002562d8, 192 bytes. Compares two 64-byte records of the table
// MCA_DirContents (selected by the two arguments, a first, b second) by a date and time stored at
// record offsets 9..14: the year halfword at +14, then the bytes at +13, +12, then the 24-bit value
// made of +11 (high), +10 and +9 (low). The first field that differs decides: 0 when a is smaller
// (older) than b, 1 when a is larger. When all of them are equal the result is 1. So the result is
// 1 exactly when a is not older than b. Index arguments are not range checked.
// Inferred: the seven bytes are a calendar stamp laid out like the memory-card directory entry's
// time fields (seconds, minutes, hours, day, month, year), which fits comparing year first.
// Unknown: which stamp of the directory entry this is beyond that. Nothing here ran on the EE.
extern "C" unsigned char MCA_DirContents[];

extern "C" int moreRecent__Fii(int a, int b) {
    const unsigned char* first = MCA_DirContents + a * 64;
    const unsigned char* second = MCA_DirContents + b * 64;
    unsigned short ya = *reinterpret_cast<const unsigned short*>(first + 14);
    unsigned short yb = *reinterpret_cast<const unsigned short*>(second + 14);
    if (ya < yb) return 0;
    if (yb < ya) return 1;
    if (first[13] < second[13]) return 0;
    if (second[13] < first[13]) return 1;
    if (first[12] < second[12]) return 0;
    if (second[12] < first[12]) return 1;
    unsigned ta = (first[11] << 16) | (first[10] << 8) | first[9];
    unsigned tb = (second[11] << 16) | (second[10] << 8) | second[9];
    return ta >= tb;
}
