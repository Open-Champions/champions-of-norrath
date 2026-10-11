#include "stream_storage.cpp"

#include <cstdio>

static unsigned word_at(const unsigned char* bytes, unsigned index) {
    const unsigned char* p = bytes + index * 4u;
    return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

int main() {
    static_assert(sizeof norrath_sound_stream_buffer_sizes_view == 12, "three sizes");
    static_assert(sizeof norrath_sound_stream_volumes_view == 12, "three volumes");
    static_assert(sizeof norrath_sound_stream_records_view == 3u * 452u, "three records");
    if (word_at(norrath_sound_stream_buffer_sizes_view, 0) != 0x0004b000u) return 1;
    if (word_at(norrath_sound_stream_buffer_sizes_view, 1) != 0x00001000u) return 2;
    if (word_at(norrath_sound_stream_buffer_sizes_view, 2) != 0x0001e000u) return 3;
    for (unsigned i = 0; i < 3; ++i)
        if (word_at(norrath_sound_stream_volumes_view, i) != 0x3f800000u) return 4;
    for (unsigned char byte : norrath_sound_stream_records_view)
        if (byte != 0) return 5;
    std::puts("stream storage passed");
    return 0;
}
