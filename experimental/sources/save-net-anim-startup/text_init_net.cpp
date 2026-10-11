// Observed: textInitNet registers textHandler and stores the returned word
// in dataHandle.
// Inferred: nothing about the handler body. The slot is the four bytes
// before the next symbol.
// Unknown: the meaning of the returned word.

extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int));
extern "C" void textHandler__FPUci(unsigned char* message, int length);
extern "C" int dataHandle;

extern "C" void textInitNet__Fv() {
    int handle = netRegisterMessageHandler__FPFPUci_v(textHandler__FPUci);
    dataHandle = handle;
}
