// Observed: Init__t5VIMap2ZiZii at 0x01227060 is 92 bytes. With (map, n) it calls
// Clear__t5VIMap2ZiZi(map) and returns -1 if that is negative. Otherwise it stores n + 1 as the
// capacity word at map+4, calls Allocate__t5VIMap2ZiZi(map), and returns -1 if that is negative
// and 0 if not.
// Inferred: n is the number of elements the caller wants; the extra one is the nil element
// at index 0. Clear and Allocate are the map's own routines. Unknown: the template parameters
// beyond the mangled name.
extern "C" {
int Clear__t5VIMap2ZiZi(void* map);
int Allocate__t5VIMap2ZiZi(void* map);

int Init__t5VIMap2ZiZii(void* map, int n) {
    if (Clear__t5VIMap2ZiZi(map) < 0) return -1;
    *reinterpret_cast<int*>(static_cast<unsigned char*>(map) + 4) = n + 1;
    return Allocate__t5VIMap2ZiZi(map) < 0 ? -1 : 0;
}
}
