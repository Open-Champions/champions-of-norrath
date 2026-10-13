#include "dbuffer.cpp"

int main() {
    if (sizeof norrath_raster_dbuffer != 0x330) return 1;
    for (unsigned i = 0; i < sizeof norrath_raster_dbuffer; ++i)
        if (norrath_raster_dbuffer[i] != 0) return 2;
    norrath_raster_dbuffer[0x2f0] = 7;
    return norrath_raster_dbuffer[0x2f0] == 7 ? 0 : 3;
}
