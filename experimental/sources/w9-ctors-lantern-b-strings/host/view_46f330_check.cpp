// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_46f330.cpp"
int main() {
    static const char want[] = "gnomepeasant";
    if (sizeof norrath_ctors_ant_lantern_view_46f330 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_46f330, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_46f330) != 12) return 3;
    return 0;
}
