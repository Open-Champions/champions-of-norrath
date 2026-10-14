// Observed: commInitNet registers commHandler with the packet registrar and
// stores that result in commHandle before registering commStatusHandler with
// the message registrar. The second result is stored in commStatusHandle.
// Inferred: nothing about either handler body.
// Unknown: the meaning of the returned words.

extern "C" int netRegisterPacketHandler__FPFPUci_v(void (*handler)(unsigned char*, int));
extern "C" int netRegisterMessageHandler__FPFPUci_v(void (*handler)(unsigned char*, int));
extern "C" void commHandler__FPUci(unsigned char* message, int length);
extern "C" void commStatusHandler__FPUci(unsigned char* message, int length);
extern "C" int commHandle;
extern "C" int commStatusHandle;

extern "C" void commInitNet__Fv() {
    int packet = netRegisterPacketHandler__FPFPUci_v(commHandler__FPUci);
    commHandle = packet;
    int status = netRegisterMessageHandler__FPFPUci_v(commStatusHandler__FPUci);
    commStatusHandle = status;
}
