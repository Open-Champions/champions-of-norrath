#include "lf_version.cpp"

#include <cstdio>

extern "C" unsigned char _lfversion[4] = {};

static int same_bytes(const void* left, const void* right, unsigned count) {
    auto* a = static_cast<const unsigned char*>(left);
    auto* b = static_cast<const unsigned char*>(right);
    for (unsigned index = 0; index < count; ++index)
        if (a[index] != b[index]) return 1;
    return 0;
}

extern "C" int memcmp(const void* left, const void* right, unsigned count) {
    return same_bytes(left, right, count);
}

int main() {
    if (sizeof __ps2_klibinfo__ != 16) return 1;
    if (same_bytes(__ps2_klibinfo__ + 12, "2800", 4) != 0) return 2;
    if (same_bytes(_lfwildcard, "....", 4) != 0) return 3;
    _lfversion[0] = '2';
    _lfversion[1] = '8';
    _lfversion[2] = '0';
    _lfversion[3] = '0';
    if (_lf_version() != 0) return 4;
    _lfversion[0] = '.';
    _lfversion[1] = '.';
    _lfversion[2] = '.';
    _lfversion[3] = '.';
    if (_lf_version() != 0) return 5;
    _lfversion[0] = 0;
    if (_lf_version() != 1) return 6;
    static const unsigned char same[4] = {'2', '8', '0', '0'};
    _lfwildcard = same;
    _lfversion[0] = 1;
    if (_lf_version() != 0) return 7;
    std::puts("_lf_version passed");
    return 0;
}
