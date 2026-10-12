#define NORRATH_SQ_HOST 1
#include "amx_get_addr.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char image[64];
    unsigned char amx[40];
    unsigned char out[4];
    std::memset(image, 0, sizeof image);
    std::memset(amx, 0, sizeof amx);
    sq::write_word(image + 16, 8u);
    sq::write_word(amx, sq::pointer_bits(image));
    sq::write_word(amx + 20, 100u);
    sq::write_word(amx + 24, 200u);
    sq::write_word(amx + 28, 300u);
    int rc;
    std::memset(out, 0x5a, sizeof out);
    rc = amx_GetAddr(amx, 10, reinterpret_cast<int**>(out));
    if (rc != 0 || sq::pointer(sq::read_word(out)) != image + 18) return 1;
    // below word20 and at/above word28 bounds
    rc = amx_GetAddr(amx, 99, reinterpret_cast<int**>(out));
    if (rc != 0 || sq::pointer(sq::read_word(out)) != image + 107) return 2;
    // the [100,200) gap fails and leaves the output alone
    std::memset(out, 0x5a, sizeof out);
    if (amx_GetAddr(amx, 100, reinterpret_cast<int**>(out)) != 5 || out[0] != 0x5a) return 3;
    if (amx_GetAddr(amx, 199, reinterpret_cast<int**>(out)) != 5 || out[3] != 0x5a) return 4;
    // 200 is outside the gap and below 300
    if (amx_GetAddr(amx, 200, reinterpret_cast<int**>(out)) != 0 ||
        sq::pointer(sq::read_word(out)) != image + 208) return 5;
    if (amx_GetAddr(amx, 300, reinterpret_cast<int**>(out)) != 5) return 6;
    if (amx_GetAddr(amx, 1000, reinterpret_cast<int**>(out)) != 5) return 7;
    // signed compare: a negative address is accepted
    sq::write_word(image + 16, 40u);
    if (amx_GetAddr(amx, -8, reinterpret_cast<int**>(out)) != 0 ||
        sq::pointer(sq::read_word(out)) != image + 32) return 8;
    std::puts("ok");
    return 0;
}
