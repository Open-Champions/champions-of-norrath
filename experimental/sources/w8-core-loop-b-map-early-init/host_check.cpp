// Host contract for earlyInit__3mapP12_worldHeader. Not an EE plan input.
#define memcpy host_memcpy
#include "early_init.cpp"

float _3map$mapOffset[2];
int _3map$mapwidth = 77;
int _3map$mapheight = 88;
unsigned char _3map$maptiles[432 * 4 + 64];

static int copies = 0;
static int copyBytes[8];
extern "C" void* memcpy(void* to, const void* from, int count) {
    copyBytes[copies++] = count;
    unsigned char* t = (unsigned char*)to;
    const unsigned char* f = (const unsigned char*)from;
    for (int i = 0; i < count; ++i) t[i] = f[i];
    return to;
}

static void reset() {
    for (unsigned i = 0; i < sizeof _3map$maptiles; ++i) _3map$maptiles[i] = 0xEE;
    _3map$mapOffset[0] = _3map$mapOffset[1] = -1.0f;
    _3map$mapwidth = 77;
    _3map$mapheight = 88;
    copies = 0;
}

int main() {
    alignas(8) unsigned char header[160] = {};
    alignas(8) unsigned char blob[8 + 6 * 3 * 2 + 16] = {};
    short* fields = (short*)blob;
    fields[0] = -5; fields[1] = 9; fields[2] = 3; fields[3] = 3;
    for (int i = 0; i < 9; ++i) ((short*)(blob + 8))[i] = (short)(100 + i);
    *(int*)(header + 108) = 1;
    *(MapRecord**)(header + 128) = (MapRecord*)blob;
    auto view = (WorldHeaderMapView*)header;

    reset();
    earlyInit__3mapP12_worldHeader(view);
    if (_3map$mapwidth != 3 || _3map$mapheight != 3) return 1;
    if (_3map$mapOffset[0] != -5.0f || _3map$mapOffset[1] != 9.0f) return 2;
    if (copies != 3 || copyBytes[0] != 6) return 3;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (((short*)(_3map$maptiles + r * 432))[c] != 100 + r * 3 + c) return 4;
    if (_3map$maptiles[6] != 0xEE || _3map$maptiles[432 + 6] != 0xEE) return 5;

    // Row stride survives a wider width and a negative first field.
    fields[2] = 2; fields[3] = 2;
    reset();
    earlyInit__3mapP12_worldHeader(view);
    if (copies != 2 || copyBytes[1] != 4) return 6;
    if (((short*)(_3map$maptiles + 432))[1] != 103) return 7;

    // Nonzero +112 word: nothing at all.
    *(int*)(header + 112) = 1;
    reset();
    earlyInit__3mapP12_worldHeader(view);
    if (copies != 0 || _3map$mapwidth != 77 || _3map$mapheight != 88 || _3map$mapOffset[0] != -1.0f) return 8;
    *(int*)(header + 112) = 0;

    // Null record clears both sizes.
    *(MapRecord**)(header + 128) = 0;
    reset();
    earlyInit__3mapP12_worldHeader(view);
    if (_3map$mapwidth != 0 || _3map$mapheight != 0 || copies != 0) return 9;
    // Zero +108 clears both sizes even with a record present.
    *(MapRecord**)(header + 128) = (MapRecord*)blob;
    *(int*)(header + 108) = 0;
    *(int*)(header + 112) = 1;
    reset();
    earlyInit__3mapP12_worldHeader(view);
    if (_3map$mapwidth != 0 || _3map$mapheight != 0 || copies != 0) return 10;
    // Zero height: sizes stored, no copy.
    *(int*)(header + 108) = 1;
    *(int*)(header + 112) = 0;
    fields[3] = 0;
    reset();
    earlyInit__3mapP12_worldHeader(view);
    if (_3map$mapheight != 0 || _3map$mapwidth != 2 || copies != 0) return 11;
    return 0;
}
