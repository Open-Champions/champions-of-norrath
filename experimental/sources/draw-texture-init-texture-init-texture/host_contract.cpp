#include <cstdint>
#include <cstring>
#include <cstdio>

extern "C" void textureInitTexture__FP8_texture(unsigned char* texture);

static int fail(int code) { return code; }

static unsigned low32(const unsigned char* address) {
    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(address));
}

static unsigned word_at(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

static unsigned short half_at(const unsigned char* address) {
    return static_cast<unsigned short>(address[6] | (address[7] << 8));
}

int main() {
    alignas(16) unsigned char already[128] = {};
    already[28] = 1;
    unsigned char copy[128];
    std::memcpy(copy, already, sizeof already);
    textureInitTexture__FP8_texture(already);
    if (std::memcmp(already, copy, sizeof already) != 0) return fail(1);

    alignas(16) unsigned char bit0[128] = {};
    bit0[8] = 1;
    bit0[16] = 0x20;
    textureInitTexture__FP8_texture(bit0);
    if (half_at(bit0) != 0) return fail(2);
    if (bit0[10] != 0xfb || bit0[11] != 0xa7) return fail(3);
    if (word_at(bit0 + 16) != low32(bit0) + 0x20u) return fail(4);
    if (word_at(bit0 + 28) != 0xffffffffu) return fail(5);

    alignas(16) unsigned char neither[128] = {};
    neither[16] = 0x20;
    textureInitTexture__FP8_texture(neither);
    if (half_at(neither) != 0 || word_at(neither + 16) != low32(neither) + 0x20u) return fail(6);
    if (neither[10] != 0xfb || word_at(neither + 28) != 0xffffffffu) return fail(7);

    alignas(16) unsigned char both[128] = {};
    both[8] = 1;
    both[9] = 1;
    both[16] = 0x20;
    both[0x24] = 0x11;
    textureInitTexture__FP8_texture(both);
    if (half_at(both) != 0 || both[0x24] != 0x11) return fail(8);

    alignas(16) unsigned char empty[128] = {};
    empty[9] = 1;
    empty[16] = 0x20;
    empty[0x20] = 0x40;
    empty[0x24] = 0xff;
    textureInitTexture__FP8_texture(empty);
    if (half_at(empty) != 72) return fail(9);
    if (word_at(empty + 0x20) != low32(empty) + 0x40u) return fail(10);
    if (empty[0x24] != 0xff) return fail(11);

    alignas(16) unsigned char one[128] = {};
    one[9] = 1;
    one[16] = 0x20;
    one[0x20] = 0x10;
    one[0x28] = 0x05;
    one[0x2c] = 0xff;
    textureInitTexture__FP8_texture(one);
    if (half_at(one) != 94) return fail(12);
    if (word_at(one + 0x20) != low32(one) + 0x10u) return fail(13);
    if (word_at(one + 0x28) != low32(one) + 0x05u) return fail(14);

    alignas(16) unsigned char wide[128] = {};
    wide[9] = 1;
    wide[16] = 0x20;
    wide[0x26] = 1;
    wide[0x28] = 0x11;
    wide[0x2c] = 0x22;
    wide[0x30] = 0xff;
    textureInitTexture__FP8_texture(wide);
    if (half_at(wide) != 110) return fail(15);
    if (word_at(wide + 0x28) != low32(wide) + 0x11u) return fail(16);
    if (word_at(wide + 0x2c) != low32(wide) + 0x22u) return fail(17);

    alignas(16) unsigned char negative[128] = {};
    negative[9] = 1;
    negative[16] = 0x20;
    negative[0x27] = 0xfe;
    negative[0x28] = 0xff;
    textureInitTexture__FP8_texture(negative);
    if (half_at(negative) != 62) return fail(18);
    if (negative[0x24] != 0 || negative[0x27] != 0xfe || negative[0x28] != 0xff) return fail(19);
    if (word_at(negative + 0x20) != low32(negative)) return fail(20);

    alignas(16) unsigned char empty_row[128] = {};
    empty_row[9] = 1;
    empty_row[16] = 0x20;
    empty_row[0x24] = 2;
    empty_row[0x28] = 0xff;
    empty_row[0x29] = 0xff;
    empty_row[0x2a] = 0xff;
    empty_row[0x2b] = 0xff;
    textureInitTexture__FP8_texture(empty_row);
    if (half_at(empty_row) != 62) return fail(21);
    if (word_at(empty_row + 0x28) != 0xffffffffu) return fail(22);

    alignas(16) unsigned char two[160] = {};
    two[9] = 1;
    two[16] = 0x20;
    two[0x28] = 0x07;
    two[0x2e] = 1;
    two[0x30] = 0x01;
    two[0x34] = 0x02;
    two[0x38] = 0xff;
    textureInitTexture__FP8_texture(two);
    if (half_at(two) != 132) return fail(23);
    if (word_at(two + 0x28) != low32(two) + 0x07u) return fail(24);
    if (word_at(two + 0x30) != low32(two) + 0x01u) return fail(25);
    if (word_at(two + 0x34) != low32(two) + 0x02u) return fail(26);

    alignas(16) unsigned char signed_byte[128] = {};
    signed_byte[9] = 1;
    signed_byte[16] = 0x20;
    signed_byte[0x24] = 0xfe;
    signed_byte[0x28] = 0x03;
    signed_byte[0x2c] = 0x04;
    signed_byte[0x30] = 0x05;
    signed_byte[0x34] = 0xff;
    textureInitTexture__FP8_texture(signed_byte);
    if (half_at(signed_byte) != 126) return fail(27);
    if (word_at(signed_byte + 0x28) != low32(signed_byte) + 0x03u) return fail(28);
    if (word_at(signed_byte + 0x2c) != low32(signed_byte) + 0x04u) return fail(29);
    if (word_at(signed_byte + 0x30) != low32(signed_byte) + 0x05u) return fail(38);

    alignas(16) unsigned char tall[128] = {};
    tall[9] = 1;
    tall[16] = 0x20;
    tall[0x27] = 1;
    tall[0x28] = 0x0a;
    tall[0x2c] = 0x0b;
    tall[0x30] = 0xff;
    textureInitTexture__FP8_texture(tall);
    if (half_at(tall) != 110) return fail(30);
    if (word_at(tall + 0x28) != low32(tall) + 0x0au) return fail(31);
    if (word_at(tall + 0x2c) != low32(tall) + 0x0bu) return fail(32);

    const unsigned char ntsc[64] = {
        0x80, 0x02, 0xe0, 0x00, 0xc0, 0x08, 0x08, 0x8c, 0x60, 0x00, 0x00, 0x00, 0x0a, 0xa0, 0x18, 0x00,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    alignas(16) unsigned char legal[64];
    std::memcpy(legal, ntsc, sizeof legal);
    textureInitTexture__FP8_texture(legal);
    if (half_at(legal) != 0x8c08) return fail(33);
    if (legal[10] != 0xfb || legal[11] != 0xa7) return fail(34);
    if (word_at(legal + 16) != low32(legal) + 0x80u) return fail(35);
    if (word_at(legal + 28) != 0xffffffffu) return fail(36);
    if (legal[0] != 0x80 || legal[8] != 0x60) return fail(37);

    std::puts("textureInitTexture contract passed");
    return 0;
}
