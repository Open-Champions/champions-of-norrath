#include "net_state.cpp"
#include <cstring>
static bool zero(const unsigned char* p, unsigned n) {
    for (unsigned i = 0; i < n; ++i) if (p[i]) return false;
    return true;
}
int main() {
    if (sizeof netNmPartners != 1 || netNmPartners != 1) return 1;
    if (sizeof nmPartners != 4 || nmPartners != 0) return 2;
    if (sizeof broadcastConnection != 4 || broadcastConnection != -1) return 3;
    unsigned char word[4];
    std::memcpy(word, &broadcastConnection, 4);
    if (word[0] != 0xff || word[1] != 0xff || word[2] != 0xff || word[3] != 0xff) return 4;
    if (sizeof commHandle != 4 || commHandle != 0) return 5;
    if (sizeof commStatusHandle != 4 || commStatusHandle != 0) return 6;
    if (sizeof netWaitMsgHandler != 4 || netWaitMsgHandler != 0) return 7;
    if (sizeof partners != 4u * 17632u) return 8;
    if (!zero(partners, sizeof partners)) return 9;
    if (sizeof g_net_buf != 2048u || !zero(g_net_buf, sizeof g_net_buf)) return 10;
    // The local-player walk reads the flag byte at partners + 0x44d4 of each 17632-byte record.
    if (0x44d4u >= 17632u) return 11;
    return 0;
}
