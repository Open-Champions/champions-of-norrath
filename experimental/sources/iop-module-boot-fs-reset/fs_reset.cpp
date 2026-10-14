// Observed sceFsReset at 0x002d92c0, 52 bytes. It stores 0 in _fs_init at
// 0x004449f4 and clears the first four bytes of _fsversion at 0x008e1be8
// through memset, then returns 0. _fs_init is four file-backed zero bytes.
// Its original binding is local. _fsversion is .bss, st_size 0. The next
// symbol, _sif_FsRcv_Data, is 24 bytes later. Only four bytes are cleared.
extern "C" {
int _fs_init = 0;
unsigned char _fsversion[4] = {};
}
extern "C" void* memset(void* destination, int value, unsigned count);

extern "C" int sceFsReset() {
    _fs_init = 0;
    memset(_fsversion, 0, 4);
    return 0;
}
