#include "pic_state.cpp"

#include <cstdio>

int main() {
    if (sizeof(picsToLoad) != 1 || sizeof(picLoadFinished) != 1 || sizeof(picInProgress) != 4) return 1;
    if (picsToLoad != 0 || picLoadFinished != 0 || picInProgress != -100000) return 2;
    if (static_cast<unsigned>(picInProgress) != 0xfffe7960u) return 3;
    if (sizeof(picBuffer) != 327680 || sizeof(picList) != 12288 || sizeof(picLoadBuffer) != 32768) return 4;
    for (unsigned char byte : picBuffer) if (byte != 0) return 5;
    for (unsigned char byte : picList) if (byte != 0) return 6;
    for (unsigned char byte : picLoadBuffer) if (byte != 0) return 7;
    std::puts("item pic storage contract passed");
    return 0;
}
