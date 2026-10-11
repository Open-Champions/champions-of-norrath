#include "restoration.hpp"
extern "C" unsigned norrath_floor_put_save_info(unsigned char* input) {
    using namespace norrath_restoration_detail;
    const Address start = address(input);
    Address cursor = start;
    norrath_floor_clear();
    const int count = signed_word(read_word(cursor));
    cursor += 8u;
    if (count > 0) {
        unsigned remaining = static_cast<unsigned>(count);
        do {
            const Address record = cursor;
            cursor += 12u;
            const float x = static_cast<float>(signed_half(read_half(record))) * 0.25f;
            const float y = static_cast<float>(signed_half(read_half(record + 2u))) * 0.25f;
            const float z_scaled = static_cast<float>(signed_half(read_half(record + 4u))) * 0.25f;
            const float extent = static_cast<float>(signed_half(read_half(record + 6u))) * 0.25f;
            const int type = signed_half(read_half(record + 8u));
            const int kind = signed_byte(*bytes(record + 10u));
            NorrathPoint3 point{x, y, z_scaled - 3.0f};
            norrath_floor_add(bytes(address(&point)), type, extent, kind, 0);
            remaining -= 1u;
        } while (remaining != 0u);
    }
    return cursor - start;
}
