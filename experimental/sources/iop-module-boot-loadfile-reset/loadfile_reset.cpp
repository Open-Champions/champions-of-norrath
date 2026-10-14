// Observed sceSifLoadFileReset at 0x002dc790, 56 bytes. It stores -1 in
// _bind_check at 0x00444a10 and clears the first four bytes of _lfversion at
// 0x008e20a8 through memset, then returns 0. _bind_check is four file-backed
// bytes whose image value is -1. Its original binding is local. _lfversion
// is .bss, st_size 0. rdata is 24 bytes later. Only four bytes are cleared.
// _lf_bind and _lf_version read these same objects.
extern "C" {
int _bind_check = -1;
unsigned char _lfversion[4] = {};
}
extern "C" void* memset(void* destination, int value, unsigned count);

extern "C" int sceSifLoadFileReset() {
    _bind_check = -1;
    memset(_lfversion, 0, 4);
    return 0;
}
