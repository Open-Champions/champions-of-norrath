#include <cstring>
extern "C" int msg_load__4GoldPc(void* object, const unsigned char* buffer);
int main() {
    unsigned char object[192];
    std::memset(object, 0x11, sizeof object);
    const unsigned char in[8] = {0x78, 0x56, 0x34, 0x82, 0x99, 0x99, 0x99, 0x99};
    if (msg_load__4GoldPc(object, in) != 4) return 1;
    static const unsigned char want[4] = {0x78, 0x56, 0x34, 0x82};
    if (std::memcmp(object + 128, want, 4) != 0) return 2;
    for (int i = 0; i < 192; ++i) if ((i < 128 || i >= 132) && object[i] != 0x11) return 3;
    return 0;
}
