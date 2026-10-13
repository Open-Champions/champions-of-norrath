#include "get_material_pal_size.cpp"
#include <stdint.h>
#include <string.h>

int main() {
    // 32-bit pointer words are modelled with an arena on the host: the function reads
    // pointers as native words, so build the structure with native pointers.
    alignas(16) static unsigned char raster[20000];
    static unsigned char table[12 * 4];
    static int recs[4][4];
    for (int i = 0; i < 4; ++i) {
        recs[i][2] = 1000 + i * 7;
        unsigned char* p = reinterpret_cast<unsigned char*>(recs[i]);
        memcpy(table + i * 12, &p, sizeof p);
    }
    unsigned char* t = table;
    memcpy(raster + 19436, &t, sizeof t);
    for (int i = 0; i < 4; ++i)
        if (GetMaterialPalSize__8VIRasteri(raster, i) != 1000 + i * 7) return 1;
    return 0;
}
