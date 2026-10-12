// Host check for netDisconnectPlayer. Not an EE plan input.
#include "net_disconnect_player.cpp"
#include <cstring>
extern "C" {
int nmPartners;
unsigned char partners[70528];
}
int main() {
    std::memset(partners, 0, sizeof partners);
    nmPartners = 2;
    netDisconnectPlayer__Fi(2);
    netDisconnectPlayer__Fi(3);
    for (unsigned char b : partners) if (b) return 1;  // out of range writes nothing
    netDisconnectPlayer__Fi(1);
    for (int i = 0; i < (int)sizeof partners; ++i)
        if (partners[i] != (i == 17632 + 17621 ? 1 : 0)) return 2;
    netDisconnectPlayer__Fi(0);
    if (partners[17621] != 1) return 3;
    nmPartners = 4;
    netDisconnectPlayer__Fi(3);
    return partners[3 * 17632 + 17621] == 1 ? 0 : 4;
}
