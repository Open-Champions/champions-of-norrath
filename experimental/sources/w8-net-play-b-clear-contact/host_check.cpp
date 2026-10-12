// Host check for netClearContact. Not an EE plan input.
#include "net_clear_contact.cpp"
#include <cstring>
extern "C" {
int nmPartners;
unsigned char partners[70528];
}
int main() {
    for (int count = -1; count <= 4; ++count) {
        std::memset(partners, 0xaa, sizeof partners);
        nmPartners = count;
        netClearContact__Fv();
        for (int i = 0; i < (int)sizeof partners; ++i) {
            int rec = i / 17632, off = i % 17632;
            bool cleared = rec < count && off >= 17616 && off < 17620;
            if (partners[i] != (cleared ? 0 : 0xaa)) return 1 + count;
        }
    }
    return 0;
}
