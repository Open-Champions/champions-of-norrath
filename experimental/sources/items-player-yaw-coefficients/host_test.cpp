#include <cstring>
extern "C" {
extern const unsigned char norrath_player_view_47fed4[];
extern const unsigned char norrath_player_view_47fed8[];
extern const unsigned char norrath_player_view_47fedc[];
extern const unsigned char norrath_player_view_47fee0[];
}
int main() {
    static const unsigned char norrath_player_view_47fed4_want[] = {0xda, 0x0f, 0x49, 0x38};
    if (std::memcmp(norrath_player_view_47fed4, norrath_player_view_47fed4_want, 4) != 0) return 1;
    static const unsigned char norrath_player_view_47fed8_want[] = {0x57, 0xaa, 0x2a, 0x3e};
    if (std::memcmp(norrath_player_view_47fed8, norrath_player_view_47fed8_want, 4) != 0) return 1;
    static const unsigned char norrath_player_view_47fedc_want[] = {0x24, 0x6b, 0x08, 0x3c};
    if (std::memcmp(norrath_player_view_47fedc, norrath_player_view_47fedc_want, 4) != 0) return 1;
    static const unsigned char norrath_player_view_47fee0_want[] = {0x17, 0x80, 0x45, 0x39};
    if (std::memcmp(norrath_player_view_47fee0, norrath_player_view_47fee0_want, 4) != 0) return 1;
    return 0;
}
