// Observed shopInitNet__Fv at 0x1bc018, 36 bytes. It registers
// shopMsgCallback__FPUci and stores the handler word at shopMsgHandle (0x486ab0).
// Declared size 0. The next symbol, iRandState, is 4 bytes later.
extern "C" void shopMsgCallback__FPUci(unsigned char* message, int length);
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*callback)(unsigned char*, int));
extern "C" int shopMsgHandle = 0;

extern "C" void shopInitNet__Fv() {
    shopMsgHandle = netRegisterMessageHandler__FPFPUci_v(shopMsgCallback__FPUci);
}
