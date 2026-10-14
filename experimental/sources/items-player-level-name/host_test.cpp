#include <cstring>
extern "C" unsigned char levelName[322];
extern "C" unsigned char norrath_player_view_34b752[];
int main() {
    unsigned char zeros[322] = {};
    if (std::memcmp(levelName, zeros, 322) != 0) return 1;
    if (+levelName == +norrath_player_view_34b752) return 2;
    levelName[0] = 1;
    levelName[80] = 2;
    levelName[242] = 3;
    levelName[321] = 4;
    if (norrath_player_view_34b752[0] != 0) return 3;
    levelName[0] = 0;
    levelName[80] = 0;
    levelName[242] = 0;
    levelName[321] = 0;
    return 0;
}
