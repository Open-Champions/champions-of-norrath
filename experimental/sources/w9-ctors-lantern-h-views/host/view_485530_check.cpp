// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_485530.cpp"
int main() {
    static const char want[] = "Crumble";
    if (sizeof norrath_ctors_ant_lantern_view_485530 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_485530, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_485530) != 7) return 3;
    return 0;
}
