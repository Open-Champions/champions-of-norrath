// Host check for the message table extent. Not an EE plan input.
#include <cstdio>
#include "in_msg_data.cpp"
int main() {
    if (sizeof inMsg != 1008 || 1008 != 8 * 126) { std::fprintf(stderr, "fail: extent\n"); return 1; }
    for (unsigned char byte : inMsg) if (byte) { std::fprintf(stderr, "fail: not zero\n"); return 2; }
    std::printf("message table storage check passed\n");
    return 0;
}
