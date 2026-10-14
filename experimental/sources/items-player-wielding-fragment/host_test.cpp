#include <cstring>
extern "C" const unsigned char norrath_player_wielding_name_fragment_view[];
int main() {
    static const unsigned char want[] = {0x63, 0x69, 0x72, 0x63, 0x6c, 0x65, 0x74, 0x00};
    return std::memcmp(norrath_player_wielding_name_fragment_view, want, sizeof want) == 0 ? 0 : 1;
}
