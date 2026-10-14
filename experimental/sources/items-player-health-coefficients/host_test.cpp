#include <cstring>
extern "C" const unsigned char norrath_player_health_coefficients_view[];
int main() {
    static const unsigned char want[] = {0x17, 0xb7, 0xd1, 0x38, 0xcc, 0xcc, 0xcc, 0x3d, 0x99, 0x99, 0x99, 0x3f, 0x0a, 0xd7, 0x23, 0x3c};
    return std::memcmp(norrath_player_health_coefficients_view, want, sizeof want) == 0 ? 0 : 1;
}
