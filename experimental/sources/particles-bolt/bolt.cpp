// bolt at 0x006f6e50, 36880 bytes, to wigglyTab.
// 20 records of 1844. BSS is zero. initTrails clears byte +1836 later.
extern "C" {
unsigned char bolt[20 * 1844];
}
static_assert(sizeof bolt == 36880u, "bolt extent");
