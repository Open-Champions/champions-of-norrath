// Host contract for the wLumpHeader storage unit. Not an EE plan input.
#include <cstdio>
#include "w_lump_header_data.cpp"
int main() {
    if (sizeof(wLumpHeader) != 4 || wLumpHeader != 0) { std::fprintf(stderr, "fail: one zero word\n"); return 1; }
    std::printf("wLumpHeader storage check passed\n");
    return 0;
}
