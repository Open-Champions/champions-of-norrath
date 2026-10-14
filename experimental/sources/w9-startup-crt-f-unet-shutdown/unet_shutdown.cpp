// Observed unet_shutdown__Fv at 0x0101c820, 76 bytes. When the library
// initialized word (symbol sLibNetInitialized, 0x0118e6b4) is nonzero it
// terminates the network library on the client object `cd` (0x0118e740), calls
// VIFreeUSocketExApi__Fv, clears the initialized word (that store sits in the
// delay slot of the VIFreeUSocketExApi__Fv call), and frees the pointer held
// in net_buf (0x0118e6b0) through __wrap_free. net_buf is not cleared. When the
// word is zero nothing happens. Who allocates net_buf and fills cd is not part
// of this function.
extern "C" {
extern int sLibNetInitialized;
extern void* net_buf;
extern unsigned char cd[];
int sceLibnetTerminate(void* client);
void VIFreeUSocketExApi__Fv();
void __wrap_free(void* block);

void unet_shutdown__Fv() {
    if (sLibNetInitialized != 0) {
        sceLibnetTerminate(cd);
        sLibNetInitialized = 0;
        VIFreeUSocketExApi__Fv();
        __wrap_free(net_buf);
    }
}
}
