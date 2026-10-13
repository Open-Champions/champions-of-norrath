#include "3map_icon_list_count.cpp"
#include <cstring>
int main() {
    if (sizeof(_3map$iconListCount) != 4) return 1;
    if (_3map$iconListCount != 0) return 2;
    return 0;
}
