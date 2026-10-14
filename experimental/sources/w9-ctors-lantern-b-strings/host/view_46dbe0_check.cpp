// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_46dbe0.cpp"
int main() {
    static const char want[] = "faydark1";
    if (sizeof norrath_ctors_ant_lantern_view_46dbe0 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_46dbe0, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_46dbe0) != 8) return 3;
    return 0;
}
