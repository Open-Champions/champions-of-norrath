// Observed: hudInitNet registers hudMsgCallback, stores that result in
// hudMsgHandle, then registers netWaitCallBack and stores that result in
// netWaitMsgHandler. The first store happens before the second registration.
// Inferred: nothing about either callback body.
// Unknown: the meaning of the returned words.

extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int));
extern "C" void hudMsgCallback__FPUci(unsigned char* message, int length);
extern "C" void netWaitCallBack__FPUci(unsigned char* message, int length);
extern "C" int hudMsgHandle;
extern "C" int netWaitMsgHandler;

extern "C" void hudInitNet__Fv() {
    int hud = netRegisterMessageHandler__FPFPUci_v(hudMsgCallback__FPUci);
    hudMsgHandle = hud;
    int wait = netRegisterMessageHandler__FPFPUci_v(netWaitCallBack__FPUci);
    netWaitMsgHandler = wait;
}
