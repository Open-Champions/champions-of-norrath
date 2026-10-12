// Observed netWaitCallBack__FPUci at 0x001c8070, 8 bytes: a return and its delay
// slot. The image symbol is file-local; this definition is a normal global so
// the address-taking callers can link. The arguments are not read.
extern "C" void netWaitCallBack__FPUci(unsigned char* packet, int length) {
    (void)packet;
    (void)length;
}
