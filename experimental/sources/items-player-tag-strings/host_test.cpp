#include <cstring>
extern "C" {
extern const char norrath_player_view_462350[];
extern const char norrath_player_view_462360[];
extern const char norrath_player_view_4623a0[];
extern const char norrath_player_view_4623b0[];
extern const char norrath_player_view_483ba0[];
extern const char norrath_player_view_483ba8[];
extern const char norrath_player_view_483bb0[];
extern const char norrath_player_view_483bb8[];
extern const char norrath_player_view_483bc0[];
extern const char norrath_player_view_483bc8[];
extern const char norrath_player_view_483bd0[];
extern const char norrath_player_view_483bd8[];
extern const char norrath_player_view_483be0[];
extern const char norrath_player_view_483be8[];
extern const char norrath_player_view_483bf0[];
extern const char norrath_player_view_483c08[];
extern const char norrath_player_view_483c10[];
extern const char norrath_player_view_483c18[];
extern const char norrath_player_view_483c20[];
extern const char norrath_player_view_483c28[];
}
int main() {
    if (std::strcmp(norrath_player_view_462350, "noCollide") != 0) return 1;
    if (std::strcmp(norrath_player_view_462360, "animMask") != 0) return 1;
    if (std::strcmp(norrath_player_view_4623a0, "equipmnt.lmp") != 0) return 1;
    if (std::strcmp(norrath_player_view_4623b0, "healingpotion") != 0) return 1;
    if (std::strcmp(norrath_player_view_483ba0, "action") != 0) return 1;
    if (std::strcmp(norrath_player_view_483ba8, "name") != 0) return 1;
    if (std::strcmp(norrath_player_view_483bb0, "rname") != 0) return 1;
    if (std::strcmp(norrath_player_view_483bb8, "%s.lmp") != 0) return 1;
    if (std::strcmp(norrath_player_view_483bc0, "%s.vif") != 0) return 1;
    if (std::strcmp(norrath_player_view_483bc8, "%s.tex") != 0) return 1;
    if (std::strcmp(norrath_player_view_483bd0, "lump") != 0) return 1;
    if (std::strcmp(norrath_player_view_483bd8, "model") != 0) return 1;
    if (std::strcmp(norrath_player_view_483be0, "texture") != 0) return 1;
    if (std::strcmp(norrath_player_view_483be8, "anim") != 0) return 1;
    if (std::strcmp(norrath_player_view_483bf0, "%d") != 0) return 1;
    if (std::strcmp(norrath_player_view_483c08, "gold") != 0) return 1;
    if (std::strcmp(norrath_player_view_483c10, "from") != 0) return 1;
    if (std::strcmp(norrath_player_view_483c18, "default") != 0) return 1;
    if (std::strcmp(norrath_player_view_483c20, "gate") != 0) return 1;
    if (std::strcmp(norrath_player_view_483c28, "item") != 0) return 1;
    if (std::strcmp(norrath_player_view_4623a0, "equipmnt.lmp") != 0) return 2;
    if (norrath_player_view_4623a0[12] != 0) return 3;
    return 0;
}
