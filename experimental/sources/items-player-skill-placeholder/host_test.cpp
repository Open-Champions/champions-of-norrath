#include <cstring>
extern "C" const unsigned char norrath_player_skill_placeholder_view[];
int main() {
    static const unsigned char want[] = {0x6e, 0x65, 0x77, 0x73, 0x74, 0x79, 0x6c, 0x65, 0x00};
    return std::memcmp(norrath_player_skill_placeholder_view, want, sizeof want) == 0 ? 0 : 1;
}
