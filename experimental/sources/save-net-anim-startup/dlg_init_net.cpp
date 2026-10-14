// Observed: DLG_InitNet registers dialogMsgCallback and stores the returned
// word in dialogMsgHandle.
// Inferred: nothing about the callback body. The slot is the four bytes
// before the next symbol.
// Unknown: the meaning of the returned word.

extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int));
extern "C" void dialogMsgCallback__FPUci(unsigned char* message, int length);
extern "C" int dialogMsgHandle;

extern "C" void DLG_InitNet__Fv() {
    int handle = netRegisterMessageHandler__FPFPUci_v(dialogMsgCallback__FPUci);
    dialogMsgHandle = handle;
}
