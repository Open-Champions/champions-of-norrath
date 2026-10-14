#include <cstring>
extern "C" int msg_save__4GoldPc(void* object, unsigned char* buffer);
int main() {
    unsigned char object[192];
    for (int i = 0; i < 192; ++i) object[i] = static_cast<unsigned char>(i);
    unsigned char out[12];
    std::memset(out, 0xee, sizeof out);
    if (msg_save__4GoldPc(object, out + 4) != 4) return 1;
    static const unsigned char want[4] = {128, 129, 130, 131};
    if (std::memcmp(out + 4, want, 4) != 0) return 2;
    for (int i = 0; i < 4; ++i) if (out[i] != 0xee || out[8 + i] != 0xee) return 3;
    for (int i = 0; i < 192; ++i) if (object[i] != static_cast<unsigned char>(i)) return 4;
    return 0;
}
