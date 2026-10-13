// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_46dcf0.cpp"
int main() {
    static const char want[] = "%s_follow.anm";
    if (sizeof norrath_ctors_ant_lantern_view_46dcf0 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_46dcf0, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_46dcf0) != 13) return 3;
    return 0;
}
