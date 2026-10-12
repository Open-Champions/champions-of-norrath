#include "memcard_state.cpp"

#include <cstdio>

int main() {
    if (sizeof MCA_Result != 4 || sizeof cardType != 4 || sizeof askedForConfigLoad != 4 || sizeof browserSlot != 4) return 1;
    if (sizeof MCA_DirContents != 0x1000 || sizeof configFileName != 0x30) return 2;
    if (sizeof configLumpLoaded != 1 || sizeof configFormatted != 1) return 3;
    if (MCA_Result != 0 || cardType != 0 || askedForConfigLoad != 0 || configLumpLoaded != 0) return 4;
    if (browserSlot != 0 || configFormatted != 0) return 5;
    for (unsigned char b : MCA_DirContents) if (b != 0) return 6;
    for (char c : configFileName) if (c != 0) return 7;
    std::puts("memcard state passed");
    return 0;
}
