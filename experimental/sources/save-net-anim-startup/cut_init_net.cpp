// Observed: cutInitNet registers cutMsgHandlerFunction and stores the
// returned word in cutMsgHandler.
// Inferred: nothing about the handler body. The slot is the four bytes
// before the next symbol.
// Unknown: the meaning of the returned word.

extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int));
extern "C" void cutMsgHandlerFunction__FPUci(unsigned char* message, int length);
extern "C" int cutMsgHandler;

extern "C" void cutInitNet__Fv() {
    int handle = netRegisterMessageHandler__FPFPUci_v(cutMsgHandlerFunction__FPUci);
    cutMsgHandler = handle;
}
