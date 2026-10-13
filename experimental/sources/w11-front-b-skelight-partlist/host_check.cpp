// Host check for partlist.1103. Not an EE plan input.
#include "skelight_partlist.cpp"
int main() {
    static_assert(sizeof(norrath_skelight_partlist_1103) == 176, "span to the next symbol");
    for (unsigned char b : norrath_skelight_partlist_1103) if (b != 0) return 1;
    return reinterpret_cast<__UINTPTR_TYPE__>(norrath_skelight_partlist_1103) % 16 == 0 ? 0 : 2;
}
