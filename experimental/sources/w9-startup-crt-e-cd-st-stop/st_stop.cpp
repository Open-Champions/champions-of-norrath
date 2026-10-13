// Observed sceCdStStop at 0x002e41a8, 56 bytes. It clears stm_status (0x00447c34), then
// returns sceCdStream(0, 0, 0, 3, dum_mode). dum_mode is the 40-byte object at
// 0x008e83d8. No debug print happens on this path.
extern "C" {
extern int stm_status;
extern unsigned char dum_mode[];
unsigned sceCdStream(unsigned argument, unsigned count, void* buffer, unsigned command, const unsigned char* mode);

unsigned sceCdStStop() {
    stm_status = 0;
    return sceCdStream(0u, 0u, nullptr, 3u, dum_mode);
}
}
