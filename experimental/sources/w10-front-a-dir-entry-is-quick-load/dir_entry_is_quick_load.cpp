// Observed: dirEntryIsQuickLoad__FP14sceMcTblGetDir at 0x0025bd48 is 168 bytes. With e the entry
// and s the object quickLoadStamp (0x007f7fc0, of which 0x007f7fe0 is read as a string), it
// returns 1 when all of these hold and 0 at the first that fails, in this order:
//  1. strcmp(e + 32, s + 32) is zero;
//  2. the 16-bit value formed by bytes 1 and 2 of e equals that of s;
//  3. byte 3 of e equals byte 3 of s;
//  4. the halfword at e + 4 equals the halfword at s + 4;
//  5. the halfword at e + 6 equals the halfword at s + 6.
// Inferred: from the libmc directory-entry layout (create time in bytes 0 to 7 as spare, second,
// minute, hour, day, month, year; name at byte 32), the test is that the entry's name is the
// stamp's name and its creation time to the second equals the stamp's. A quick-load save is
// recognised by that name and time; the stamp's layout is taken to match the entry's.
// Unknown: where and when quickLoadStamp is filled, and its size.

extern "C" {
extern unsigned char quickLoadStamp[];
int strcmp(const char* left, const char* right);
}

extern "C" int dirEntryIsQuickLoad__FP14sceMcTblGetDir(void* entry) {
    const unsigned char* e = static_cast<const unsigned char*>(entry);
    const unsigned char* s = quickLoadStamp;
    if (strcmp(reinterpret_cast<const char*>(e + 32), reinterpret_cast<const char*>(s + 32)) != 0) return 0;
    if (static_cast<unsigned short>(e[1] | (e[2] << 8)) != static_cast<unsigned short>(s[1] | (s[2] << 8))) return 0;
    if (e[3] != s[3]) return 0;
    if (e[4] != s[4] || e[5] != s[5]) return 0;
    if (e[6] != s[6] || e[7] != s[7]) return 0;
    return 1;
}
