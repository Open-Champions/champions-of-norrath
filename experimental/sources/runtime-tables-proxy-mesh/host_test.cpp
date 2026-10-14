// Host comparison only. Not an EE build input.
#define NORRATH_PROXY_MESH_HOST 1
#include "proxy_mesh.cpp"

#include <cstdio>
#include <cstring>

static void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

static unsigned bits_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

int main() {
    unsigned char record[56];
    unsigned char description[16];
    std::memset(record, 0, sizeof record);
    std::memset(description, 0, sizeof description);
    norrath_proxy_mesh_host_note(record);
    write_word(description, bits_of(record));
    // Base mesh id 300, flag clear: the halfword alone is returned.
    record[52] = 44;
    record[53] = 1;
    if (norrath_player_proxy_mesh_id(description) != 300) {
        std::printf("clear flag did not return the halfword\n");
        return 1;
    }
    // Negative halfwords sign-extend.
    record[52] = 0;
    record[53] = 0x80;
    if (norrath_player_proxy_mesh_id(description) != -32768) {
        std::printf("negative halfword did not sign-extend\n");
        return 2;
    }
    // Flag set with low bits 7: base + 7.
    record[52] = 44;
    record[53] = 1;
    write_word(description + 8, 0x100007u);
    write_word(description + 12, 0u);
    if (norrath_player_proxy_mesh_id(description) != 307) {
        std::printf("flagged word did not add the low bits\n");
        return 3;
    }
    // Only bit 20 of the whole 64-bit word selects the addition: a set bit
    // 20 in the upper half (whole bit 52) leaves the result unadjusted.
    write_word(description + 8, 0x000007u);
    write_word(description + 12, 0x00100000u >> 0);
    if (norrath_player_proxy_mesh_id(description) != 300) {
        std::printf("upper-half bit 20 must not select the addition\n");
        return 4;
    }
    write_word(description + 8, 0x000007u);
    write_word(description + 12, 0u);
    if (norrath_player_proxy_mesh_id(description) != 300) {
        std::printf("low bits without the flag must be ignored\n");
        return 5;
    }
    // The addition wraps mod 2^32: halfword -1 plus 31 reads back -1 + 31.
    record[52] = 0xff;
    record[53] = 0xff;
    write_word(description + 8, 0x10001fu);
    if (norrath_player_proxy_mesh_id(description) != 30) {
        std::printf("wrap case gave %d\n", norrath_player_proxy_mesh_id(description));
        return 6;
    }
    return 0;
}
