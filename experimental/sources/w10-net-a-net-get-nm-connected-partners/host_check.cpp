// Host check for netGetNmConnectedPartners. Not an EE plan input.
#include "net_get_nm_connected_partners.cpp"
#include <cstring>
int nmPartners;
unsigned char partners[5 * 17632];
int main() {
    std::memset(partners, 0, sizeof partners);
    nmPartners = 0;
    if (netGetNmConnectedPartners__Fv() != 0) return 1;
    nmPartners = -3;
    if (netGetNmConnectedPartners__Fv() != 0) return 2;
    for (int n = 1; n <= 4; ++n) {
        nmPartners = n;
        if (netGetNmConnectedPartners__Fv() != n) return 3;
    }
    nmPartners = 4;
    for (int mark = 0; mark < 4; ++mark) {
        partners[mark * 17632 + 17621] = 0x80;
        if (netGetNmConnectedPartners__Fv() != 3) return 4;
        partners[mark * 17632 + 17621] = 0;
        partners[mark * 17632 + 17622] = 1;
        if (netGetNmConnectedPartners__Fv() != 3) return 5;
        partners[mark * 17632 + 17621] = 1;
        if (netGetNmConnectedPartners__Fv() != 3) return 6;
        partners[mark * 17632 + 17621] = 0;
        partners[mark * 17632 + 17622] = 0;
    }
    partners[4 * 17632 + 17621] = 1;  // beyond nmPartners: not counted
    if (netGetNmConnectedPartners__Fv() != 4) return 7;
    return 0;
}
