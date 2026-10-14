// Observed _lf_version at 0x002dc700, 140 bytes, local binding. It compares
// four bytes. Equal _lfversion and the four bytes at offset 12 of
// __ps2_klibinfo__ returns 0. Otherwise equal _lfversion and the four bytes
// at _lfwildcard returns 0. Otherwise it returns 1 when those two four-byte
// references differ, and 0 when they match. __ps2_klibinfo__ at 0x00444940
// has st_size 0. ttyinit is 16 bytes later, and those 16 bytes are
// PsIIlibkernl2800 with no extra NUL. _lfwildcard at 0x00444a14 is the
// address 0x0047b8e0, which holds four dots and a NUL.
extern "C" unsigned char _lfversion[4];
extern "C" int memcmp(const void* left, const void* right, unsigned count);

extern "C" {
const unsigned char __ps2_klibinfo__[16] = {
    'P', 's', 'I', 'I', 'l', 'i', 'b', 'k', 'e', 'r', 'n', 'l', '2', '8', '0', '0'
};
}
static const unsigned char wildcard_bytes[5] = {0x2e, 0x2e, 0x2e, 0x2e, 0};
extern "C" {
const unsigned char* _lfwildcard = wildcard_bytes;
}

extern "C" int _lf_version() {
    if (memcmp(_lfversion, __ps2_klibinfo__ + 12, 4) == 0)
        return 0;
    if (memcmp(_lfversion, _lfwildcard, 4) == 0)
        return 0;
    return memcmp(__ps2_klibinfo__ + 12, _lfwildcard, 4) != 0 ? 1 : 0;
}
