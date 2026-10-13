// Host check for getDamageMultiplier__4itemPQ24item11description. Not an EE plan input.
#include "get_damage_multiplier.cpp"
#include <cstring>
int main() {
    unsigned char desc[96];
    std::memset(desc, 0xff, sizeof desc);
    float value = getDamageMultiplier__4itemPQ24item11description(desc);
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    if (bits != 0x3f800000u) return 1;
    value = getDamageMultiplier__4itemPQ24item11description(nullptr);
    std::memcpy(&bits, &value, 4);
    if (bits != 0x3f800000u) return 2;
    for (unsigned char b : desc) if (b != 0xff) return 3;
    return 0;
}
