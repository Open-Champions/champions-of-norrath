// Observed _fs_version at 0x002d9230, 140 bytes, local binding, the file-system
// RPC counterpart of _lf_version (same instruction sequence with different
// operands). sceOpen calls it after making sure the library is initialised and
// treats a nonzero result as a version mismatch.
//
// It compares four bytes at a time with memcmp:
//   equal _fsversion and the last four bytes of __ps2_klibinfo__ (offset 12) -> 0
//   equal _fsversion and the four bytes at _fswildcard                       -> 0
//   otherwise 1 when those two reference words differ, 0 when they match.
// The last case does not involve _fsversion at all. _fswildcard is a data word at
// 0x00444a08 (observed) holding the address 0x0047b8d8, where the image has four
// dots and a NUL; that string is not authored separately here, so a four-dot
// array local to this unit stands in for it. _fsversion is defined by the
// sceFsReset unit and __ps2_klibinfo__ by the _lf_version unit.
extern "C" unsigned char _fsversion[4];
extern "C" const unsigned char __ps2_klibinfo__[16];
extern "C" int memcmp(const void* left, const void* right, unsigned count);

static const unsigned char wildcard_bytes[5] = {0x2e, 0x2e, 0x2e, 0x2e, 0};
extern "C" {
const unsigned char* _fswildcard = wildcard_bytes;
}

extern "C" int _fs_version() {
    if (memcmp(_fsversion, __ps2_klibinfo__ + 12, 4) == 0)
        return 0;
    if (memcmp(_fsversion, _fswildcard, 4) == 0)
        return 0;
    return memcmp(__ps2_klibinfo__ + 12, _fswildcard, 4) != 0 ? 1 : 0;
}
