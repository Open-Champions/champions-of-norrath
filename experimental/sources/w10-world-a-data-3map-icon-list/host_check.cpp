#include "3map_icon_list.cpp"
#include <cstring>
int main() {
    if (sizeof(_3map$iconList) != 1344) return 1;
    for (unsigned i = 0; i < sizeof(_3map$iconList); i++) if (((const unsigned char*)&_3map$iconList[0])[i] != 0) return 2;
    return 0;
}
