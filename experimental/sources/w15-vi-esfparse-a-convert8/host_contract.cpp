#include <string.h>
#include "convert_8.cpp"
int main() {
    alignas(8) unsigned char parse[40 + 1024]; unsigned char src[6] = {0, 2, 255, 1, 7, 9}; unsigned char dst[32]; int store = 6;
    for (int i = 0; i < 1024; ++i) parse[40 + i] = (unsigned char)(i * 7 + (i >> 2));
    memset(dst, 0xee, sizeof dst);
    if (Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, src, dst, 4) != 0) return 1;
    for (int k = 0; k < 4; ++k) {
        const unsigned char* e = parse + 40 + 4 * src[k];
        if (dst[4 * k] != e[2] || dst[4 * k + 1] != e[1] || dst[4 * k + 2] != e[0] || dst[4 * k + 3] != e[3]) return 2;
    }
    if (dst[16] != 0xee) return 3;
    memset(dst, 0xee, sizeof dst);
    if (Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, src, dst, 0) != 0 || dst[0] != 0xee) return 4;
    if (Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, src, dst, -3) != 0 || dst[0] != 0xee) return 5;
    store = 1;
    if (Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, src, dst, 6) != 0 || memcmp(dst, src, 6) != 0 || dst[6] != 0xee) return 6;
    store = 0; if (Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, src, dst, 6) != -1) return 7;
    store = 7; if (Convert8__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, src, dst, 6) != -1) return 8;
    return 0;
}
