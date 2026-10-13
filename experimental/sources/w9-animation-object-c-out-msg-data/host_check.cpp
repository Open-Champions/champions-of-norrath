// Host contract for the outMsg storage unit. Not an EE plan input.
#include <cstdio>
#include "out_msg_data.cpp"
int main() {
    if (sizeof(outMsg) != 128) { std::fprintf(stderr, "fail: extent\n"); return 1; }
    for (unsigned char b : outMsg) if (b) { std::fprintf(stderr, "fail: not zero\n"); return 1; }
    std::printf("outMsg storage check passed\n");
    return 0;
}
