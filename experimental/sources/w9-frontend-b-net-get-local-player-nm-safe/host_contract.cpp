#include "net_get_local_player_nm_safe.cpp"

#include <cstdio>

int nmPartners;
signed char partners[17632 * 5 + 17620 + 1];

int main() {
    nmPartners = 0;
    if (netGetLocalPlayerNmSafe__Fi(0) != -1) return 1;
    nmPartners = 5;
    if (netGetLocalPlayerNmSafe__Fi(0) != -1) return 2;
    partners[17620 + 17632 * 1] = 1;
    partners[17620 + 17632 * 3] = -1;
    partners[17620 + 17632 * 4] = 7;
    if (netGetLocalPlayerNmSafe__Fi(0) != 1) return 3;
    if (netGetLocalPlayerNmSafe__Fi(1) != 3) return 4;
    if (netGetLocalPlayerNmSafe__Fi(2) != 4) return 5;
    if (netGetLocalPlayerNmSafe__Fi(3) != -1) return 6;
    nmPartners = 4;  // record 4 is out of range
    if (netGetLocalPlayerNmSafe__Fi(2) != -1) return 7;
    if (netGetLocalPlayerNmSafe__Fi(-1) != -1) return 8;
    std::puts("net get local player nm safe passed");
    return 0;
}
