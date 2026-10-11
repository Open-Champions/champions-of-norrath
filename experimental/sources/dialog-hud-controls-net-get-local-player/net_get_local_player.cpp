// Observed netGetLocalPlayerNm__Fi at 0x00135438, 108 bytes.
// The word at nmPartners is loaded in the delay slot of the mode test, so
// the load happens when netNetworkMode is zero as well. A zero mode returns
// the argument. A positive mode walks partner records of 17632 bytes. Each
// nonzero byte at partners + 0x44d4 consumes one requested local player.
// The walk index of the match is the result. A nonpositive count, or no
// remaining match, returns -1. The three symbols read here are not defined
// in this file.
extern "C" unsigned char netNetworkMode;
extern "C" volatile int nmPartners;
extern "C" unsigned char partners[];

extern "C" int netGetLocalPlayerNm__Fi(int which) {
    const int count = nmPartners;
    if (netNetworkMode == 0) return which;
    if (count <= 0) return -1;
    unsigned char* cursor = partners + 0x44d4u;
    int index = 0;
    while (index < count) {
        const unsigned char flag = *cursor;
        cursor += 17632u;
        if (flag != 0) {
            which = static_cast<int>(static_cast<unsigned>(which) - 1u);
            if (which == -1) return index;
        }
        index = static_cast<int>(static_cast<unsigned>(index) + 1u);
    }
    return -1;
}
