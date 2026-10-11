#include <cstring>
extern "C" unsigned char norrath_player_view_483f3e[];
int main() {
    static const unsigned char want[] = {0x00};
    return std::memcmp(norrath_player_view_483f3e, want, sizeof want) == 0 ? 0 : 1;
}
