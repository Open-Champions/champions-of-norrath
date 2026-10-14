#include <cstring>
extern "C" const unsigned char norrath_player_feat_end_name_view[];
int main() {
    static const unsigned char want[] = {0x45, 0x4e, 0x44, 0x00};
    return std::memcmp(norrath_player_feat_end_name_view, want, sizeof want) == 0 ? 0 : 1;
}
