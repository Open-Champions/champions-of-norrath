#include <cstring>
extern "C" const unsigned char norrath_player_ability_coefficients_view[];
int main() {
    static const unsigned char want[] = {0x0a, 0xd7, 0x23, 0x3c, 0x00, 0x40, 0x9c, 0x45, 0x00, 0x3c, 0x1c, 0x46};
    return std::memcmp(norrath_player_ability_coefficients_view, want, sizeof want) == 0 ? 0 : 1;
}
