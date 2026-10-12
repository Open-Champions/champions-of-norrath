// Observed playerDeallocateArmorCaches__Fv at 0x00195bb8, 256 bytes.
// 1. If the word cacheLoadCache is nonzero, call cdWait__Fv() until the byte
//    cacheCdDone is nonzero (no call at all if it already is), then clear
//    cacheLoadCache.
// 2. Walk a 3520-byte table that the image symbol table does not name (here the
//    import norrath_player_armor_cache_view_7cb770, address 0x007cb770): for
//    group 0..3 (stride 88 bytes), then slot 0..9 (stride 352 bytes), take the
//    pointer at the start of the 88-byte element; if nonzero call memFree__FPv on
//    it, store 0 there and 0 to the halfword at element+10. Then in every case
//    zero all 88 bytes of the element with memset.
// The pointers are 32-bit words on the target; the sources treat them as such.
// 3. If the word eqLumpHeader is nonzero, free it and clear it; do the same for
//    wLumpHeader.
// The meaning of the element fields other than the leading pointer is unknown.
extern "C" {
extern int cacheLoadCache;
extern signed char cacheCdDone;
extern unsigned eqLumpHeader;
extern unsigned wLumpHeader;
extern unsigned char norrath_player_armor_cache_view_7cb770[];
void cdWait__Fv();
void memFree__FPv(void* block);
void* memset(void* dest, int value, unsigned count);

void playerDeallocateArmorCaches__Fv() {
    if (cacheLoadCache != 0) {
        while (cacheCdDone == 0) cdWait__Fv();
        cacheLoadCache = 0;
    }
    for (int group = 0; group < 4; ++group) {
        unsigned char* element = norrath_player_armor_cache_view_7cb770 + group * 88;
        for (int slot = 0; slot < 10; ++slot, element += 352) {
            unsigned& block = *(unsigned*)element;
            if (block != 0) {
                memFree__FPv((void*)(unsigned long)block);
                block = 0;
                *(short*)(element + 10) = 0;
            }
            memset(element, 0, 88);
        }
    }
    if (eqLumpHeader != 0) {
        memFree__FPv((void*)(unsigned long)eqLumpHeader);
        eqLumpHeader = 0;
    }
    if (wLumpHeader != 0) {
        memFree__FPv((void*)(unsigned long)wLumpHeader);
        wLumpHeader = 0;
    }
}
}
