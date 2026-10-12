#include "storage.hpp"
int main() {
    static const unsigned char expect[4] = { 0x00, 0x01, 0x00, 0x38 };
    if (sizeof(norrath_forbidden_orientation_scale_bytes) != 4) return 1;
    for (unsigned i = 0; i != 4; ++i) {
        if (norrath_forbidden_orientation_scale_bytes[i] != expect[i]) return 2;
    }
    return 0;
}
