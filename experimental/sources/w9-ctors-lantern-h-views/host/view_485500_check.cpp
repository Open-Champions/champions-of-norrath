// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_485500.cpp"
int main() {
    static const char want[] = ".anm";
    if (sizeof norrath_ctors_ant_lantern_view_485500 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_485500, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_485500) != 4) return 3;
    return 0;
}
