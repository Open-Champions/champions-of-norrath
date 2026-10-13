#include "fs_version.cpp"

unsigned char _fsversion[4];
const unsigned char __ps2_klibinfo__[16] = {'P', 's', 'I', 'I', 'l', 'i', 'b', 'k', 'e', 'r', 'n', 'l', '2', '8', '0', '0'};
extern "C" int memcmp(const void* left, const void* right, unsigned count) {
    const unsigned char* a = static_cast<const unsigned char*>(left);
    const unsigned char* b = static_cast<const unsigned char*>(right);
    for (unsigned i = 0; i < count; ++i)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

static void set_version(const char* text) {
    for (int i = 0; i < 4; ++i) _fsversion[i] = static_cast<unsigned char>(text[i]);
}

int main() {
    if (memcmp(_fswildcard, "....", 5) != 0) return 1;
    // The library's own version matches.
    set_version("2800");
    if (_fs_version() != 0) return 2;
    // The wildcard matches.
    set_version("....");
    if (_fs_version() != 0) return 3;
    // The library string and the wildcard differ, so any other version is a mismatch.
    set_version("2500");
    if (_fs_version() != 1) return 4;
    set_version("\0\0\0");
    if (_fs_version() != 1) return 5;
    // A different wildcard that equals the library string makes every version pass.
    static const unsigned char same[5] = {'2', '8', '0', '0', 0};
    _fswildcard = same;
    if (_fs_version() != 0) return 6;
    return 0;
}
