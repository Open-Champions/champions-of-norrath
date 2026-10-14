// Host contract for reset__3Web; not an EE input. Includes the source.
#include "reset.cpp"
#include <cstring>
int main() {
    unsigned char web[96];
    std::memset(web, 0x5a, sizeof web);
    unsigned char before[96];
    std::memcpy(before, web, sizeof web);
    reset__3Web(web);
    for (int i = 0; i < 96; ++i) {
        unsigned char want = before[i];
        if (i == 44) want = 1;
        else if (i >= 45 && i < 48) want = 0;
        if (web[i] != want) return 1;
    }
    return 0;
}
