// Host check for netPlayerIsConnected. Not an EE plan input.
#include "net_player_is_connected.cpp"
#include <cstring>
extern "C" {
signed char netNetworkMode;
int nmPartners;
unsigned char partners[70528];
}
int main() {
    std::memset(partners, 0, sizeof partners);
    nmPartners = 3;
    netNetworkMode = 0;
    for (int p = -2; p < 6; ++p) if (netPlayerIsConnected__Fi(p) != 1) return 1;  // mode 0 ignores the argument
    netNetworkMode = 1;
    for (int p = 0; p < 3; ++p) if (netPlayerIsConnected__Fi(p) != 1) return 2;
    for (int p = 3; p < 6; ++p) if (netPlayerIsConnected__Fi(p) != 0) return 3;
    for (int mark = 0; mark < 3; ++mark) {
        partners[mark * 17632 + 17621] = 0x80;  // a negative signed byte is nonzero
        for (int p = 0; p < 3; ++p)
            if (netPlayerIsConnected__Fi(p) != (p == mark ? 0 : 1)) return 4;
        partners[mark * 17632 + 17621] = 0;
    }
    // neighbouring bytes do not matter
    partners[17620] = 1; partners[17622] = 1;
    if (netPlayerIsConnected__Fi(0) != 1) return 5;
    return 0;
}
