#include "amx_get_string.cpp"
#include <cstdio>
#include <cstring>
int main() {
    char out[32];
    // packed: "ABCDE" -> cells 'A''B''C''D', 'E' 0 0 0
    int packed[3] = {0x41424344, 0x45000000, 0x7fffffff};
    std::memset(out, 0x7e, sizeof out);
    if (amx_GetString(out, packed) != 0) return 1;
    if (std::strcmp(out, "ABCDE") != 0 || out[6] != 0x7e) return 2;
    // packed, length a multiple of four: terminator comes from the next cell
    int exact[2] = {0x61626364, 0};
    std::memset(out, 0x7e, sizeof out);
    amx_GetString(out, exact);
    if (std::strcmp(out, "abcd") != 0 || out[5] != 0x7e) return 3;
    // unpacked: low byte of each cell, stops at a zero cell
    int plain[4] = {'h', 'i', 'A', 0};
    std::memset(out, 0x7e, sizeof out);
    amx_GetString(out, plain);
    if (std::strcmp(out, "hiA") != 0 || out[4] != 0x7e) return 4;
    // unpacked: only the low byte is kept
    int wide[3] = {0x41, 0x142, 0};
    amx_GetString(out, wide);
    if (std::strcmp(out, "AB") != 0) return 5;
    // an unpacked cell whose low byte is zero but cell is nonzero keeps going
    int gap[3] = {'x', 0x100, 0};
    std::memset(out, 0x7e, sizeof out);
    amx_GetString(out, gap);
    if (out[0] != 'x' || out[1] != 0 || out[2] != 0 || out[3] != 0x7e) return 6;
    // empty
    int empty[1] = {0};
    std::memset(out, 0x7e, sizeof out);
    amx_GetString(out, empty);
    if (out[0] != 0 || out[1] != 0x7e) return 7;
    // negative first cell is unpacked
    int neg[2] = {-1, 0};
    std::memset(out, 0x7e, sizeof out);
    amx_GetString(out, neg);
    if (static_cast<unsigned char>(out[0]) != 0xff || out[1] != 0) return 8;
    std::puts("ok");
    return 0;
}
