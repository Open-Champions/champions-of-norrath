// Host contract for dropItem__8Skelighti; not an EE input. Zeroes exactly 8 bytes of partlist.1103.
#include <cstring>
#include <initializer_list>
extern "C" unsigned char norrath_skelight_partlist_1103[32] __asm__("partlist.1103");
unsigned char norrath_skelight_partlist_1103[32];
#include "drop_item.cpp"
int main() {
    int bad = 0;
    for (int arg : {0, 1, 7, -1}) {
        std::memset(norrath_skelight_partlist_1103, 0xa5, 32);
        dropItem__8Skelighti(nullptr, arg);
        for (int i = 0; i < 32; ++i) if (norrath_skelight_partlist_1103[i] != (i < 8 ? 0 : 0xa5)) bad = 1;
    }
    return bad;
}
