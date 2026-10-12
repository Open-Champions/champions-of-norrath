// Observed sceCdStInit at 0x002e40e0, 44 bytes. It clears stm_status
// (0x00447c34), then returns sceCdStream(a0, a1, a2, 5, dum_mode) with the
// three incoming arguments passed through unchanged. dum_mode is the 40-byte
// object at 0x008e83d8.
extern "C" {
extern int stm_status;
extern unsigned char dum_mode[];
unsigned sceCdStream(unsigned argument, unsigned count, void* buffer, unsigned command, const unsigned char* mode);
unsigned sceCdStInit(unsigned buffer_max, unsigned bank_max, void* iop_buffer) {
    stm_status = 0;
    return sceCdStream(buffer_max, bank_max, iop_buffer, 5u, dum_mode);
}
}
