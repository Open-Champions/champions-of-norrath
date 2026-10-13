// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_485488.cpp"
int main() {
    static const char want[] = ".tex";
    if (sizeof norrath_ctors_ant_lantern_view_485488 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_485488, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_485488) != 4) return 3;
    return 0;
}
