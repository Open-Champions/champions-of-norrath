// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
// Pointer words are native width on the host; strides (12, 4) are the image's.
#include "raster-get-material-pal-entry.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char raster[19500] = {0};
    alignas(8) unsigned char table[12 * 4] = {0};
    alignas(8) unsigned char record0[32] = {0}, record3[32] = {0};
    int pal0[4] = {10, 11, 12, 13}, pal3[6] = {20, 21, 22, 23, 24, 25};
    const void* p;
    p = table; std::memcpy(raster + 19436, &p, sizeof p);
    p = record0; std::memcpy(table + 0, &p, sizeof p);
    p = record3; std::memcpy(table + 36, &p, sizeof p);
    p = pal0; std::memcpy(record0 + 12, &p, sizeof p);
    p = pal3; std::memcpy(record3 + 12, &p, sizeof p);
    if (GetMaterialPalEntry__8VIRasterii(raster, 0, 0) != 10) return 1;
    if (GetMaterialPalEntry__8VIRasterii(raster, 0, 3) != 13) return 2;
    if (GetMaterialPalEntry__8VIRasterii(raster, 3, 0) != 20) return 3;
    if (GetMaterialPalEntry__8VIRasterii(raster, 3, 5) != 25) return 4;
    return 0;
}
