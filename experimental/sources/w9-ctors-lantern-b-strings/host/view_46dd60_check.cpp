// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_46dd60.cpp"
int main() {
    static const char want[] = "turnSpeed";
    if (sizeof norrath_ctors_ant_lantern_view_46dd60 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_46dd60, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_46dd60) != 9) return 3;
    return 0;
}
