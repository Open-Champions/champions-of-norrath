// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_486058.cpp"
int main() {
    static const char want[] = "%s.vif";
    if (sizeof norrath_ctors_ant_lantern_view_486058 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_486058, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_486058) != 6) return 3;
    return 0;
}
