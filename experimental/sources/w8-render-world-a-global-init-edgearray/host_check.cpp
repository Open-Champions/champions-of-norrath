extern "C" int zeroTile;
int zeroTile = 0x55;
#include "global_init_edgearray.cpp"
int main() {
    _GLOBAL_$I$edgearray();
    if (zeroTile != 0) return 1;
    zeroTile = 0x55;
    // The routine only acts for (1, 0xffff); calling the wrapper twice keeps the word at zero.
    _GLOBAL_$I$edgearray();
    return zeroTile != 0;
}
