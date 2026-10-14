// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_4854d8.cpp"
int main() {
    static const char want[] = "metal";
    if (sizeof norrath_ctors_ant_lantern_view_4854d8 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_4854d8, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_4854d8) != 5) return 3;
    return 0;
}
