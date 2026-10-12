#include <cstdio>
#include <cstring>

extern "C" {
void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, void* texture,
                                                   void* draw_record, int flags);
unsigned char drawRecords[0x12000];
unsigned char drawHead[64];
int writeBuffer;
int nmDrawUsed;
int textureMaxBlockSize;
unsigned drawFrameNm;
}
unsigned char* norrath_draw_add_arena = drawRecords;

namespace {
unsigned word_at(const void* address) {
    const auto* bytes = static_cast<const unsigned char*>(address);
    return bytes[0] | (unsigned(bytes[1]) << 8) | (unsigned(bytes[2]) << 16) |
           (unsigned(bytes[3]) << 24);
}
void set_word(void* address, unsigned value) {
    auto* bytes = static_cast<unsigned char*>(address);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
}
unsigned bits(const void* pointer) {
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - drawRecords);
}
int fail(int code) {
    std::fprintf(stderr, "draw add failed %d\n", code);
    return code;
}
}

int main() {
    unsigned char* texture = drawRecords + 0x10000;
    unsigned char* head = drawRecords + 0x10100;
    unsigned char* packet = drawRecords + 0x10200;
    unsigned char* kept = drawRecords + 0x10300;
    unsigned char* marker = drawRecords + 0x10400;
    unsigned char* first = drawRecords + 0x10500;
    unsigned char* second = drawRecords + 0x10600;

    writeBuffer = 1;
    nmDrawUsed = 2048;
    if (drawAddToList__FPviP8_textureP11_drawRecordi(packet, 0, nullptr, nullptr, 0) != nullptr ||
        nmDrawUsed != 2048) {
        return fail(2);
    }

    nmDrawUsed = 8;
    textureMaxBlockSize = 100;
    drawFrameNm = 7;
    std::memset(texture, 0x5a, 68);
    texture[4] = 40;
    texture[5] = 0;
    set_word(texture + 28, 1);
    std::memset(head, 0, 16);
    set_word(head + 12, 0x44u);
    set_word(drawHead + 32, bits(head));
    void* record = drawAddToList__FPviP8_textureP11_drawRecordi(packet, 0, texture, nullptr, 0);
    const unsigned expect_slot = ((1u << 11) + 8u) << 4;
    if (record != drawRecords + expect_slot) return fail(3);
    if (nmDrawUsed != 9) return fail(4);
    if (word_at(record) != (bits(packet) & 0x0fffffffu)) return fail(5);
    if (word_at(static_cast<unsigned char*>(record) + 4) != 0u) return fail(6);
    if (word_at(static_cast<unsigned char*>(record) + 8) != bits(texture)) return fail(7);
    if (word_at(static_cast<unsigned char*>(record) + 12) != 0x44u) return fail(8);
    if (word_at(head + 12) != bits(record)) return fail(9);
    if (word_at(texture + 28) != 7u) return fail(10);
    for (int word = 0; word < 8; ++word) {
        if (word_at(texture + 32 + word * 4) != (word == 0 ? bits(record) : 0u)) return fail(11);
    }
    if (texture[64] != 0x5a) return fail(12);

    set_word(texture + 36, 0x55u);
    texture[4] = 100;
    unsigned char* spare = drawRecords + 0x10700;
    std::memset(spare, 0, 16);
    set_word(drawHead + 36, bits(spare));
    void* rejected = drawAddToList__FPviP8_textureP11_drawRecordi(packet, 1, texture, nullptr, 1);
    if (word_at(static_cast<unsigned char*>(rejected) + 8) != 0u) return fail(13);
    if (word_at(texture + 36) != 0x55u) return fail(14);

    texture[4] = 10;
    std::memset(marker, 0, 16);
    set_word(marker + 12, bits(kept));
    void* linked = drawAddToList__FPviP8_textureP11_drawRecordi(packet, 0, texture, marker, 4);
    if (word_at(marker + 4) != 2u) return fail(15);
    if (word_at(marker + 12) != bits(linked)) return fail(16);
    if (word_at(static_cast<unsigned char*>(linked) + 12) != bits(kept)) return fail(17);
    if ((word_at(static_cast<unsigned char*>(linked) + 4) & 2u) != 0u) return fail(18);

    set_word(marker + 4, 2);
    void* flagged = drawAddToList__FPviP8_textureP11_drawRecordi(packet, 0, nullptr, marker, 4);
    if ((word_at(static_cast<unsigned char*>(flagged) + 4) & 2u) == 0u) return fail(19);

    std::memset(first, 0, 16);
    std::memset(second, 0, 16);
    set_word(first + 4, 2);
    set_word(first + 12, bits(second));
    set_word(second + 12, 0x77u);
    set_word(texture + 28, drawFrameNm);
    set_word(texture + 32, bits(first));
    void* walked = drawAddToList__FPviP8_textureP11_drawRecordi(packet, 0, texture, nullptr, 0);
    if (word_at(second + 12) != bits(walked)) return fail(20);
    if (word_at(static_cast<unsigned char*>(walked) + 12) != 0x77u) return fail(21);
    if (word_at(first + 12) != bits(second)) return fail(22);
    return 0;
}
