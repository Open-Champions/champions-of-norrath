// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_485548.cpp"
int main() {
    static const char want[] = "laundry";
    if (sizeof norrath_ctors_ant_lantern_view_485548 != sizeof want) return 1;
    if (std::memcmp(norrath_ctors_ant_lantern_view_485548, want, sizeof want) != 0) return 2;
    if (std::strlen(norrath_ctors_ant_lantern_view_485548) != 7) return 3;
    return 0;
}
