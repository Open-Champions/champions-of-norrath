extern "C" {
unsigned char netNetworkMode;
volatile int nmPartners;
unsigned char partners[0x12000];
int netGetLocalPlayerNm__Fi(int which);
}

int main() {
    netNetworkMode = 0;
    nmPartners = 4;
    partners[0x44d4u] = 0;
    if (netGetLocalPlayerNm__Fi(3) != 3) return 1;
    if (netGetLocalPlayerNm__Fi(-1) != -1) return 2;

    netNetworkMode = 1;
    nmPartners = 0;
    if (netGetLocalPlayerNm__Fi(0) != -1) return 3;
    nmPartners = -2;
    if (netGetLocalPlayerNm__Fi(0) != -1) return 4;

    nmPartners = 4;
    const unsigned stride = 17632u;
    partners[0x44d4u] = 0;
    partners[0x44d4u + stride] = 1;
    partners[0x44d4u + stride * 2u] = 0;
    partners[0x44d4u + stride * 3u] = 0x7f;
    if (netGetLocalPlayerNm__Fi(0) != 1) return 5;
    if (netGetLocalPlayerNm__Fi(1) != 3) return 6;
    if (netGetLocalPlayerNm__Fi(2) != -1) return 7;

    partners[0x44d4u] = 1;
    partners[0x44d4u + stride] = 1;
    if (netGetLocalPlayerNm__Fi(0) != 0) return 8;
    if (netGetLocalPlayerNm__Fi(1) != 1) return 9;
    return 0;
}
