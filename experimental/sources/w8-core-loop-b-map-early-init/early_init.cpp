// Observed earlyInit__3mapP12_worldHeader at 0x0015c1d8, 324 bytes (a static function of the map class).
// Header words read: +108, +128 (a pointer) and +112. When the word at +108 is zero, or the pointer at +128 is
// null, _3map$mapwidth (0x00483ec0) and _3map$mapheight (0x00483ec4) are set to zero and nothing else happens.
// Otherwise, when the word at +112 is nonzero, nothing happens. Otherwise the record at the +128 pointer is read:
// halfwords at +0 and +2 become the floats _3map$mapOffset[0] and [1] (0x00483e80), the halfword at +4 becomes
// mapwidth and the halfword at +6 becomes mapheight. Starting 8 bytes after the record, one row of
// (short)width * 2 bytes per step is copied to _3map$maptiles (0x003347c0) at a 432-byte stride, for mapheight
// rows (mapheight is read again after each copy). The source advances by the same row byte count.
// Then a delay loop runs mapheight times over mapwidth iterations; its body is five nop words and has no
// memory effect.
// Unknown: what the words at +108 and +112 of the header mean, why the empty loop exists, and whether the
// rows are 16-bit tiles (the 432 stride is 216 halfwords; inferred from the byte counts only).
extern "C" void* memcpy(void* to, const void* from, int count);
extern "C" float _3map$mapOffset[2];
extern "C" int _3map$mapwidth;
extern "C" int _3map$mapheight;
extern "C" unsigned char _3map$maptiles[];

struct MapRecord {
    short first;
    short second;
    unsigned short width;
    short height;
};
struct WorldHeaderMapView {
    unsigned char before[108];
    int flagA;
    int flagB;
    unsigned char between[12];
    MapRecord* record;
};

extern "C" void earlyInit__3mapP12_worldHeader(WorldHeaderMapView* header) {
    if (header->flagA == 0) {
        _3map$mapwidth = 0;
        _3map$mapheight = 0;
        return;
    }
    MapRecord* record = header->record;
    if (record == 0) {
        _3map$mapwidth = 0;
        _3map$mapheight = 0;
        return;
    }
    if (header->flagB != 0) return;

    float second = (float)record->second;
    float first = (float)record->first;
    _3map$mapwidth = (short)record->width;
    _3map$mapOffset[1] = second;
    _3map$mapOffset[0] = first;
    _3map$mapheight = record->height;

    const unsigned char* source = (const unsigned char*)record + 8;
    unsigned char* destination = _3map$maptiles;
    if (_3map$mapheight > 0) {
        int row = 0;
        do {
            memcpy(destination, source, (short)record->width * 2);
            source += (short)record->width * 2;
            destination += 432;
        } while (++row < _3map$mapheight);
    }
    for (int h = _3map$mapheight; h > 0; --h) {
        for (int w = _3map$mapwidth; w > 0; --w)
            __asm__ volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop");
    }
}
