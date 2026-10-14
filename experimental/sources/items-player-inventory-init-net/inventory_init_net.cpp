// Observed inventoryInitNet__Fv at 0x1b1cb0, 36 bytes. It registers
// invMsgCallback__FPUci and stores the handler word at invMsgHandle (0x486a88).
// That symbol's declared size is 0. The next symbol is 8 bytes later; this
// object is the 4-byte word the store writes.
extern "C" void invMsgCallback__FPUci(unsigned char* message, int length);
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*callback)(unsigned char*, int));
extern "C" int invMsgHandle = 0;

extern "C" void inventoryInitNet__Fv() {
    invMsgHandle = netRegisterMessageHandler__FPFPUci_v(invMsgCallback__FPUci);
}
