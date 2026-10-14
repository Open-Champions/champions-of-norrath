// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp   (the source is included)
#include "deci2_user_area.cpp"
static_assert(sizeof(norrath_deci2_user_area_view) == 0x8e5780 - 0x8e4748, "span up to the next recorded symbol");
static_assert(__alignof__(norrath_deci2_user_area_view) >= 8, "original address is 8-byte aligned");
int main() {
    for (unsigned char byte : norrath_deci2_user_area_view) if (byte != 0) return 1;
    return 0;
}
