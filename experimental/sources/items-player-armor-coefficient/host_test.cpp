#include <cstring>
extern "C" const unsigned char norrath_item_armor_percent_coefficient_view[];
int main() {
    static const unsigned char want[] = {0x0a, 0xd7, 0x23, 0x3c};
    return std::memcmp(norrath_item_armor_percent_coefficient_view, want, sizeof want) == 0 ? 0 : 1;
}
