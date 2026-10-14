#include "card_count.cpp"

int main() {
    if (sizeof norrath_world_c_view_4861e8 != 4) return 1;
    if (norrath_world_c_view_4861e8 != 0) return 2;
    norrath_world_c_view_4861e8 = 9;
    return norrath_world_c_view_4861e8 == 9 ? 0 : 3;
}
